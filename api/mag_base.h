#pragma once
#include <stdint.h>
#include <stddef.h>

class Mag_Base_Driver {
public:
        
    struct Magnetic_Field_Vector {
        int32_t x;
        int32_t y;
        int32_t z;
    };

    enum class ErrorCodes {
        BMM_OK = 0,
        BMM_ERR_INIT = -1,
        BMM_ERR_READ_REG = -2,
        BMM_ERR_GET_STATUS = -3,
        BMM_ERR_GET_DATA = -4,
        BMM_ERR_COMBINE_AXIS_DATA = -5,
    };

    virtual ~Mag_Base_Driver() = default;
    virtual ErrorCodes init(void) = 0;
    virtual ErrorCodes get_data(Magnetic_Field_Vector *data) = 0;
};
