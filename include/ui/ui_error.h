#ifndef GUI_ERROR_HPP
#define GUI_ERROR_HPP

#include <string>

#include <tesla.hpp>    // The Tesla Header

class GuiError : public tsl::Gui {
  private:
    std::string message;

  public:
    GuiError(std::string message);

    virtual tsl::elm::Element* createUI() override;

    virtual bool handleInput(
      u64 keysDown,
      u64 keysHeld,
      const HidTouchState &touchPos,
      HidAnalogStickState joyStickPosLeft,
      HidAnalogStickState joyStickPosRight
    ) override;
};

#endif // GUI_ERROR_HPP
