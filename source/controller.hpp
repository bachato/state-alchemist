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

    tsl::elm::List* loadGroups() {
      this->openSdCardIfNeeded();

      auto list = new tsl::elm::List();

      // throws std::filesystem::filesystem_error
      for (auto& node: std::filesystem::directory_iterator(this->getGamePath())) {
        if (node.is_directory()) {
          list->addItem(new tsl::elm::ListItem(node.path().filename().string()));
        }
      }

      return list;
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
            // Throw error
        }
        result = fsdevMountSdmc();
        if (R_FAILED(result)) {
            // Throw error
        }
    }

    std::string getGamePath() {
      this->openSdCardIfNeeded();

      std::string path = GAME_FOLDER_PATH + std::to_string(this->titleId);
      if (std::filesystem::exists(path)) {
        return path;
      }
      // throws std::filesystem::filesystem_erro
    }
};