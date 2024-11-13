#include "fs_manager.h"
#include "meta_manager.h"
#include "ui/ui_error.h"

FsFileSystem FsManager::sdSystem;

/**
 * Creates a new open FsDir object for the specified path
 * 
 * Don't forget to close when done
 */
FsDir FsManager::openFolder(const std::string& path, const u32& mode) {
  FsDir dir;
  changeFolder(dir, path, mode);
  return dir;
}

/**
 * Changes an FsDir instance to the specified path
 */
void FsManager::changeFolder(FsDir& dir, const std::string& path, const u32& mode) {
  fsDirClose(&dir);

  GuiError::tryResult(
    fsFsOpenDirectory(&sdSystem, toPathBuffer(path).get(), mode, &dir),
    "fsOpenDir"
  );
}

void FsManager::createFolderIfNeeded(const std::string& path) {
  if (doesFolderExist(path)) { return; }

  GuiError::tryResult(
    fsFsCreateDirectory(&sdSystem, toPathBuffer(path).get()),
    "fsCreateDir"
  );
}

bool FsManager::doesFolderExist(const std::string& path) {
  FsDir dir;
  Result result = fsFsOpenDirectory(
    &sdSystem,
    toPathBuffer(path).get(),
    FsOpenMode_Read,
    &dir
  );

  if (R_SUCCEEDED(result)) {
    fsDirClose(&dir);
    return true; // File exists
  } else if (result == 0x202) {
    return false; // File does not exist
  } else {
    GuiError::tryResult(result, "check if directory exists"); // Handle other exceptions
    return false; // This line will never be reached, but added for completeness
  }
}

bool FsManager::doesFileExist(const std::string& path) {
  FsFile file;
  Result result = fsFsOpenFile(
    &sdSystem,
    toPathBuffer(path).get(),
    FsOpenMode_Read,
    &file
  );

  if (R_SUCCEEDED(result)) {
    fsFileClose(&file);
    return true; // File exists
  } else if (result == 0x202) {
    return false; // File does not exist
  } else {
    GuiError::tryResult(result, "check if file exists"); // Handle other exceptions
    return false; // This line will never be reached, but added for completeness
  }
}

/**
 * Gets a vector of all entity names that are directly within the specified path
 * (parsing the name from the folder name)
 */
std::vector<std::string> FsManager::listNames(const std::string& path) {
  std::vector<std::string> names;

  FsDir dir = FsManager::openFolder(path, FsDirOpenMode_ReadDirs);

  FsDirectoryEntry entry;
  s64 readCount = 0;
  while (R_SUCCEEDED(fsDirRead(&dir, &readCount, 1, &entry)) && readCount) {
    if (entry.type == FsDirEntryType_Dir) {
      names.push_back(MetaManager::parseName(entry.name));
    }
  }

  fsDirClose(&dir);

  std::sort(names.begin(), names.end());

  return names;
}

/**
 * Gets the name of the folder that currently exists with the name of the specified entity
 */
std::string FsManager::getFolderName(const std::string& path, const std::string& name) {
  std::string folderName;
  
  FsDir dir = FsManager::openFolder(path, FsDirOpenMode_ReadDirs);

  FsDirectoryEntry entry;
  s64 readCount = 0;
  while (R_SUCCEEDED(fsDirRead(&dir, &readCount, 1, &entry)) && readCount) {
    if (entry.type == FsDirEntryType_Dir && MetaManager::namesMatch(entry.name, name)) {
      folderName = entry.name;
      break;
    }
  }

  fsDirClose(&dir);

  return folderName;
}

/**
 * Opens a file at the path (creating it if it doesn't exist)
 */
FsFile FsManager::initFile(const std::string& path) {

  // If the file hasn't been created yet, create it:
  if (!doesFileExist(path)) {
    GuiError::tryResult(
      fsFsCreateFile(&sdSystem, toPathBuffer(path).get(), 0, 0),
      "fsCreateMoved"
    );
  }

  // Open the file:
  FsFile file;
  GuiError::tryResult(
    fsFsOpenFile(
      &sdSystem,
      toPathBuffer(path).get(),
      FsOpenMode_Write | FsOpenMode_Append,
      &file
    ),
    "fsWriteMoved"
  );

  return file;
}

/**
 * Records the text parameter in the filePath, appending it to the FsFile
 * 
 * offset is expected to be at the end of the file,
 * and it's updated to the new position at the end of file
 */
void FsManager::write(FsFile& file, const std::string& text, s64& offset) {

  // Write the path to the end of the list:
  GuiError::tryResult(
    fsFileWrite(&file, offset, text.c_str(), text.size(), FsWriteOption_Flush),
    "fsWritePath"
  );

  // Update the offset to the end of the file:
  offset += text.size();
}

/**
 * Changes the fromPath file parameter's location to what's specified as the toPath parameter
 */
void FsManager::moveFile(const std::string& fromPath, const std::string& toPath) {
  GuiError::tryResult(
    fsFsRenameFile(&sdSystem, toPathBuffer(fromPath).get(), toPathBuffer(toPath).get()),
    "fsMoveFile"
  );
}

/**
 * Formats a string as a char array that will work properly as a parameter for libnx's filesystem functions
 * 
 * Use `get()` when passing it to a libnx function
 */
std::unique_ptr<char[]> FsManager::toPathBuffer(const std::string& path) {
  // Allocate memory for the char array with a fixed size
  std::unique_ptr<char[]> pathBuffer(new char[FS_MAX_PATH]);

  // Ensure the input fits within FS_MAX_PATH
  if (path.length() >= FS_MAX_PATH) {
    tsl::changeTo<GuiError>("Input path exceeds maximum allowed length");
  }

  // Copy the input string into the buffer
  std::strcpy(pathBuffer.get(), path.c_str());

  // Return the unique_ptr which will handle garbage collection automatically
  return pathBuffer;
}