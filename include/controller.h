#pragma once

#include <switch.h>

#include <vector>
#include <string>

class Controller {
  public:
    u64 titleId; // The current Game's Title ID
    std::string group;
    std::string source;

    void init();

    /**
     * Gets the title ID in the form of a string
     */
    std::string getTitleIdStr();

    bool doesGameHaveFolder();

    /*
     * Load all groups from the game folder
     */
    std::vector<std::string> loadGroups();

    /*
     * Load all source options within the specified group
     */
    std::vector<std::string> loadSources();

    /*
     * Load all mod options that could be activated for the moddable source in the group
     */
    std::vector<std::string> loadMods();

    /*
     * Gets the mod currently activated for the moddable source in the group
     *
     * Returns an empty string if no mod is active and vanilla files are being used
     */
    std::string_view getActiveMod();

    /*
     * Activates the specified mod, moving all its files into the atmosphere folder for the game
     */
    void activateMod(const std::string& mod);

    /**
     * Deactivates the currently active mod, restoring the moddable source to its vanilla state
     */
    void deactivateMod();

    /**
     * Returns the files of all currently active mods to their location in the mod_alchemy subfolders
     * (disabling them in the game).
     * 
     * Mods will still be marked as "active".
     * 
     * Used when the game is closed.
     */
    void disableAllActiveMods();

    /**
     * Brings the files of all currently active mods to their proper location in the atmosphere folder's subfolders
     * (re-enabling any of them in the game).
     * 
     * Used when the game is started.
     */
    void enableAllActiveMods();

    /**
     * Unmount SD card when destroyed 
     */
    ~Controller();

  private:

    /**
     * Assumes the mod parameter is currently active
     * 
     * Returns the files of the mod from the atmosphere directorey to their original alchemist location
     * (disabling them in the game).
     * Just does the files. Does not "unmark" the mod as being deactivated in the UI.
     */
    void returnModFiles(const std::string& mod);

    /**
     * Assumes the mod parameter is currently active
     * 
     * Moves the mod's files from the alchemist directory to the atmosphere directory
     */
    void bringOutModFiles(const std::string& mod);

    /**
     * Gets Mod Alchemist's game directory:
     */
    std::string getGamePath();

    /**
     * Gets the file path for the specified group
     */
    std::string getGroupPath();

    /**
     * Gets the file path for the specified source within the group
     */
    std::string getSourcePath();

    /**
     * Get the file path for the specified mod within the moddable source
     */
    std::string getModPath(const std::string& mod);

    /**
     * Gets the game's path that's stored within Atmosphere's directory
     */
    std::string getAtmospherePath();

    /**
     * Builds the path a mod's file should have once we intend to move it into Atmosphere's folder
     * It is built off of its current path within the Mod Alchemist's directory structure.
     */
    std::string getAtmosphereModPath(std::size_t alchemistModFolderPathSize, const std::string& alchemistModFilePath);

    /**
     * Gets the file path for the list of moved files for the specified mod
     * 
     * The file should only exist if the mod is currently active
     */
    std::string getMovedFilesListFilePath(const std::string& mod);
};

extern Controller controller;