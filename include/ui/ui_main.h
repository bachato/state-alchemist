#ifndef GUI_MAIN_HPP
#define GUI_MAIN_HPP

#include <tesla.hpp>    // The Tesla Header

class GuiMain : public tsl::Gui {
  public:
    GuiMain();
    virtual tsl::elm::Element* createUI() override;
};

#endif // GUI_MAIN_HPP