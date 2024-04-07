#pragma once

#include <tesla.hpp>    // The Tesla Header

/**
 * UI displayed when an error occurs (hopefully this UI is never seen)
 */
class GuiError : public tsl::Gui {
  private:
    std::string message;

  public:
    GuiError(std::string message);

    virtual tsl::elm::Element* createUI() override;

    virtual void update() override;

    virtual bool handleInput(
      u64 keysDown,
      u64 keysHeld,
      const HidTouchState &touchPos,
      HidAnalogStickState joyStickPosLeft,
      HidAnalogStickState joyStickPosRight
    ) override;
};

