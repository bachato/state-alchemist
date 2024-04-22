#include "controller.h"

#include "constants.cpp"

#include "ui/ui_error.h"

#include <tesla.hpp>

/**
 * Load all groups from the game folder
 */
std::vector<std::string_view> Controller::loadGroups() {
  this->openSdCardIfNeeded();
  return this->listSubfolderNames(this->getGamePath());
}

/**
 * Load all source options within the specified group
 */
std::vector<std::string_view> Controller::loadSources(std::string group) {
  this->openSdCardIfNeeded();
  return this->listSubfolderNames(this->getGroupPath(std::move(group)));
}

/**
 * Load all mod options that could be activated for the moddable source in the group
 */
std::vector<std::string_view> Controller::loadMods(std::string source, std::string group) {
  this->openSdCardIfNeeded();
  return this->listSubfolderNames(this->getSourcePath(std::move(group), std::move(source)));
}

/**
 * Gets the mod currently activated for the moddable source in the group
 *
 * Returns an empty string if no mod is active and vanilla files are being used
 */
std::string_view Controller::getActiveMod(std::string source, std::string group) {
  this->openSdCardIfNeeded();

  // Open to the correct source directory
  FsDir sourceDir = this->openDirectory(this->getSourcePath(std::move(group), std::move(source)), FsDirOpenMode_ReadFiles);

  s64 entryCount;
  fsDirGetEntryCount(&sourceDir, &entryCount);

  // Find the .txt file in the directory. The name would be the active mod:
  FsDirectoryEntry entry;
  s64 readCount;
  std::string_view activeMod = "";
  while (R_SUCCEEDED(fsDirRead(&sourceDir, &readCount, entryCount, &entry))) {
    if (entry.type == FsDirEntryType_File) {
      std::string_view name(entry.name);
      if (name.find(TXT_EXT) != std::string::npos) {
        activeMod = name.substr(0, name.size() - TXT_EXT.size());
        break;
      }
    }
  }
  fsDirClose(&sourceDir);

  return activeMod;
}

/**
 * Activates the specified mod, moving all its files into the atmosphere folder for the game
 * 
 * Make sure to deactivate any existing active mod for this source if there is one
 */
void Controller::activateMod(std::string source, std::string group, std::string mod) {
  this->openSdCardIfNeeded();

  // Path to the "mod" folder in alchemy's directory:
  std::string modPath = this->getModPath(group, source, mod);
  // Path of the txt file for the active mod:
  std::string movedFilesFilePath = this->getMovedFilesListFilePath(std::move(group), std::move(source), std::move(mod));

  FsDir dir = this->openDirectory(modPath, FsDirOpenMode_ReadDirs | FsDirOpenMode_ReadFiles);

  // Used for "storing" data of parent folders when traversing deeper into the hierarchy:
  std::vector<u64> entryCounts;
  std::vector<u64> readCounts;

  // The directory we are currently at:
  std::string currentDirectory = modPath;

  // Position in the txt file where we should write the next file path:
  s64 txtOffset = 0;

  // Counts for entries of currentDirectory:
  s64 currentReadCount = 0;
  s64 currentEntryCount;
  fsDirGetEntryCount(&dir, &currentEntryCount);

  // Set to true once we have traversed the entire folder directory within the mod folder:
  bool isDone = false;
  while (!isDone) {

    // If we have analyzed all entries in the current directory, navigate back up to where we left off in the parent:
    if (currentEntryCount == currentReadCount) {

      // If there's nothing left in our count storage, we've navigated everything, so we're done:
      if (entryCounts.size() == 0) {
        isDone = true;
        break;
      } else {
        // Otherwise, let's get back the count data of where we left off in the parent:
        currentEntryCount = entryCounts.back();
        currentReadCount = readCounts.back();
        entryCounts.pop_back();
        readCounts.pop_back();

        // Remove the string portion after the last '/' to get the parent's path:
        std::size_t lastSlashIndex = currentDirectory.rfind('/');
        currentDirectory = currentDirectory.substr(0, lastSlashIndex);
        this->changeDirectory(dir, currentDirectory, FsDirOpenMode_ReadDirs | FsDirOpenMode_ReadFiles);
      }
    }

    FsDirectoryEntry entry;

    if (R_SUCCEEDED(fsDirRead(&dir, &currentReadCount, currentEntryCount, &entry))) {

      // The entry path we will look at next:
      std::string nextPath = currentDirectory + "/" + entry.name;

      // If the next entry is a file, we will move it and record it as moved as long as there isn't a conflict:
      if (entry.type == FsDirEntryType_File) {
        std::string atmospherePath = this->getAtmosphereModPath(modPath.size(), nextPath);

        if (this->doesFileNotExist(atmospherePath)) {
          this->recordFile(atmospherePath, movedFilesFilePath, txtOffset);
          this->moveFile(nextPath, atmospherePath);
        }
      // If the next entry is a folder, we will traverse within it:
      } else if (entry.type == FsDirEntryType_Dir) {

        // Add the current counts to the storage:
        entryCounts.push_back(currentEntryCount);
        readCounts.push_back(currentReadCount);

        currentDirectory = nextPath;
        this->changeDirectory(dir, currentDirectory, FsDirOpenMode_ReadDirs | FsDirOpenMode_ReadFiles);

        // Have the counts ready for the next directory:
        fsDirGetEntryCount(&dir, &currentEntryCount);
        currentReadCount = 0;
      }
    }
  }

  fsDirClose(&dir);
}

