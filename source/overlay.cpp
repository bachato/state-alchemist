#include "overlay.h"

#include "controller.h"
#include "ui/ui_groups.h"

void ModAlchemist::initServices() {
  pmdmntInitialize();
  pminfoInitialize();

  controller.init();
  controller.enableAllActiveMods();
}
void ModAlchemist::exitServices() {
  controller.disableAllActiveMods();
  
  pminfoExit();
  pmdmntExit();
}

void ModAlchemist::onHide() {}
void ModAlchemist::onShow() {}

std::unique_ptr<tsl::Gui> ModAlchemist::loadInitialGui() {
  return initially<GuiGroups>();
}