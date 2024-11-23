#include "ui/ui_error.h"   // Include the header file

GuiError::GuiError(std::string message) {
  this->message = message;
}

tsl::elm::Element* GuiError::createUI() {
  auto frame = new tsl::elm::OverlayFrame("State Alchemist", "Error");
  auto list = new tsl::elm::List();
  list->addItem(new tsl::elm::ListItem(this->message));
  frame->setContent(list);
  return frame;
}

bool GuiError::handleInput(
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

/**
 * Displays an error code to the user if @param r is erroneous
 * 
 * @param alchemyCode: A short semi-readable unique code to indicate the origin of the error in Mod Alchemist's code
 */
void GuiError::tryResult(const Result& r, const std::string& alchemyCode) {
  if (R_FAILED(r)) {
    tsl::changeTo<GuiError>("Error: " + alchemyCode + " " + std::to_string(r));
    abort();
  }
}
