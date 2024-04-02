#include <tesla.hpp>    // The Tesla Header

#include <dmntcht.h>
#include "ui_error.hpp"
#include "../controller.hpp"

class GuiSources : public tsl::Gui {
  private:
    Controller controller;
    std::string group;

  public:
    GuiSources(Controller controller, std::string group) {
      this->controller = controller;
      this->group = group;
    }

    // Called when this Gui gets loaded to create the UI
    // Allocate all elements on the heap. libtesla will make sure to clean them up when not needed anymore
    virtual tsl::elm::Element* createUI() override {
        // A OverlayFrame is the base element every overlay consists of. This will draw the default Title and Subtitle.
        // If you need more information in the header or want to change it's look, use a HeaderOverlayFrame.
        auto frame = new tsl::elm::OverlayFrame("The Mod Alchemist", this->group);

        try {
          auto groupList = new tsl::elm::List();

          std::vector<std::string> sources = this->controller.loadSources(this->group);
          if (sources.empty()) {
            auto uiMessage = new tsl::elm::CategoryHeader("Group is empty");
            frame->setContent(uiMessage);
            return frame;
          }

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

