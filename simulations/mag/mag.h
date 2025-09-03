#pragma once

#include "../../api/mag_base.h"

class Mag_SIM_Driver : public Mag_Driver {
public:
    Mag_Driver::ErrorCodes init(void) override;
    Mag_Driver::ErrorCodes get_data(Mag_Driver::Magnetic_Field_Vector *data) override;
};

