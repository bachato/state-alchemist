#include "ui/ui_groups.h"
#include "ui/ui_sources.h"
#include "controller.h"

#include "../constants.cpp"

GuiGroups::GuiGroups() { }

tsl::elm::Element* GuiGroups::createUI() {   // Remove the virtual keyword
  auto frame = new tsl::elm::OverlayFrame("The Mod Alchemist", "Mod Groups");

  auto groupList = new tsl::elm::List();

  Controller controller = Controller();

  if (!controller.doesGameHaveFolder()) {
    frame->setContent(new tsl::elm::CategoryHeader(
      "The running game has no folder. It should be named \"" + std::to_string(controller.titleId) +
      "\" in the \"" + ALCHEMIST_PATH + "\" directory."
    ));
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
