#include "ui/ui_main.h"
#include "ui/ui_groups.h"
#include "ui/ui_all_disabled.h"
#include "ui/ui_random.h"

#include "controller.h"
#include "constants.h"

GuiMain::GuiMain() { }

tsl::elm::Element* GuiMain::createUI() {
  auto frame = new tsl::elm::OverlayFrame("State Alchemist", "Version 1.0.0");

  auto list = new tsl::elm::List();

  controller.init();
  
  // Error message when the game's Folder does not exist or isn't named as the ID:
  if (!controller.doesGameHaveFolder()) {
    list->addItem(new tsl::elm::ListItem("The running game has no folder."));
    list->addItem(new tsl::elm::ListItem("It should be named \"" + controller.getHexTitleId() + "\""));
    list->addItem(new tsl::elm::ListItem("And located in the \"" + ALCHEMIST_PATH + "\" directory."));
    frame->setContent(list);
    return frame;
  }

  auto* setMods = new tsl::elm::ListItem("View Mod Groups");
  setMods->setClickListener([](u64 keys) {
    if (keys & HidNpadButton_A) {
      tsl::changeTo<GuiGroups>();
      return true;
    }
    return false;
  });
  list->addItem(setMods);

  auto* random = new tsl::elm::ListItem("Pick at Random");
  random->setClickListener([](u64 keys) {
    if (keys & HidNpadButton_A) {
      tsl::changeTo<GuiRandom>();
      return true;
    }
    return false;
  });
  list->addItem(random);

  // A little extra space above the option for disabling all:
  list->addItem(new tsl::elm::CategoryHeader("-------------------------"));

  auto* disableAll = new tsl::elm::ListItem("Disable All Mods");
  disableAll->setClickListener([](u64 keys) {
    if (keys & HidNpadButton_A) {
      tsl::changeTo<GuiAllDisabled>();
      return true;
    }
    return false;
  });
  list->addItem(disableAll);

  frame->setContent(list);

  return frame;
}