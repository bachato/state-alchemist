#include "ui/ui_sources.h"
#include "ui/ui_mods.h"
#include "ui/ui_ratings.h"

#include <string>
#include <vector>

#include "controller.h"

GuiSources::GuiSources() {}

tsl::elm::Element* GuiSources::createUI() {
  auto frame = new tsl::elm::OverlayFrame("State Alchemist", controller.group);

  auto list = new tsl::elm::List();

  std::vector<std::string> sources = controller.loadSources(true);

  // For when the group is empty for some reason:
  if (sources.empty()) {
    frame->setContent(new tsl::elm::CategoryHeader("Group is empty"));
    return frame;
  }

  list->addItem(new tsl::elm::CategoryHeader("\uE0E0 View Mods    |    \uE0E3 View Mod Probabilities"));

  // List all of the group's sources:
  for (const std::string &source : sources) {
    auto *item = new tsl::elm::ListItem(source);

    item->setClickListener([this, source](u64 keys) {
      if (keys & HidNpadButton_A) {
        controller.source = source;
        tsl::changeTo<GuiMods>();
        return true;
      }

      if (keys & HidNpadButton_Y) {
        controller.source = source;
        tsl::changeTo<GuiRatings>();
        return true;
      }

      return false;
    });

    list->addItem(item);
  }

  frame->setContent(list);
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
