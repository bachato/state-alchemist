#pragma once

#include <tesla.hpp>
#include "../controller.h"
#include <ui_mods.h>

/**
 * UI for displaying the moddable sources within each group
 */
class GuiSources : public tsl::Gui {
  private:
    Controller controller;
    std::string group;

  public:
    GuiSources(Controller& controller_, std::string& group_) : controller(controller_), group(group_);

    virtual tsl::elm::Element* createUI() override;

    virtual void update() override;

    // Called once every frame to handle inputs not handled by other UI elements
    virtual bool handleInput(
      u64 keysDown,
      u64 keysHeld,
      const HidTouchState &touchPos,
      HidAnalogStickState joyStickPosLeft,
      HidAnalogStickState joyStickPosRight
    ) override;
};

