#include "imu.h"

etl::expected<void, IMU_Base_Driver::ErrorCode> IMU_SIM_Driver::acceleration_init(void) {
    return {};
}

etl::expected<IMU_Base_Driver::Acceleration_Vector_mg, IMU_Base_Driver::ErrorCode> get_acceleration(void) {
    return etl::unexpected(IMU_Base_Driver::ErrorCode::IMU_OK);
}
