#pragma once

#include <switch.h>

#include <vector>
#include <string>
#include <fstream>

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
    std::vector<std::string_view> loadGroups();

    /*
     * Load all source options within the specified group
     */
    std::vector<std::string_view> loadSources(std::string group);

    /*
     * Load all mod options that could be activated for the moddable source in the group
     */
    std::vector<std::string_view> loadMods(std::string source, std::string group);

    /*
     * Gets the mod currently activated for the moddable source in the group
     *
     * Returns an empty string if no mod is active and vanilla files are being used
     */
    std::string_view getActiveMod(std::string source, std::string group);

    /*
     * Activates the specified mod, moving all its files into the atmosphere folder for the game
     */
    void activateMod(std::string source, std::string group, std::string mod);

    /**
     * Deactivates the currently active mod, restoring the moddable source to its vanilla state
     */
    void deactivateMod(std::string source, std::string group);

    /**
     * Unmount SD card when destroyed 
     */
    ~Controller();

  private:

    /**
     * Mounts the SD card if it hasn't been mounted yet
     * 
     * We need to be sure this always runs before attempting to perform any file operations 
     */
    void openSdCardIfNeeded();

    /**
     * Creates a new open FsDir object for the specified path
     * 
     * Don't forget to close when done
     */
    FsDir openDirectory(std::string path, u32 mode);

    /**
     * Changes an FsDir instance to the specified path
     */
    void changeDirectory(FsDir& dir, std::string path, u32 mode);

    bool Controller::doesFileExist(std::string path);
    bool Controller::doesFileNotExist(std::string path);

    /**
     * Gets a vector of all folder names that are directly within the specified path
     */
    std::vector<std::string_view> listSubfolderNames(std::string path);

    /**
     * Records the atmospherePath parameter for a file being moved in modPath
     * Data is written to the movedFilesListPath file
     * 
     * offset is expected to be at the end of the file,
     * and it's updated to the new position at the end of file
     */
    void recordFile(std::string atmospherePath, std::string movedFilesListPath, s64& offset);

    /**
     * Changes the fromPath file parameter's location to what's specified as the toPath parameter
     */
    void moveFile(std::string fromPath, std::string toPath);

    /**
     * Gets Mod Alchemist's game directory:
     */
    std::string getGamePath();

    /**
     * Gets the file path for the specified group
     */
    std::string getGroupPath(std::string group);

    /**
     * Gets the file path for the specified source within the group
     */
    std::string getSourcePath(std::string group, std::string source);

    /**
     * Get the file path for the specified mod within the moddable source
     */
    std::string getModPath(std::string group, std::string source, std::string mod);

    /**
     * Gets the game's path that's stored within Atmosphere's directory
     */
    std::string getAtmospherePath();

    /**
     * Builds the path a mod's file should have once we intend to move it into Atmosphere's folder
     * It is built off of its current path within the Mod Alchemist's directory structure.
     */
    std::string getAtmosphereModPath(std::size_t alchemistModFolderPathSize, std::string alchemistModFilePath);

    /**
     * Gets the file path for the list of moved files for the specified mod
     * 
     * The file should only exist if the mod is currently active
     */
    std::string getMovedFilesListFilePath(std::string group, std::string source, std::string mod);
};
