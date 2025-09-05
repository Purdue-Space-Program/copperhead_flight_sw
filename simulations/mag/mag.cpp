#include "mag.h"


Mag_Base_Driver::ErrorCodes Mag_SIM_Driver::init(void) {
    return ErrorCodes::BMM_OK;
}

Mag_Base_Driver::ErrorCodes Mag_SIM_Driver::get_data(Mag_Base_Driver::Magnetic_Field_Vector *data) {
    return ErrorCodes::BMM_OK;
}

