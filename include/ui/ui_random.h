#ifndef GUI_RANDOM_HPP
#define GUI_RANDOM_HPP

#include <tesla.hpp>    // The Tesla Header

/**
 * Randomly activates/deactivates all mods
 */
class GuiRandom : public tsl::Gui {
  public:
    GuiRandom();

    virtual tsl::elm::Element* createUI() override;

    virtual bool handleInput(
      u64 keysDown,
      u64 keysHeld,
      const HidTouchState &touchPos,
      HidAnalogStickState joyStickPosLeft,
      HidAnalogStickState joyStickPosRight
    ) override;
};

#endif // GUI_RANDOM_HPP