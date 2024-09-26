#include "ui/ui_locks.h"

#include <string>

#include "controller.h"

GuiLocks::GuiLocks() {}

tsl::elm::Element* GuiLocks::createUI() {
  auto frame = new tsl::elm::OverlayFrame("State Alchemist", controller.group);

  auto list = new tsl::elm::List();

  list->addItem(new tsl::elm::CategoryHeader("Locking Mods"));
  list->addItem(new tsl::elm::CategoryHeader("This is for the \"Pick at Random\" option"));
  list->addItem(new tsl::elm::CategoryHeader("Prevents the mod from changing"));

  std::map<std::string, bool> sources = controller.loadSourceLocks();

  // For when the group is empty for some reason:
  if (sources.empty()) {
    frame->setContent(new tsl::elm::CategoryHeader("Group is empty"));
    return frame;
  }

  // List all the group's source with active mods for locking/unlocking:
  for (const auto& [name, locked]: sources) {
    std::string activeMod = controller.getActiveMod(name);

    std::string label;
    if (activeMod.empty()) {
      label = name + " - no mod active";
    } else {
      label = name + " (" + activeMod + ")";
    }

    auto *item = new tsl::elm::ToggleListItem(label, locked);
    item->setClickListener([name, locked](u64 keys) {
      if (keys & HidNpadButton_A) {
        if (locked) {
          controller.unlockSource(name);
        } else {
          controller.lockSource(name);
        }
        return true;
      }
      return false;
    });

    list->addItem(item);
  }

  frame->setContent(list);
  return frame;
}

bool GuiLocks::handleInput(
  u64 keysDown,
  u64 keysHeld,
  const HidTouchState &touchPos,
  HidAnalogStickState joyStickPosLeft,
  HidAnalogStickState joyStickPosRight
) {
  if (keysDown & HidNpadButton_B) {
    controller.group = "";
    tsl::goBack();
    return true;
  }
  return false;
}
