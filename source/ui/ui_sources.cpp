#include "ui/ui_sources.h"
#include "ui/ui_mods.h"
#include "ui/ui_ratings.h"

#include <string>
#include <vector>

#include "controller.h"

GuiSources::GuiSources(EditMode editMode) {
  this->editMode = editMode;
}

tsl::elm::Element* GuiSources::createUI() {
  auto frame = new tsl::elm::OverlayFrame("The Mod Alchemist", controller.group);

  auto groupList = new tsl::elm::List();

  std::vector<std::string> sources = controller.loadSources();

  // For when the group is empty for some reason:
  if (sources.empty()) {
    frame->setContent(new tsl::elm::CategoryHeader("Group is empty"));
    return frame;
  }

  // List all of the group's sources:
  for (const std::string &source : sources) {
    auto *item = new tsl::elm::ListItem(source);

    item->setClickListener([this, source](u64 keys) {
      if (keys & HidNpadButton_A) {
        controller.source = source;

        if (this->editMode == EditMode::TOGGLE) {
          tsl::changeTo<GuiMods>();
        } else if (this->editMode == EditMode::RATING) {
          tsl::changeTo<GuiRatings>();
        }
        return true;
      }
      return false;
    });

    groupList->addItem(item);
  }

  frame->setContent(groupList);
  return frame;
}

bool GuiSources::handleInput(
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
