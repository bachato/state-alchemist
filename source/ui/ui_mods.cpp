#include "ui/ui_mods.h"
#include "ui/ui_error.h"

#include <string>

#include "controller.h"

GuiMods::GuiMods() { }

tsl::elm::Element* GuiMods::createUI() {
  auto frame = new tsl::elm::OverlayFrame("State Alchemist", controller.source);

  std::vector<std::string> mods = controller.loadMods();
  std::string activeMod = controller.getActiveMod(controller.source);

  auto list = new tsl::elm::List();

  list->addItem(new tsl::elm::CategoryHeader("Turn Mods On/Off"));
  // Used to disable any active mod:
  auto *defaultToggle = new tsl::elm::ToggleListItem("Default " + controller.source, activeMod == "");
  defaultToggle->setStateChangedListener([this](bool state) {
    if (state) { this->activateDefaultMod(); }
    else { this->deactivateDefaultMod(); }
  });

  // Add the default option:
  this->toggles.push_back(defaultToggle);
  list->addItem(this->toggles[0]);

  // Add a toggle for each mod:
  for (const std::string &mod : mods) {
    auto *item = new tsl::elm::ToggleListItem(mod, mod == activeMod);
    item->setStateChangedListener([this, item, mod](bool state) {
      if (state) { this->activateMod(mod, item); }
      else { this->deactivateMod(mod); }
    });

    this->toggles.push_back(item);
    list->addItem(item);
  }

  frame->setContent(list);
  return frame;
}

/**
 * Activates the specified mod, thereby deactivating the current active one
 */
void GuiMods::activateMod(const std::string& mod, tsl::elm::ToggleListItem* modToggle) {
  controller.deactivateMod();
  controller.activateMod(mod);

  // Untoggle all other mods:
  for (const auto &toggle: this->toggles) {
    if (toggle->getText() != mod) {
      toggle->setState(false);
    }
  }

  // Edge-case: If all the mod's files have conflicts, none of them will be transferred, so the mod won't actually get activated.
  // Untoggle to correctly reflect the state, and notify the user to prevent confusion:
  if (controller.getActiveMod(controller.source) != mod) {
    modToggle->setState(false);
    this->toggles[0]->setState(true);
    tsl::changeTo<GuiError>("Cannot enable. All mod files conflict with active files.");
  }
}

/**
 * Deactivates the specified mod, thereby making the default option the active one
 */
void GuiMods::deactivateMod(const std::string& mod) {
  controller.deactivateMod();
  this->toggles[0]->setState(true);
}

/**
 * Activates the default vanilla option, thereby deactivating whichever mod is currently active
 */
void GuiMods::activateDefaultMod() {
  controller.deactivateMod();

  // Untoggle all mods, but keep the default option toggled:
  for (const auto &toggle: this->toggles) {
    toggle->setState(false);
  }
  this->toggles[0]->setState(true);
}

/**
 * The default option cannot be deactivated unless another mod is being activated
 * 
 * Therefore, if the user attempts to deactivate the default option, this will keep it activated
 */
void GuiMods::deactivateDefaultMod() {
  this->toggles[0]->setState(true);
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
