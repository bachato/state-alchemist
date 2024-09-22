#ifndef UI_MODS_HPP
#define UI_MODS_HPP

#include <tesla.hpp>    // The Tesla Header

#include <vector>

class GuiMods : public tsl::Gui {
  private:
    std::vector<tsl::elm::ToggleListItem*> toggles;

    void activateMod(const std::string& mod, tsl::elm::ToggleListItem* toggle);
    void deactivateMod(const std::string& mod);

    void activateDefaultMod();
    void deactivateDefaultMod();

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
