#include "ui/ui_random.h"

#include "controller.h"

/**
 * Randomly activates/deactivates all mods
 */
GuiRandom::GuiRandom() {
  controller.randomize();
}

tsl::elm::Element* GuiRandom::createUI() {
  auto frame = new tsl::elm::OverlayFrame("The Mod Alchemist", "Version 1.0.0");
  frame->setContent(new tsl::elm::ListItem("Randomization Completed"));
  return frame;
}

bool GuiRandom::handleInput(
  u64 keysDown,
  u64 keysHeld,
  const HidTouchState &touchPos,
  HidAnalogStickState joyStickPosLeft,
  HidAnalogStickState joyStickPosRight
) {
  if (keysDown & (HidNpadButton_B | HidNpadButton_A)) {
    tsl::goBack();
    return true;
  }
  return false;
}