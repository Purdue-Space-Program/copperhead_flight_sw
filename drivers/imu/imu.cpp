#include "imu.h"

etl::expected<void, IMU_Base_Driver::ErrorCode> IMU_MCU_Driver::acceleration_init(void) {
    if (acceleration_read_register(IMU_Accel_Reg::ACC_CHIP_ID).has_value()) {
        return {};
    } else {
        return etl::unexpected(acceleration_read_register(IMU_Accel_Reg::ACC_CHIP_ID).error());
    }
}

etl::expected<uint8_t, IMU_Base_Driver::ErrorCode> IMU_MCU_Driver::acceleration_read_register(IMU_Accel_Reg reg) {
    uint8_t address = 1 | (static_cast<uint8_t>(reg) << 1);
    SPI_WRITE(&address, 1);

    uint8_t return_values[2] = { 0 };
    SPI_READ(return_values, 2);

    return return_values[1];
}

etl::expected<void, IMU_Base_Driver::ErrorCode> IMU_MCU_Driver::acceleration_write_register(IMU_Accel_Reg reg, uint8_t value) {
    uint8_t address = 0 | (static_cast<uint8_t>(reg) << 1);
    uint8_t to_write[2] = { address, value };

    SPI_WRITE(to_write, 2);
    return {};
}

etl::expected<void, IMU_Base_Driver::ErrorCode> IMU_MCU_Driver::acceleration_read_registers(IMU_Accel_Reg starting_reg, uint8_t* buf, uint8_t size) {
    uint8_t address = 1 | (static_cast<uint8_t>(starting_reg) << 1);
    SPI_WRITE(&address, 1);
    SPI_READ(buf, size);
    return {};
}

etl::expected<IMU_Base_Driver::Acceleration_Vector_mg, IMU_Base_Driver::ErrorCode> IMU_MCU_Driver::get_acceleration(void) {
    uint8_t buf[7] = { 0 };
    if (!acceleration_read_registers(IMU_Accel_Reg::ACC_X_LSB, buf, 7).has_value()) {
        return etl::unexpected(acceleration_read_registers(IMU_Accel_Reg::ACC_X_LSB, buf, 7).error());
    }
    uint8_t acc_range;
    if (acceleration_read_register(IMU_Accel_Reg::ACC_RANGE).has_value()) {
       acc_range = acceleration_read_register(IMU_Accel_Reg::ACC_RANGE).value();
    } else {
        return etl::unexpected(acceleration_read_register(IMU_Accel_Reg::ACC_RANGE).error());
    }
    
    uint16_t x_accel = (buf[2] << 8) | buf[1];
    uint16_t y_accel = (buf[4] << 8) | buf[3];
    uint16_t z_accel = (buf[6] << 8) | buf[5];

    Acceleration_Vector_mg acc_vec = {
        .x = ((float)x_accel / 32768.0) * 1000.0 * (1 << (acc_range + 1)) * 1.5,
        .y = ((float)y_accel / 32768.0) * 1000.0 * (1 << (acc_range + 1)) * 1.5,
        .z = ((float)z_accel / 32768.0) * 1000.0 * (1 << (acc_range + 1)) * 1.5,
    };
    
    return acc_vec;
}
