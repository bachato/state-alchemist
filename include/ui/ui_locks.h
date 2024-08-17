#ifndef UI_LOCKS_H
#define UI_LOCKS_H

#include <tesla.hpp>

class GuiLocks : public tsl::Gui {
  public:
    GuiLocks();

    virtual tsl::elm::Element* createUI() override;

    virtual bool handleInput(
      u64 keysDown,
      u64 keysHeld,
      const HidTouchState &touchPos,
      HidAnalogStickState joyStickPosLeft,
      HidAnalogStickState joyStickPosRight
    ) override;
};

#endif // UI_LOCKS_H

