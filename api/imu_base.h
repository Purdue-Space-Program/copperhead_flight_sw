#pragma once

#include <etl/expected.h>
#include <stdint.h>
#include <stddef.h>

class IMU_Base_Driver {
public:    
    struct Acceleration_Vector_mg {
        float x;
        float y;
        float z;
    };
 
    enum class ErrorCode {
        IMU_OK,
        IMU_ERR,
    };

    virtual ~IMU_Base_Driver() = default;

    virtual etl::expected<void, ErrorCode> acceleration_init(void) = 0;
    virtual etl::expected<Acceleration_Vector_mg, ErrorCode> get_acceleration(void) = 0; 
};
