#include "ui/ui_groups.h"
#include "ui/ui_sources.h"
#include "ui/ui_locks.h"

#include "controller.h"
#include "constants.h"

GuiGroups::GuiGroups() {}

tsl::elm::Element* GuiGroups::createUI() {
  auto frame = new tsl::elm::OverlayFrame("State Alchemist", "Mod Groups");

  auto list = new tsl::elm::List();

  std::vector<std::string> groups = controller.loadGroups();

  // When there are no groups for some odd reason:
  if (groups.empty()) {
    frame->setContent(new tsl::elm::CategoryHeader("No groups found"));
    return frame;
  }

  list->addItem(new tsl::elm::CategoryHeader("\uE0E0 View group    |    \uE0E2 View group (for locking)"));

  for (const std::string &group : groups) {
    auto *item = new tsl::elm::ListItem(group);

    item->setClickListener([this, group](u64 keys) {
      if (keys & HidNpadButton_A) {
        controller.group = group;
        tsl::changeTo<GuiSources>();
        return true;
      }

      if (keys & HidNpadButton_X) {
        controller.group = group;
        tsl::changeTo<GuiLocks>();
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
