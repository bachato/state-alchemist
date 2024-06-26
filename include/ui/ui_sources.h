#ifndef UI_SOURCES_H
#define UI_SOURCES_H

#include "constants.h"

#include <tesla.hpp>

class GuiSources : public tsl::Gui {
  private:
    EditMode editMode;

  public:
    GuiSources(EditMode editMode);

    virtual tsl::elm::Element* createUI() override;

    virtual bool handleInput(
      u64 keysDown,
      u64 keysHeld,
      const HidTouchState &touchPos,
      HidAnalogStickState joyStickPosLeft,
      HidAnalogStickState joyStickPosRight
    ) override;
};

#endif // UI_SOURCES_H

