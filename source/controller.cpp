#include "controller.h"

#include "constants.cpp"

#include "ui/ui_error.h"

#include <filesystem>
#include <tesla.hpp>

/**
 * Load all groups from the game folder
 */
std::vector<std::string> Controller::loadGroups() {
  this->openSdCardIfNeeded();

  std::vector<std::string> groups;

  this->validateFolderExistence(this->getGamePath());

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

  this->validateFolderExistence(this->getGroupPath(group));

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

  this->validateFolderExistence(this->getSourcePath(group, source));

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

  this->validateFolderExistence(this->getSourcePath(group, source));

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
  std::string movedFilesFilePath = this->getMovedFilesListFilePath(std::move(group), std::move(source), std::move(mod));

  this->validateFolderExistence(modPath);
  this->validateFolderExistence(movedFilesFilePath);

  std::string movedFilesListFileName { std::move(movedFilesFilePath) };
  {
    std::ofstream ostrm(std::move(movedFilesListFileName));

    bool fileExists = false;

    for (auto& node: std::filesystem::recursive_directory_iterator(modPath)) {
      if (node.is_regular_file()) {
        fileExists = true;
        std::string_view atmospherePath = this->getAtmosphereModPath(modPath.size(), node.path().string());

        if (!std::filesystem::exists(atmospherePath)) {
          ostrm << node.path().string() << std::endl;
          std::filesystem::rename(node.path(), atmospherePath);
        }
      }
    }

    if (!fileExists) {
      tsl::changeTo<GuiError>("Error: Mod folder has no files: " + modPath);
    }
  }
}

/**
 * Deactivates the currently active mod, restoring the moddable source to its vanilla state
 */
void Controller::deactivateMod(std::string source, std::string group) {
  this->openSdCardIfNeeded();

  std::string activeMod = this->getActiveMod(source, group);
  std::string modPath = this->getModPath(group, source, activeMod);

  this->validateFolderExistence(modPath);

  std::string_view movedFilesListPath = this->getMovedFilesListFilePath(std::move(group), std::move(source), std::move(activeMod));

  // If the moved-files doesn't exist, then obviously this mod isn't active, so short-circuit:
  if (!std::filesystem::exists(movedFilesListPath)) {
    return;
  }

  short missingFiles = 0;

  std::string openedMovedFilesListPath { movedFilesListPath };
  {
    std::ifstream istrm(openedMovedFilesListPath);

    std::string alchemistModPath;
    while (std::getline(istrm, alchemistModPath)) {

      std::string atmosphereModPath = this->getAtmosphereModPath(modPath.size(), alchemistModPath);

      if (std::filesystem::exists(atmosphereModPath)) {
        std::filesystem::rename(std::move(atmosphereModPath), alchemistModPath);
      } else {
        missingFiles++;
      }
    }
  }

  if (missingFiles > 0) {
    tsl::changeTo<GuiError>("Error: " + std::to_string(missingFiles) + " files are mysteriously missing. You may want to restore from a backup.");
  }

  std::filesystem::remove(std::move(movedFilesListPath));
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
        tsl::changeTo<GuiError>("Error: Failed to open SD card filesystem.");
    }
}

/*
 * Gets Mod Alchemist's game directory:
 */
std::string Controller::getGamePath() {
  return ALCHEMIST_PATH + std::to_string(this->titleId);
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
std::string Controller::getAtmosphereModPath(std::size_t alchemistModFolderPathSize, std::string alchemistModFilePath) {
  return this->getAtmospherePath() + "/" + alchemistModFilePath.substr(alchemistModFolderPathSize);
}

/**
 * Gets the file path for the list of moved files for the specified mod
 * 
 * The file should only exist if the mod is currently active
 */
std::string Controller::getMovedFilesListFilePath(std::string group, std::string source, std::string mod) {
  return this->getSourcePath(std::move(group), std::move(source)) + "/" + std::move(mod) + TXT_EXT;
}

/**
 * Validates the existence of a path 
 */
void Controller::validateFolderExistence(std::string path) {
  if (std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
    tsl::changeTo<GuiError>("Error: Path does not exist, but it should: " + std::move(path));
  }
}
