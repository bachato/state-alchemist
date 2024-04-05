#include <tesla.hpp>    // The Tesla Header

#include <string>
#include <fs_dev.h>
#include <system_error>
#include <filesystem>
#include <fstream>
#include "constants.hpp"


class Controller {
  private:
    bool isSdCardOpen;
    u64 titleId;

  public:
    Controller(u64 titleId) {
      this->titleId = titleId;
    }

    std::vector<std::string> loadGroups() {
      this->openSdCardIfNeeded();

      std::vector<std::string> groups;

      for (auto& node: std::filesystem::directory_iterator(this->getGamePath())) {
        if (node.is_directory()) {
          groups.push_back(node.path().filename().string());
        }
      }

      return groups;
    }

    std::vector<std::string> loadSources(std::string group) {
      this->openSdCardIfNeeded();

      std::vector<std::string> sources;

      for (auto& node: std::filesystem::directory_iterator(this->getGroupPath(group))) {
        if (node.is_directory()) {
          sources.push_back(node.path().filename().string());
        }
      }

      return sources;
    }

    std::vector<std::string> loadMods(std::string source, std::string group) {
      this->openSdCardIfNeeded();

      std::vector<std::string> mods;

      for (auto& node: std::filesystem::directory_iterator(this->getSourcePath(group, source))) {
        if (node.is_directory()) {
          mods.push_back(node.path().filename().string());
        }
      }

      return mods;
    }

    std::string getActiveMod(std::string source, std::string group) {
      this->openSdCardIfNeeded();

      for (auto& node: std::filesystem::directory_iterator(this->getSourcePath(group, source))) {
        if (node.is_regular_file()) {
          std::string filename = node.path().filename().string();
          if (filename.ends_with(MOVED_FILES_LIST_POSTFIX)) {
            return filename.substr(0, filename.size() - MOVED_FILES_LIST_POSTFIX.size());
          }
        }
      }

      return "";
    }

    void activateMod(std::string source, std::string group, std::string mod) {
      this->openSdCardIfNeeded();

      std::string modPath = this->getModPath(group, source, mod);
      std::string movedFilesListFileName = this->getMovedFilesListFileName(group, source, mod);
      {
        std::ofstream ostrm(movedFilesListFileName);

        for (auto& node: std::filesystem::recursive_directory_iterator(modPath)) {
          if (node.is_regular_file()) {
            std::string atmospherePath = this->getAtmosphereModPath(modPath, node.path().string());
            if (!std::filesystem::exists(atmospherePath)) {
              ostrm << node.path().string() << std::endl;
              std::filesystem::rename(node.path(), atmospherePath);
            }
          }
        }
      }
    }

    void deactivateMod(std::string source, std::string group) {
      this->openSdCardIfNeeded();

      std::string activeMod = this->getActiveMod(source, group);
      std::string movedFilesListFileName = this->getMovedFilesListFileName(group, source, activeMod);
      {
        std::ifstream istrm(movedFilesListFileName);

        std::string alchemistModPath;
        while (std::getline(istrm, alchemistModPath)) {
          std::string atmosphereModPath = this->getAtmosphereModPath(this->getModPath(source, group, activeMod), alchemistModPath);
          if (std::filesystem::exists(atmosphereModPath)) {
            std::filesystem::rename(atmosphereModPath, alchemistModPath);
          }
        }
      }
      std::filesystem::remove(movedFilesListFileName);
    }

  private:
    void openSdCardIfNeeded() {
        if (this->isSdCardOpen) {
            return;
        }

        this->isSdCardOpen = true;
        Result result;
        result = fsInitialize();
        if (R_FAILED(result)) {
          std::error_code ec(static_cast<int>(result), std::generic_category());
          throw std::filesystem::filesystem_error("Failed to initialize fs", std::filesystem::path(), ec);
        }
        result = fsdevMountSdmc();
        if (R_FAILED(result)) {
          std::error_code ec(static_cast<int>(result), std::generic_category());
          throw std::filesystem::filesystem_error("Failed to mount sdmc", std::filesystem::path(), ec);
        }
    }

    std::string getGamePath() {
      std::string path = ALCHEMIST_PATH + std::to_string(this->titleId);
      if (std::filesystem::exists(path)) {
        return path;
      }
      std::error_code ec = std::make_error_code(std::errc::no_such_file_or_directory);
      throw std::filesystem::filesystem_error("Game Folder not found", ALCHEMIST_PATH, ec);
    }

    std::string getGroupPath(std::string group) {
      return this->getGamePath() + "/" + group;
    }

    std::string getSourcePath(std::string group, std::string source) {
      return this->getGroupPath(group) + "/" + source;
    }

    std::string getModPath(std::string group, std::string source, std::string mod) {
      return this->getSourcePath(group, source) + "/" + mod;
    }

    std::string getAtmospherePath() {
      return ATMOSPHERE_PATH + std::to_string(this->titleId);
    }

    std::string getAtmosphereModPath(std::string alchemistModFolderPath, std::string alchemistModFilePath) {
      return this->getAtmospherePath() + "/" + alchemistModFilePath.substr(alchemistModFolderPath.size());
    }

    std::string getMovedFilesListFileName(std::string group, std::string source, std::string mod) {
      return this->getSourcePath(group, source) + "/" + mod + MOVED_FILES_LIST_POSTFIX;
    }
};