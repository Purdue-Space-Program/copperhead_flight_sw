#pragma once
#include <stdint.h>
#include <stddef.h>
#include "../../api/mag_base.h"
#include "stm32h7xx_hal.h"

extern I2C_HandleTypeDef hi2c1;

#define I2C_READ(device_addr, reg_addr, buffer, len) \
HAL_I2C_Mem_Read(&hi2c1, (device_addr) << 1, (reg_addr), I2C_MEMADD_SIZE_8BIT, (uint8_t *)(buffer), (len), HAL_MAX_DELAY)
#define I2C_WRITE(device_addr, reg_addr, data, len) \
HAL_I2C_Mem_Write(&hi2c1, (device_addr) << 1, (reg_addr), I2C_MEMADD_SIZE_8BIT, (uint8_t *)(data), (len), HAL_MAX_DELAY)
#define BMM_350_I2C_ADDRESS 0x10

class Mag_MCU_Driver : public Mag_Base_Driver {
public:

    enum class Registers {
        CMD         = 0x7E,
        INT_STATUS  = 0x30,
        MAG_X_XLSB  = 0x31,
        MAG_X_LSB   = 0x32,
        MAG_X_MSB   = 0x33,
        MAG_Y_XLSB  = 0x34,
        MAG_Y_LSB   = 0x35,
        MAG_Y_MSB   = 0x36,
        MAG_Z_XLSB  = 0x37,
        MAG_Z_LSB   = 0x38,
        MAG_Z_MSB   = 0x39,
    };

    Mag_Base_Driver::ErrorCodes init(void) override;
    Mag_Base_Driver::ErrorCodes read_register(Registers start_reg, uint8_t *values, size_t len);
    Mag_Base_Driver::ErrorCodes get_status(uint8_t *status);
    Mag_Base_Driver::ErrorCodes get_data(Mag_Base_Driver::Magnetic_Field_Vector *data) override;
};

