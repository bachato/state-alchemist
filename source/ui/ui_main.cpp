#include "ui/ui_main.h"
#include "ui/ui_groups.h"
#include "ui/ui_all_disabled.h"
#include "ui/ui_random.h"

#include "controller.h"
#include "constants.h"

GuiMain::GuiMain() { }

tsl::elm::Element* GuiMain::createUI() {
  auto frame = new tsl::elm::OverlayFrame("The Mod Alchemist", "Version 1.0.0");

  auto list = new tsl::elm::List();

  controller.init();
  
  if (!controller.doesGameHaveFolder()) {
    list->addItem(new tsl::elm::ListItem("The running game has no folder."));
    list->addItem(new tsl::elm::ListItem("It should be named \"" + controller.getHexTitleId() + "\""));
    list->addItem(new tsl::elm::ListItem("And located in the \"" + ALCHEMIST_PATH + "\" directory."));
    frame->setContent(list);
    return frame;
  }

  auto* random = new tsl::elm::ListItem("Randomize");
  random->setClickListener([](u64 keys) {
    if (keys & HidNpadButton_A) {
      tsl::changeTo<GuiRandom>();
      return true;
    }
    return false;
  });

  auto* setMods = new tsl::elm::ListItem("Set Mods");
  setMods->setClickListener([](u64 keys) {
    if (keys & HidNpadButton_A) {
      tsl::changeTo<GuiGroups>(EditMode::TOGGLE);
      return true;
    }
    return false;
  });

  auto* setRatings = new tsl::elm::ListItem("Randomize Preferences");
  setRatings->setClickListener([](u64 keys) {
    if (keys & HidNpadButton_A) {
      tsl::changeTo<GuiGroups>(EditMode::RATING);
      return true;
    }
    return false;
  });

  auto* disableAll = new tsl::elm::ListItem("Disable All Mods");
  disableAll->setClickListener([](u64 keys) {
    if (keys & HidNpadButton_A) {
      tsl::changeTo<GuiAllDisabled>();
      return true;
    }
    return false;
  });

  list->addItem(random);
  list->addItem(setMods);
  list->addItem(setRatings);
  list->addItem(disableAll);

  frame->setContent(list);

  return frame;
}