#include "ui/ui_groups.h"
#include "ui/ui_sources.h"
#include "controller.h"
#include "constants.h"

GuiGroups::GuiGroups() { }

tsl::elm::Element* GuiGroups::createUI() {
  auto frame = new tsl::elm::OverlayFrame("The Mod Alchemist", "Mod Groups");

  auto list = new tsl::elm::List();

  controller.init();
  
  if (!controller.doesGameHaveFolder()) {
    list->addItem(new tsl::elm::ListItem("The running game has no folder."));
    list->addItem(new tsl::elm::ListItem("It should be named \"" + controller.getTitleIdStr() + "\""));
    list->addItem(new tsl::elm::ListItem("And located in the \"" + ALCHEMIST_PATH + "\" directory."));
    frame->setContent(list);
    return frame;
  }

  std::vector<std::string> groups = controller.loadGroups();

  // When there are no groups for some odd reason:
  if (groups.empty()) {
    frame->setContent(new tsl::elm::CategoryHeader("No groups found"));
    return frame;
  }

  for (const std::string &group : groups) {
    auto *item = new tsl::elm::ListItem(group);

    item->setClickListener([group](u64 keys) {
      if (keys & HidNpadButton_A) {
        controller.group = group;
        tsl::changeTo<GuiSources>();
        return true;
      }
      return false;
    });

    list->addItem(item);
  }
  
  frame->setContent(list);
  return frame;
}

bool GuiGroups::handleInput(
  u64 keysDown,
  u64 keysHeld,
  const HidTouchState &touchPos,
  HidAnalogStickState joyStickPosLeft,
  HidAnalogStickState joyStickPosRight
) {
  if (keysDown & HidNpadButton_B) {
    tsl::goBack();
    return true;
  }
  return false;
}
