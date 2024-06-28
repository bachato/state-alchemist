#include "ui/ui_ratings.h"

#include "controller.h"

GuiRatings::GuiRatings() { }

tsl::elm::Element* GuiRatings::createUI() {
  auto frame = new tsl::elm::OverlayFrame("The Mod Alchemist", controller.source);

  this->ratings = controller.loadRatings();
  this->defaultRating = controller.loadDefaultRating();

  // Used for when no mod is active:
  auto *defaultSlider = new tsl::elm::StepTrackBar(" ", 10);
  defaultSlider->setProgress(this->defaultRating);

  defaultSlider->setValueChangedListener([this](u8 value) {
    this->defaultRating = value;
  });
  
  auto list = new tsl::elm::List();

  // Add the default option with the header:
  list->addItem(new tsl::elm::CategoryHeader("Default " + controller.source));
  list->addItem(defaultSlider);

  // Add a header & slider for each mod:
  for (const auto& [name, rating]: this->ratings) {
    list->addItem(new tsl::elm::CategoryHeader(name));

    auto *slider = new tsl::elm::StepTrackBar(" ", 10);
    slider->setProgress(rating);

    slider->setValueChangedListener([this, name](u8 value) {
      this->ratings[name] = value;
    });

    list->addItem(slider);
  }

  frame->setContent(list);
  return frame;
}

bool GuiRatings::handleInput(
  u64 keysDown,
  u64 keysHeld,
  const HidTouchState &touchPos,
  HidAnalogStickState joyStickPosLeft,
  HidAnalogStickState joyStickPosRight
) {
  if (keysDown & HidNpadButton_B) {
    controller.saveRatings(this->ratings);
    controller.saveDefaultRating(this->defaultRating);
    controller.source = "";
    tsl::goBack();
    return true;
  }
  return false;
}