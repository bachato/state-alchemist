#include "fs_manager.h"

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
    fsFsOpenDirectory(&sdSystem, toPathBuffer(path), mode, &dir),
    "fsOpenDir"
  );
}

void FsManager::createFolderIfNeeded(const std::string& path) {
  if (doesFolderExist(path)) { return; }

  GuiError::tryResult(
    fsFsCreateDirectory(&sdSystem, toPathBuffer(path)),
    "fsCreateDir"
  );
}

bool FsManager::doesFolderExist(const std::string& path) {
  FsDir dir;

  // If the directory can be opened, it exists:
  bool exists = R_SUCCEEDED(fsFsOpenDirectory(&sdSystem, toPathBuffer(path), FsDirOpenMode_ReadFiles, &dir));
  
  if (exists) {
    fsDirClose(&dir);
  }
  
  return exists;
}

bool FsManager::doesFileExist(const std::string& path) {
  FsFile file;

  // If the file can be opened, it exists:
  bool exists = R_SUCCEEDED(fsFsOpenFile(&sdSystem, toPathBuffer(path), FsOpenMode_Read, &file));

  if (exists) {
    fsFileClose(&file);
  }

  return exists;
}

/**
 * Gets a vector of all folder names that are directly within the specified path
 */
std::vector<std::string> FsManager::listSubfolderNames(const std::string& path) {
  std::vector<std::string> subfolders;

  FsDir dir = openFolder(path, FsDirOpenMode_ReadDirs);

  FsDirectoryEntry entry;
  s64 readCount = 0;
  while (R_SUCCEEDED(fsDirRead(&dir, &readCount, 1, &entry)) && readCount) {
    if (entry.type == FsDirEntryType_Dir) {
      subfolders.push_back(entry.name);
    }
  }

  fsDirClose(&dir);

  return subfolders;
}

/**
 * Records the line parameter in the filePath
 * 
 * offset is expected to be at the end of the file,
 * and it's updated to the new position at the end of file
 */
void FsManager::recordFile(const std::string& line, const std::string& filePath, s64& offset) {
 
  // If the file hasn't been created yet, create it:
  if (!doesFileExist(filePath)) {
    GuiError::tryResult(
      fsFsCreateFile(&sdSystem, toPathBuffer(filePath), 0, 0),
      "fsCreateMoved"
    );
  }
  
  // Open the file:
  FsFile movedListFile;
  GuiError::tryResult(
    fsFsOpenFile(
      &sdSystem,
      toPathBuffer(filePath),
      FsOpenMode_Write | FsOpenMode_Append,
      &movedListFile
    ),
    "fsWriteMoved"
  );

  // Write the path to the end of the list:
  GuiError::tryResult(
    fsFileWrite(&movedListFile, offset, line.c_str(), line.size(), FsWriteOption_Flush),
    "fsWritePath"
  );
  fsFileClose(&movedListFile);

  // Update the offset to the end of the file:
  offset += line.size();
}

/**
 * Changes the fromPath file parameter's location to what's specified as the toPath parameter
 */
void FsManager::moveFile(const std::string& fromPath, const std::string& toPath) {
  GuiError::tryResult(
    fsFsRenameFile(&sdSystem, toPathBuffer(fromPath), toPathBuffer(toPath)),
    "fsMoveFile"
  );
}

/**
 * Formats a string as a char array that will work properly as a parameter for libnx's filesystem functions
 */
char* FsManager::toPathBuffer(const std::string& path) {
  char* buffer = new char[FS_MAX_PATH];
  std::strcpy(buffer, path.c_str());
  return buffer;
}