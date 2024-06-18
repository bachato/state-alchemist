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
  defaultToggle->setStateChangedListener([this, defaultToggle](bool state) {
    if (state) {
      // De-activate whatever the other active mod is:
      for (const auto &toggle: this->toggles) {
        toggle->setState(false);
      }
      defaultToggle->setState(true); // Reactivate
      controller.deactivateMod();
    } else {
      defaultToggle->setState(true); /* Cannot "unset" default toggle */
    }
  });

  // Add the default option:
  this->toggles.push_back(defaultToggle);
  list->addItem(defaultToggle);

  // Add a toggle for each mod:
  for (const std::string &mod : mods) {
    auto *toggle = new tsl::elm::ToggleListItem(mod, mod == activeMod);

    toggle->setStateChangedListener([this, defaultToggle, toggle, mod](bool state) {
      if (state) {
        // De-activate whatever the other active mod is:
        for (const auto &otherToggle: this->toggles) {
          otherToggle->setState(false);
        }
        toggle->setState(true); // Reactivate
        controller.deactivateMod();
        controller.activateMod(mod);
      } else {
        this->toggles[0]->setState(true); /* Set "default" option to true */
        controller.deactivateMod();
      }
    });

    this->toggles.push_back(toggle);
    list->addItem(toggle);
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
