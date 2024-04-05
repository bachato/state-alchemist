#include <tesla.hpp>    // The Tesla Header

#include <dmntcht.h>
#include "ui_error.hpp"
#include "ui_mods.hpp"
#include "../controller.hpp"

/**
 * UI for displaying the moddable sources within each group
 */
class GuiSources : public tsl::Gui {
  private:
    Controller controller;
    std::string group;

  public:
    GuiSources(Controller& controller_, std::string& group_)
      : controller(controller_), group(group_) { }

    virtual tsl::elm::Element* createUI() override {
        auto frame = new tsl::elm::OverlayFrame("The Mod Alchemist", this->group);

        try {
          auto groupList = new tsl::elm::List();

          std::vector<std::string> sources = this->controller.loadSources(this->group);

          // For when the group is empty for some reason:
          if (sources.empty()) {
            auto uiMessage = new tsl::elm::CategoryHeader("Group is empty");
            frame->setContent(uiMessage);
            return frame;
          }

          // List all of the group's sources:
          for (const std::string source : sources) {
            auto item = new tsl::elm::ListItem(source);

            item->setClickListener([&](u64 keys) {
              if (keys & HidNpadButton_A) {
                tsl::changeTo<GuiMods>(controller, source, group);
                return true;
              }
              return false;
            });

            groupList->addItem(item);
          }

          frame->setContent(groupList);
          return frame;
        } catch (std::filesystem::filesystem_error &e) {
          tsl::changeTo<GuiError>(std::string("Filesystem error: ") + e.what());
        } catch (std::exception &e) {
          tsl::changeTo<GuiError>(e.what());
        } catch (...) {
          tsl::changeTo<GuiError>("Unknown error");
        }
    }

    // Called once every frame to update values
    virtual void update() override {

    }

    // Called once every frame to handle inputs not handled by other UI elements
    virtual bool handleInput(u64 keysDown, u64 keysHeld, const HidTouchState &touchPos, HidAnalogStickState joyStickPosLeft, HidAnalogStickState joyStickPosRight) override {
        if (keysDown & HidNpadButton_B) {
            tsl::goBack();
            return true;
        }
        return false;
    }
};

