#include "ui/ui_random.h"

#include "controller.h"

/**
 * UI for activating / deactivating mods at random
 */
GuiRandom::GuiRandom() {}

tsl::elm::Element* GuiRandom::createUI() {
  auto frame = new tsl::elm::OverlayFrame("State Alchemist", "Random Mods");
  this->items = new tsl::elm::List();

  this->items->addItem(new tsl::elm::CategoryHeader("This will enable mods at random"));
  this->items->addItem(new tsl::elm::CategoryHeader("Tesla menu will freeze briefly"));
  this->items->addItem(new tsl::elm::CategoryHeader("(up to a minute if there are many mods)"));
  this->items->addItem(new tsl::elm::CategoryHeader("This menu will change when it is done"));

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

      // Begin randomly choosing mods
      controller.randomize();
      removeFocus(this->yes);
      this->items->clear();

      // Create new form for when randomization finishes
      auto* finished = new tsl::elm::ListItem("Finished!");
      finished->setClickListener([](u64 keys) {
        if (keys & HidNpadButton_A) {
          tsl::goBack();
          return true;
        }
        return false;
      });

      this->items->addItem(finished);
      this->items->addItem(new tsl::elm::CategoryHeader("Please relaunch game now"));
      return true;
    }
    return false;
  });

  this->items->addItem(no);
  this->items->addItem(this->yes);

  frame->setContent(this->items);

  return frame;
}

bool GuiRandom::handleInput(
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