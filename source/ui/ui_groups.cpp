#include <tesla.hpp>    // The Tesla Header
#include <dmntcht.h>

#include "ui_sources.cpp"
#include "../controller.h"

class GuiGroups : public tsl::Gui {
  public:
    GuiGroups() {}

    virtual tsl::elm::Element* createUI() override {
      auto frame = new tsl::elm::OverlayFrame("The Mod Alchemist", "Mod Groups");

      // Get the game's title ID:
      DmntCheatProcessMetadata metadata;
      dmntchtGetCheatProcessMetadata(&metadata);

      Controller controller = Controller(metadata.title_id);

      auto groupList = new tsl::elm::List();

      std::vector<std::string> groups = controller.loadGroups();

      // When there are no groups for some odd reason:
      if (groups.empty()) {
        auto uiMessage = new tsl::elm::CategoryHeader("No groups found");
        frame->setContent(uiMessage);
        return frame;
      }

      for (const std::string &group : groups) {
        auto item = new tsl::elm::ListItem(group);

        item->setClickListener([&](u64 keys) {
          if (keys & HidNpadButton_A) {
            tsl::changeTo<GuiSources>(controller, group);
            return true;
          }
          return false;
        });

        groupList->addItem(item);
      }

      frame->setContent(groupList);
      return frame;
    }

    virtual void update() override { }

    virtual bool handleInput(
      u64 keysDown,
      u64 keysHeld,
      const HidTouchState &touchPos,
      HidAnalogStickState joyStickPosLeft,
      HidAnalogStickState joyStickPosRight
    ) override {
      if (keysDown & HidNpadButton_B) {
        tsl::goBack();
        return true;
      }
      return false;
    }
};
