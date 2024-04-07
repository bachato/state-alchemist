#include "main.h"

GuiMain::GuiMain() { }

tsl::elm::Element* GuiMain::createUI() override {
    // A OverlayFrame is the base element every overlay consists of. This will draw the default Title and Subtitle.
    auto frame = new tsl::elm::OverlayFrame("The Mod Alchemist", "v0.1.0");

    auto list = new tsl::elm::List();

    // Option for switching between mods
    auto *mod_item = new tsl::elm::ListItem("Set Mods");
    mod_item->setClickListener([](u64 keys) {
        if (keys & HidNpadButton_A) {
            tsl::changeTo<GuiGroups>();
            return true;
        }
        return false;
    });
    list->addItem(mod_item);

    // Add the list to the frame for it to be drawn
    frame->setContent(list);

    // Return the frame to have it become the top level element of this Gui
    return frame;
}

// Called once every frame to update values
void GuiMain::update() override { }

// Called once every frame to handle inputs not handled by other UI elements
bool GuiMain::handleInput(u64 keysDown, u64 keysHeld, const HidTouchState &touchPos, HidAnalogStickState joyStickPosLeft, HidAnalogStickState joyStickPosRight) override {
    return false;   // Return true here to signal the inputs have been consumed
}

void ModAlchemist::initServices() override {}
void ModAlchemist::exitServices() override {}

void ModAlchemist::onShow() override {}
void ModAlchemist::onHide() override {}

std::unique_ptr<tsl::Gui> ModAlchemist::loadInitialGui() override {
  return initially<GuiMain>();
}

int main(int argc, char **argv) {
    return tsl::loop<ModAlchemist>(argc, argv);
}
