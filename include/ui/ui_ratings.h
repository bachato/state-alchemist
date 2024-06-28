#ifndef UI_RATINGS_HPP
#define UI_RATINGS_HPP

#include <tesla.hpp>    // The Tesla Header

#include <string>
#include <map>

class GuiRatings : public tsl::Gui {
  private:
    u8 savedDefaultRating;
    u8 defaultRating;
    std::map<std::string, u8> changedRatings;

  public:
    GuiRatings();

    virtual tsl::elm::Element* createUI() override;

    virtual bool handleInput(
      u64 keysDown,
      u64 keysHeld,
      const HidTouchState &touchPos,
      HidAnalogStickState joyStickPosLeft,
      HidAnalogStickState joyStickPosRight
    ) override;
};

#endif // UI_RATINGS_HPP