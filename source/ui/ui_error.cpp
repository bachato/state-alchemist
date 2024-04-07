#include "ui_error.h"

/**
 * UI displayed when an error occurs (hopefully this UI is never seen)
 */
GuiError::GuiError(std::string message) { this->message = message; }

tsl::elm::Element* GuiError::createUI() override {
  auto frame = new tsl::elm::OverlayFrame("The Mod Alchemist", "Error");
    
  auto uiMessage = new tsl::elm::CategoryHeader(this->message);
  frame->setContent(uiMessage);
  return frame;
}

void GuiError::update() override { }

bool GuiError::handleInput(u64 keysDown, u64 keysHeld, const HidTouchState &touchPos, HidAnalogStickState joyStickPosLeft, HidAnalogStickState joyStickPosRight) override {
  if (keysDown & HidNpadButton_B) {
    tsl::goBack();
    return true;
  }
  return false;
}