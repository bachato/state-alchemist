#include "ui_error.h"

#include <tesla.hpp>    // The Tesla Header

tsl::elm::Element* GuiError::createUI() {
  auto frame = new tsl::elm::OverlayFrame("The Mod Alchemist", "Error");
    
  auto uiMessage = new tsl::elm::CategoryHeader(this->message);
  frame->setContent(uiMessage);
  return frame;
}

void GuiError::update() { }

bool GuiError::handleInput(u64 keysDown, u64 keysHeld, const HidTouchState &touchPos, HidAnalogStickState joyStickPosLeft, HidAnalogStickState joyStickPosRight) {
  if (keysDown & HidNpadButton_B) {
    tsl::goBack();
    return true;
  }
  return false;
}