/**
 * Deactivates the currently active mod, restoring the moddable source to its vanilla state
 */
void Controller::deactivateMod(std::string source, std::string group) {
  this->openSdCardIfNeeded();

  std::string activeMod(this->getActiveMod(source, group));

  // Path of the txt file for the active mod:
  std::string movedFilesListPath = this->getMovedFilesListFilePath(group, source, activeMod);

  // Try to open the active mod's txt file to get the list of files that were moved to atmosphere's folder:
  FsFile movedFilesList;
  if (R_FAILED(fsFsOpenFile(&this->sdSystem, movedFilesListPath.c_str(), FsOpenMode_Read, &movedFilesList))) {
    tsl::changeTo<GuiError>("Error: Failed to open file at " + movedFilesListPath);
  }

  s64 fileSize;
  if (R_FAILED(fsFileGetSize(&movedFilesList, &fileSize))) {
    tsl::changeTo<GuiError>("Error: Failed to get file size at " + movedFilesListPath);
  }

  // Used to stream the file a small number of bytes at a time to minimize memory consumption:
  s64 offset = 0;
  char* buffer = new char[FILE_LIST_BUFFER_SIZE];
  std::string pathBuilder = "";

  // As long as there is still data in the file:
  while (offset < fileSize) {

    // Read some of the text into our buffer:
    if (R_FAILED(fsFileRead(&movedFilesList, offset, buffer, FILE_LIST_BUFFER_SIZE, FsReadOption_None, nullptr))) {
      tsl::changeTo<GuiError>("Error: Failed to read file at " + movedFilesListPath);
    }

    // Append it to the string we're using to build the next path:
    pathBuilder += std::string_view(buffer, FILE_LIST_BUFFER_SIZE);

    // If the path builder got a new line character from the buffer, we have a full path:
    std::size_t newLinePos = pathBuilder.find('\n');
    if (newLinePos != std::string::npos) {
      // Trim the new line and any characters that were gathered after it to get the cleaned atmosphere file path:
      std::string atmoPath = pathBuilder.substr(0, newLinePos);

      // Move any characters gathered after the new line to the pathBuilder string for the next path:
      pathBuilder = pathBuilder.substr(newLinePos + 1);

      // The file's original location can be built by replacing the atmosphere portion of the path with alchemy's portion:
      std::string originalPath = this->getModPath(group, source, activeMod) + atmoPath.substr(this->getAtmospherePath().size());

      this->moveFile(atmoPath, originalPath);
    }

    offset += FILE_LIST_BUFFER_SIZE;
  }

  // Once all the files have been returned, delete the txt list:
  fsFsDeleteFile(&this->sdSystem, movedFilesListPath.c_str());

  fsFileClose(&movedFilesList);
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

/**
 * Creates a new open FsDir object for the specified path
 * 
 * Don't forget to close when done
 */
FsDir Controller::openDirectory(std::string path, u32 mode) {
  FsDir dir;
  this->changeDirectory(dir, std::move(path), mode);
  return dir;
}

/**
 * Changes an FsDir instance to the specified path
 */
void Controller::changeDirectory(FsDir& dir, std::string path, u32 mode) {
  if (R_FAILED(fsFsOpenDirectory(&this->sdSystem, path.c_str(), mode, &dir))) {
    tsl::changeTo<GuiError>("Error: Failed to open directory at " + path);
  }
}

bool Controller::doesFileExist(std::string path) {
  FsDir dir;
  // If the file can be opened, it exists:
  bool exists = R_SUCCEEDED(fsFsOpenDirectory(&this->sdSystem, path.c_str(), FsDirOpenMode_ReadFiles, &dir));
  fsDirClose(&dir);
  return exists;
}

bool Controller::doesFileNotExist(std::string path) {
  return !this->doesFileExist(path);
}

/**
 * Gets a vector of all folder names that are directly within the specified path
 */
std::vector<std::string_view> Controller::listSubfolderNames(std::string path) {
  std::vector<std::string_view> subfolders;

  FsDir dir = this->openDirectory(std::move(path), FsDirOpenMode_ReadDirs);

  s64 entryCount;
  fsDirGetEntryCount(&dir, &entryCount);

  FsDirectoryEntry entry;
  s64 readCount;
  while (R_SUCCEEDED(fsDirRead(&dir, &readCount, entryCount, &entry))) {
    if (entry.type == FsDirEntryType_Dir) {
      subfolders.push_back(entry.name);
    }
  }

  fsDirClose(&dir);

  return subfolders;
}

/**
 * Records the atmospherePath parameter for a file being moved in modPath
 * Data is written to the movedFilesListPath file
 * 
 * offset is expected to be at the end of the file,
 * and it's updated to the new position at the end of file
 */
void Controller::recordFile(std::string atmospherePath, std::string movedFilesListPath, s64& offset) {

  // If the file hasn't been created yet, create it:
  if (this->doesFileNotExist(movedFilesListPath)) {
    if (R_FAILED(fsFsCreateFile(&this->sdSystem, movedFilesListPath.c_str(), 0, 0))) {
      tsl::changeTo<GuiError>("Error: Failed to create file at " + movedFilesListPath);
    }
  }
  
  // Open the file:
  FsFile movedListFile;
  if (R_FAILED(fsFsOpenFile(&this->sdSystem, movedFilesListPath.c_str(), FsOpenMode_Write, &movedListFile))) {
    tsl::changeTo<GuiError>("Error: Failed to open file at " + movedFilesListPath);
  }

  // Paths we record are delimited by new lines:
  std::string line = std::move(atmospherePath) + "\n";

  // Write the path to the end of the list:
  if (R_FAILED(fsFileWrite(&movedListFile, offset, line.c_str(), line.size(), FsWriteOption_Flush))) {
    tsl::changeTo<GuiError>("Error: Failed to write to file at " + movedFilesListPath);
  }

  // Update the offset to the end of the file:
  offset += line.size();

  fsFileClose(&movedListFile);
}

/**
 * Changes the fromPath file parameter's location to what's specified as the toPath parameter
 */
void Controller::moveFile(std::string fromPath, std::string toPath) {
  if (R_FAILED(fsFsRenameFile(&this->sdSystem, fromPath.c_str(), toPath.c_str()))) {
    tsl::changeTo<GuiError>("Error: Failed to move file from " + fromPath + " to " + toPath);
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
