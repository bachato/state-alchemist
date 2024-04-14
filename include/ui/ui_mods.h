#ifndef UI_MODS_HPP
#define UI_MODS_HPP

#include <string>
#include <vector>

#include <tesla.hpp>    // The Tesla Header

#include "../controller.h"

class GuiMods : public tsl::Gui {
  private:
    Controller controller;
    std::string source;
    std::string group;
    std::vector<tsl::elm::ToggleListItem*> toggles;

  public:
    GuiMods(Controller& controller_, const std::string& source_, const std::string& group_);

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
