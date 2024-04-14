#define TESLA_INIT_IMPL // If you have more than one file using the tesla header, only define this in the main one
#include <tesla.hpp>    // The Tesla Header

#include "overlay.h"

int main(int argc, char **argv) {
    return tsl::loop<ModAlchemist>(argc, argv);
}