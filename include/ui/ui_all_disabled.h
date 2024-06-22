#ifndef GUI_ALL_DISABLED_HPP
#define GUI_ALL_DISABLED_HPP

#include <tesla.hpp>    // The Tesla Header

class GuiAllDisabled : public tsl::Gui {
  private:
    tsl::elm::List* items;
    tsl::elm::ListItem* yes;

  public:
    GuiAllDisabled();

    virtual tsl::elm::Element* createUI() override;

    virtual bool handleInput(
      u64 keysDown,
      u64 keysHeld,
      const HidTouchState &touchPos,
      HidAnalogStickState joyStickPosLeft,
      HidAnalogStickState joyStickPosRight
    ) override;
};

#endif // GUI_ALL_DISABLED_HPP