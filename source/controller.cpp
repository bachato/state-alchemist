#include "controller.h"


Controller::Controller(u64 titleId) {
  this->titleId = titleId;
}

/**
 * Load all groups from the game folder
 */
std::vector<std::string> Controller::loadGroups() {
  this->openSdCardIfNeeded();

  std::vector<std::string> groups;

  for (auto& node: std::filesystem::directory_iterator(this->getGamePath())) {
    if (node.is_directory()) {
      groups.push_back(node.path().filename().string());
    }
  }

  return groups;
}

/**
 * Load all source options within the specified group
 */
std::vector<std::string> Controller::loadSources(std::string group) {
  this->openSdCardIfNeeded();

  std::vector<std::string> sources;

  for (auto& node: std::filesystem::directory_iterator(this->getGroupPath(std::move(group)))) {
    if (node.is_directory()) {
      sources.push_back(node.path().filename().string());
    }
  }

  return sources;
}

/**
 * Load all mod options that could be activated for the moddable source in the group
 */
std::vector<std::string> Controller::loadMods(std::string source, std::string group) {
  this->openSdCardIfNeeded();

  std::vector<std::string> mods;

  for (auto& node: std::filesystem::directory_iterator(this->getSourcePath(std::move(group), std::move(source)))) {
    if (node.is_directory()) {
      mods.push_back(node.path().filename().string());
    }
  }

  return mods;
}

/**
 * Gets the mod currently activated for the moddable source in the group
 *
 * Returns an empty string if no mod is active and vanilla files are being used
 */
std::string Controller::getActiveMod(std::string source, std::string group) {
  this->openSdCardIfNeeded();

  for (auto& node: std::filesystem::directory_iterator(this->getSourcePath(std::move(group), std::move(source)))) {
    if (node.is_regular_file()) {
      std::string filename = node.path().filename().string();

      if (filename.ends_with(TXT_EXT)) {
        return filename.substr(0, filename.size() - TXT_EXT.size());
      }
    }
  }

  return "";
}

/**
 * Activates the specified mod, moving all its files into the atmosphere folder for the game
 */
void Controller::activateMod(std::string source, std::string group, std::string mod) {
  this->openSdCardIfNeeded();

  std::string modPath = this->getModPath(group, source, mod);
  std::string movedFilesListFileName { this->getMovedFilesListFileName(std::move(group), std::move(source), std::move(mod)) };
  {
    std::ofstream ostrm(std::move(movedFilesListFileName));

    for (auto& node: std::filesystem::recursive_directory_iterator(modPath)) {
      if (node.is_regular_file()) {
        std::string_view atmospherePath = this->getAtmosphereModPath(modPath.size(), node.path().string());

        if (!std::filesystem::exists(atmospherePath)) {
          ostrm << node.path().string() << std::endl;
          std::filesystem::rename(node.path(), atmospherePath);
        }
      }
    }
  }
}

/**
 * Deactivates the currently active mod, restoring the moddable source to its vanilla state
 */
void Controller::deactivateMod(std::string source, std::string group) {
  this->openSdCardIfNeeded();

  std::string activeMod = this->getActiveMod(source, group);

  std::size_t alchemistPathSize = this->getModPath(source, group, activeMod).size();

  std::string movedFilesListFileName {
    this->getMovedFilesListFileName(std::move(group), std::move(source), std::move(activeMod))
  };
  {
    std::ifstream istrm(movedFilesListFileName);

    std::string alchemistModPath;
    while (std::getline(istrm, alchemistModPath)) {

      std::string_view atmosphereModPath = this->getAtmosphereModPath(alchemistPathSize, alchemistModPath);

      if (std::filesystem::exists(atmosphereModPath)) {
        std::filesystem::rename(std::move(atmosphereModPath), alchemistModPath);
      }
    }
  }

  std::filesystem::remove(std::move(movedFilesListFileName));
}

/**
 * Unmount SD card when destroyed 
 */
Controller::~Controller() {
  if (this->isSdCardOpen) {
    fsFsClose(&this->sdSystem);
  }
}

/*
 * Mounts the SD card if it hasn't been mounted yet
 */
void Controller::openSdCardIfNeeded() {
    if (this->isSdCardOpen) {
        return;
    }

    this->isSdCardOpen = true;
    Result result;
    result = fsOpenSdCardFileSystem(&this->sdSystem);
    if (R_FAILED(result)) {
        std::error_code ec(static_cast<int>(result), std::generic_category());
        throw std::filesystem::filesystem_error("Failed to mount sdmc", std::filesystem::path(), ec);
    }
}

/*
 * Gets Mod Alchemist's game directory:
 */
std::string Controller::getGamePath() {
  std::string path = ALCHEMIST_PATH + std::to_string(this->titleId);

  if (!std::filesystem::exists(path)) {
    std::error_code ec = std::make_error_code(std::errc::no_such_file_or_directory);
    throw std::filesystem::filesystem_error("Game Folder not found", ALCHEMIST_PATH, ec);
  }

  return path;
}

/*
 * Gets the file path for the specified group
 */
std::string Controller::getGroupPath(std::string group) {
  return this->getGamePath() + "/" + std::move(group);
}

/*
 * Gets the file path for the specified source within the group
 */
std::string Controller::getSourcePath(std::string group, std::string source) {
  return this->getGroupPath(std::move(group)) + "/" + std::move(source);
}

/*
 * Get the file path for the specified mod within the moddable source
 */
std::string Controller::getModPath(std::string group, std::string source, std::string mod) {
  return this->getSourcePath(std::move(group), std::move(source)) + "/" + std::move(mod);
}

/**
 * Gets the game's path that's stored within Atmosphere's directory
 */
std::string Controller::getAtmospherePath() {
  return ATMOSPHERE_PATH + std::to_string(this->titleId);
}

/**
 * Builds the path a mod's file should have once we intend to move it into Atmosphere's folder
 * It is built off of its current path within the Mod Alchemist's directory structure.
 */
std::string_view Controller::getAtmosphereModPath(std::size_t alchemistModFolderPathSize, std::string alchemistModFilePath) {
  return this->getAtmospherePath() + "/" + alchemistModFilePath.substr(alchemistModFolderPathSize);
}

/**
 * Gets the file path for the list of moved files for the specified mod
 * 
 * The file should only exist if the mod is currently active
 */
std::string_view Controller::getMovedFilesListFileName(std::string group, std::string source, std::string mod) {
  return this->getSourcePath(std::move(group), std::move(source)) + "/" + std::move(mod) + TXT_EXT;
}