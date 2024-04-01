#include <tesla.hpp>    // The Tesla Header

#include <string>
#include <fs_dev.h>
#include <system_error>
#include <filesystem>
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
      this->openSdCardIfNeeded();

      std::string path = GAME_FOLDER_PATH + std::to_string(this->titleId);
      if (std::filesystem::exists(path)) {
        return path;
      }
    }
};