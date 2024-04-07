#pragma once

#include <tesla.hpp>    // The Tesla Header

#include <dmntcht.h>
#include "ui_sources.h"
#include "../controller.h"

/**
 * List of the groups of mods
 */
class GuiGroups : public tsl::Gui {
  public:
    GuiGroups() { }

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

