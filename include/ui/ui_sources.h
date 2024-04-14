#ifndef UI_SOURCES_H
#define UI_SOURCES_H

#include <string>
#include <vector>

#include <tesla.hpp>
#include "../controller.h"

class GuiSources : public tsl::Gui {
  private:
    Controller controller;
    std::string group;

  public:
    GuiSources(Controller& controller_, const std::string& group_);

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

