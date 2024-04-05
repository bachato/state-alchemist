#include <tesla.hpp>    // The Tesla Header

/**
 * UI displayed when an error occurs (hopefully this UI is never seen)
 */
class GuiError : public tsl::Gui {
  private:
    std::string message;

  public:
    GuiError(std::string message) { this->message = message; }

    virtual tsl::elm::Element* createUI() override {
        auto frame = new tsl::elm::OverlayFrame("The Mod Alchemist", "Error");
        
        auto uiMessage = new tsl::elm::CategoryHeader(this->message);
        frame->setContent(uiMessage);
        return frame;
    }

    virtual void update() override { }

    virtual bool handleInput(u64 keysDown, u64 keysHeld, const HidTouchState &touchPos, HidAnalogStickState joyStickPosLeft, HidAnalogStickState joyStickPosRight) override {
        if (keysDown & HidNpadButton_B) {
            tsl::goBack();
            return true;
        }
        return false;
    }
};

