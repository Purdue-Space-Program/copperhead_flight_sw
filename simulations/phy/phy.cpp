#include "phy.h"


PHY_Base_Driver::ErrorCodes PHY_SIM_Driver::init(void) {
    return ErrorCodes::PHY_OK;
}

PHY_Base_Driver::ErrorCodes PHY_SIM_Driver::read_mode(uint8_t *mode) {
    return ErrorCodes::PHY_OK;
}

PHY_Base_Driver::ErrorCodes PHY_SIM_Driver::apply_mode(const uint8_t mode) {
    return ErrorCodes::PHY_OK;
}

