#ifndef UI_MODS_HPP
#define UI_MODS_HPP

#include <tesla.hpp>    // The Tesla Header

#include <vector>

class GuiMods : public tsl::Gui {
  private:
    std::vector<tsl::elm::ToggleListItem*> toggles;

  public:
    GuiMods();

    virtual tsl::elm::Element* createUI() override;

    virtual bool handleInput(
      u64 keysDown,
      u64 keysHeld,
      const HidTouchState &touchPos,
      HidAnalogStickState joyStickPosLeft,
      HidAnalogStickState joyStickPosRight
    ) override;
};

#endif // UI_MODS_HPP
