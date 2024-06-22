#include "ui/ui_all_disabled.h"

#include "controller.h"

GuiAllDisabled::GuiAllDisabled() {
  controller.deactivateAll();
}

tsl::elm::Element* GuiAllDisabled::createUI() {
  auto frame = new tsl::elm::OverlayFrame("The Mod Alchemist", "Version 1.0.0");
  this->items = new tsl::elm::List();

  this->items->addItem(new tsl::elm::CategoryHeader("Disable all active mods?"));

  auto* no = new tsl::elm::ListItem("Cancel");
  no->setClickListener([](u64 keys) {
    if (keys & HidNpadButton_A) {
      tsl::goBack();
      return true;
    }
    return false;
  });

  this->yes = new tsl::elm::ListItem("OK");
  this->yes->setClickListener([this](u64 keys) {
    if (keys & HidNpadButton_A) {
      controller.deactivateAll();
      removeFocus(this->yes);
      this->items->clear();
      this->items->addItem(new tsl::elm::ListItem("All mods have been disabled."));
      return true;
    }
    return false;
  });

  this->items->addItem(no);
  this->items->addItem(this->yes);

  frame->setContent(this->items);

  return frame;
}

bool GuiAllDisabled::handleInput(
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
