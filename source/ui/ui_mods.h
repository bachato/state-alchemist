#pragma once

#include <tesla.hpp>    // The Tesla Header

#include "../controller.h"

/**
 * List of toggable mods for a specific source 
 */
class GuiMods : public tsl::Gui {
  private:
    Controller controller;
    std::string source;
    std::string group;
    std::vector<tsl::elm::ToggleListItem*> toggles;

  public:
    GuiMods(Controller& controller_, std::string& source_, std::string& group_)
      : controller(controller_), source(source_), group(group_);

    virtual tsl::elm::Element* createUI() override;

    virtual void update() override { };

    virtual bool handleInput(
      u64 keysDown,
      u64 keysHeld,
      const HidTouchState &touchPos,
      HidAnalogStickState joyStickPosLeft,
      HidAnalogStickState joyStickPosRight
    ) override;
};

