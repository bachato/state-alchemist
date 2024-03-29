#include <tesla.hpp>    // The Tesla Header

class GuiGroups : public tsl::Gui {
  public:
    GuiGroups() { }

    // Called when this Gui gets loaded to create the UI
    // Allocate all elements on the heap. libtesla will make sure to clean them up when not needed anymore
    virtual tsl::elm::Element* createUI() override {
        // A OverlayFrame is the base element every overlay consists of. This will draw the default Title and Subtitle.
        // If you need more information in the header or want to change it's look, use a HeaderOverlayFrame.
        auto frame = new tsl::elm::OverlayFrame("The Mod Alchemist", "Mod Groups");

        // NAVIGATE TO GAME'S CURRENT DIRECTORY

        // IF GAME FOLDER NOT FOUND
        auto message = new tsl::elm::CategoryHeader("Game folder not found for ID: GAME_ID. Are mods set up for this game?");
        frame->setContent(message);
        return frame;

        // ITERATE OVER FOLDER NAMES, CREATING LIST FROM THEM

        // A list that can contain sub elements and handles scrolling
        /*auto list = new tsl::elm::List();

        // Create and add a new list item to the list
        list->addItem(new tsl::elm::ListItem("Set Mods"));

        // Add the list to the frame for it to be drawn
        frame->setContent(list);*/

        // Return the frame to have it become the top level element of this Gui
        return frame;
    }

    // Called once every frame to update values
    virtual void update() override {

    }

    // Called once every frame to handle inputs not handled by other UI elements
    virtual bool handleInput(u64 keysDown, u64 keysHeld, const HidTouchState &touchPos, HidAnalogStickState joyStickPosLeft, HidAnalogStickState joyStickPosRight) override {
        if (keysDown & HidNpadButton_B) {
            tsl::changeTo<GuiMain>(); // Exit the overlay
            return true;
        }
        return false;
    }
};

