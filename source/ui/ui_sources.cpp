#include "ui_sources.h"

#include <tesla.hpp>
#include "../controller.h"
#include "ui_mods.h"

tsl::elm::Element* GuiSources::createUI() {
  auto frame = new tsl::elm::OverlayFrame("The Mod Alchemist", this->group);

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
}

void GuiSources::update() { }

bool GuiSources::handleInput(u64 keysDown, u64 keysHeld, const HidTouchState &touchPos, HidAnalogStickState joyStickPosLeft, HidAnalogStickState joyStickPosRight) {
  if (keysDown & HidNpadButton_B) {
    tsl::goBack();
    return true;
  }
  return false;
}

