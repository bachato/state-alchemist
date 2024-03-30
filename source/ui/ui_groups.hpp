#include <tesla.hpp>    // The Tesla Header

#include <dmntcht.h>
#include "../controller.hpp"

class GuiGroups : public tsl::Gui {
  public:
    GuiGroups() { }

    // Called when this Gui gets loaded to create the UI
    // Allocate all elements on the heap. libtesla will make sure to clean them up when not needed anymore
    virtual tsl::elm::Element* createUI() override {
        // A OverlayFrame is the base element every overlay consists of. This will draw the default Title and Subtitle.
        // If you need more information in the header or want to change it's look, use a HeaderOverlayFrame.
        auto frame = new tsl::elm::OverlayFrame("The Mod Alchemist", "Mod Groups");

        try {
          DmntCheatProcessMetadata metadata;
          dmntchtGetCheatProcessMetadata(&metadata);
          Controller controller = Controller(metadata.title_id);

          auto list = controller.loadGroups();
          frame->setContent(list);
          return frame;
        } catch (std::filesystem::filesystem_error &e) {
          auto message = new tsl::elm::CategoryHeader(e.what());
          frame->setContent(message);
          return frame;
        }
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

