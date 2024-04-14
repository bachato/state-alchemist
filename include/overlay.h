#pragma once

#include <tesla.hpp>

#include "ui/ui_main.h"

class ModAlchemist : public tsl::Overlay {
public:
                                             // libtesla already initialized fs, hid, pl, pmdmnt, hid:sys and set:sys
    virtual void initServices() override {} // Called at the start to initialize all services necessary for this Overlay
    virtual void exitServices() override {} // Callet at the end to clean up all services previously initialized

    virtual void onShow() override {}
    virtual void onHide() override {}

    virtual std::unique_ptr<tsl::Gui> loadInitialGui() override {
        return initially<GuiMain>();
    }
};