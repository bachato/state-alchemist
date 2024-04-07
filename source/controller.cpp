#include "controller.h"


class Controller {
  private:
    FsFileSystem sdSystem;
    u64 titleId; // The current Game's Title ID
    bool isSdCardOpen; // Whether the SD card has been mounted or not

  public:
    Controller(u64 titleId) {
      this->titleId = titleId;
    }

    /*
     * Load all groups from the game folder
     */
    std::vector<std::string> loadGroups() {
      this->openSdCardIfNeeded();

      std::vector<std::string> groups;

      // Each folder within the game's folder is a group
      for (auto& node: std::filesystem::directory_iterator(this->getGamePath())) {
        if (node.is_directory()) {
          groups.push_back(node.path().filename().string());
        }
      }

      return groups;
    }

    /*
     * Load all source options within the specified group
     */
    std::vector<std::string> loadSources(std::string group) {
      this->openSdCardIfNeeded();

      std::vector<std::string> sources;

      // Each folder within the group's folder is an option for a moddable source
      for (auto& node: std::filesystem::directory_iterator(this->getGroupPath(std::move(group)))) {
        // "group" has been moved now. "group" may be empty.
        if (node.is_directory()) {
          sources.push_back(node.path().filename().string());
        }
      }

      return sources;
    }

    /*
     * Load all mod options that could be activated for the moddable source in the group
     */
    std::vector<std::string> loadMods(std::string source, std::string group) {
      this->openSdCardIfNeeded();

      std::vector<std::string> mods;

      // Each folder within the moddable source's folder is a toggable mod
      for (auto& node: std::filesystem::directory_iterator(this->getSourcePath(std::move(group), std::move(source)))) {
        // "group" and "source" have been moved now. "group" and "source" may be empty.
        if (node.is_directory()) {
          mods.push_back(node.path().filename().string());
        }
      }

      return mods;
    }

    /*
     * Gets the mod currently activated for the moddable source in the group
     *
     * Returns an empty string if no mod is active and vanilla files are being used
     */
    std::string getActiveMod(std::string source, std::string group) {
      this->openSdCardIfNeeded();

      // The mod that currently has the moved-files-list file is the active mod:
      for (auto& node: std::filesystem::directory_iterator(this->getSourcePath(std::move(group), std::move(source)))) {
        // "group" and "source" have been moved now. "group" and "source" may be empty.
        if (node.is_regular_file()) {
          std::string filename = node.path().filename().string();

          // The active mod's name will be the name of the txt file in the source's folder:
          if (filename.ends_with(TXT_EXT)) {
            return filename.substr(0, filename.size() - TXT_EXT.size());
          }
        }
      }

      // No active mod
      return "";
    }

    /*
     * Activates the specified mod, moving all its files into the atmosphere folder for the game
     */
    void activateMod(std::string source, std::string group, std::string mod) {
      this->openSdCardIfNeeded();

      std::string modPath = this->getModPath(group, source, mod);
      std::string movedFilesListFileName { this->getMovedFilesListFileName(std::move(group), std::move(source), std::move(mod)) };
      {
        // "source", "group", and "mod" have been moved now. "source", "group", and "mod" may be empty.
        std::ofstream ostrm(std::move(movedFilesListFileName));
        // "movedFilesListFileName" has been moved now. "movedFilesListFileName" may be empty.

        // For each of the mod's files...
        for (auto& node: std::filesystem::recursive_directory_iterator(modPath)) {
          if (node.is_regular_file()) {

            // Get its new path:
            std::string_view atmospherePath = this->getAtmosphereModPath(modPath.size(), node.path().string());

            // Skip the file if there's a conflict:
            if (!std::filesystem::exists(atmospherePath)) {
              // Otherwise, record the original file path so we can move it back:
              ostrm << node.path().string() << std::endl;
              // Then move it:
              std::filesystem::rename(node.path(), atmospherePath);
            }
          }
        }
      }
    }

    /**
     * Deactivates the currently active mod, restoring the moddable source to its vanilla state
     */
    void deactivateMod(std::string source, std::string group) {
      this->openSdCardIfNeeded();

      std::string activeMod = this->getActiveMod(source, group);

      // The number of characters in the path to where the modded file's path begins:
      std::size_t alchemistPathSize = this->getModPath(source, group, activeMod).size();

      std::string movedFilesListFileName {
        this->getMovedFilesListFileName(std::move(group), std::move(source), std::move(activeMod))
      };
      {
        // "source", "group" and "activeMod" have been moved now. "source", "group" and "activeMod" may be empty.
        std::ifstream istrm(movedFilesListFileName);

        std::string alchemistModPath;
        // Get each file to bring back from Atmosphere's mods folder:
        while (std::getline(istrm, alchemistModPath)) {

          std::string_view atmosphereModPath = this->getAtmosphereModPath(alchemistPathSize, alchemistModPath);

          // Then bring it back:
          if (std::filesystem::exists(atmosphereModPath)) {
            std::filesystem::rename(std::move(atmosphereModPath), alchemistModPath);
          }
        }
      }

      // Delete the files list when done:
      std::filesystem::remove(std::move(movedFilesListFileName));
    }

    /**
     * Unmount SD card when destroyed 
     */
    ~Controller() {
      if (this->isSdCardOpen) {
        fsFsClose(&this->sdSystem);
      }
    }

  private:

    /**
     * Mounts the SD card if it hasn't been mounted yet
     * 
     * We need to be sure this always runs before attempting to perform any file operations 
     */
    void openSdCardIfNeeded() {
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

    /**
     * Gets Mod Alchemist's game directory:
     */
    std::string getGamePath() {
      std::string path = ALCHEMIST_PATH + std::to_string(this->titleId);
      if (std::filesystem::exists(path)) {
        return std::move(path);
      }
      std::error_code ec = std::make_error_code(std::errc::no_such_file_or_directory);
      throw std::filesystem::filesystem_error("Game Folder not found", ALCHEMIST_PATH, ec);
    }

    /**
     * Gets the file path for the specified group
     */
    std::string getGroupPath(std::string group) {
      return this->getGamePath() + "/" + std::move(group);
    }

    /**
     * Gets the file path for the specified source within the group
     */
    std::string getSourcePath(std::string group, std::string source) {
      return this->getGroupPath(std::move(group)) + "/" + std::move(source);
    }

    /**
     * Get the file path for the specified mod within the moddable source
     */
    std::string getModPath(std::string group, std::string source, std::string mod) {
      return this->getSourcePath(std::move(group), std::move(source)) + "/" + std::move(mod);
    }

    /**
     * Gets the game's path that's stored within Atmosphere's directory
     */
    std::string getAtmospherePath() {
      return ATMOSPHERE_PATH + std::to_string(this->titleId);
    }

    /**
     * Builds the path a mod's file should have once we intend to move it into Atmosphere's folder
     * It is built off of its current path within the Mod Alchemist's directory structure.
     */
    std::string_view getAtmosphereModPath(std::size_t alchemistModFolderPathSize, std::string alchemistModFilePath) {
      return this->getAtmospherePath() + "/" + alchemistModFilePath.substr(alchemistModFolderPathSize);
    }

    /**
     * Gets the file path for the list of moved files for the specified mod
     * 
     * The file should only exist if the mod is currently active
     */
    std::string_view getMovedFilesListFileName(std::string group, std::string source, std::string mod) {
      return this->getSourcePath(std::move(group), std::move(source)) + "/" + std::move(mod) + TXT_EXT;
    }
};