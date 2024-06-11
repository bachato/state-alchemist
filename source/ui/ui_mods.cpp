#include "ui/ui_mods.h"

#include <string>

#include "controller.h"

GuiMods::GuiMods() { }

tsl::elm::Element* GuiMods::createUI() {
  auto frame = new tsl::elm::OverlayFrame("The Mod Alchemist", controller.source);

  std::vector<std::string> mods = controller.loadMods();
  std::string_view activeMod = controller.getActiveMod();

  auto list = new tsl::elm::List();

  // Used to disable any active mod:
  auto *defaultToggle = new tsl::elm::ToggleListItem("Default " + controller.source, activeMod == "");
  defaultToggle->setStateChangedListener([this](bool state) {
    if (state) {
      this->toggles[0]->setState(true); /* Cannot "unset" default toggle */
    } else {
      // De-activate whatever the other active mod is:
      for (const auto &toggle: this->toggles) {
        toggle->setState(false);
      }
      this->toggles[0]->setState(true);
      controller.deactivateMod();
    }
  });

  // Add the default option:
  this->toggles.push_back(defaultToggle);
  list->addItem(this->toggles[0]);

  // Add a toggle for each mod:
  for (const std::string &mod : mods) {
    auto *item = new tsl::elm::ToggleListItem(mod, mod == activeMod);

    item->setStateChangedListener([this, mod](bool state) {
      if (state) {
        this->toggles[0]->setState(false); /* toggles[0] is the default no-mod toggle */
        controller.deactivateMod();
        controller.activateMod(mod);
      } else {
        this->toggles[0]->setState(true);
        controller.deactivateMod();
      }
    });

    this->toggles.push_back(item);
    list->addItem(item);
  }

  frame->setContent(list);
  return frame;
}

bool GuiMods::handleInput(
  u64 keysDown,
  u64 keysHeld,
  const HidTouchState &touchPos,
  HidAnalogStickState joyStickPosLeft,
  HidAnalogStickState joyStickPosRight
) {
  if (keysDown & HidNpadButton_B) {
    controller.source = "";
    tsl::goBack();
    return true;
  }
  return false;
}
