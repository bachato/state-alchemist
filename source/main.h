#pragma once

#include <tesla.hpp>    // The Tesla Header



class GuiMain : public tsl::Gui {
public:
    GuiMain() {}

    // Called when this Gui gets loaded to create the UI
    virtual tsl::elm::Element* createUI() override;

    // Called once every frame to update values
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

class ModAlchemist : public tsl::Overlay {
public:
                                             // libtesla already initialized fs, hid, pl, pmdmnt, hid:sys and set:sys
    virtual void initServices() override; // Called at the start to initialize all services necessary for this Overlay
    virtual void exitServices() override; // Callet at the end to clean up all services previously initialized

    virtual void onShow() override;
    virtual void onHide() override;

    virtual std::unique_ptr<tsl::Gui> loadInitialGui() override;
};

int main(int argc, char **argv);
