#pragma once
#include <stdint.h>
#include <stddef.h>
#include "stm32h7xx_hal_conf.h"

#define I2C_READ(device_addr, reg_addr, buffer, len) (0)
#define I2C_WRITE(device_addr, reg_addr, data, len) (0)
#define BMM_350_I2C_ADDRESS 0x10

typedef struct {
    int32_t x;
    int32_t y;
    int32_t z;
} Magnetic_Field_Vector;

typedef enum {
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
} MAG_MCU_Registers;

typedef enum {
    BMM_OK = 0,
    BMM_ERR_INIT = -1,
    BMM_ERR_READ_REG = -2,
    BMM_ERR_GET_STATUS = -3,
    BMM_ERR_GET_DATA = -4,
    BMM_ERR_COMBINE_AXIS_DATA = -5,
} MAG_ErrorCodes;

class MAG_Driver {
public:
    virtual ~MAG_Driver() = default;
    virtual MAG_ErrorCodes bmm_init(void) = 0;
    virtual MAG_ErrorCodes bmm_get_data(Magnetic_Field_Vector *data) = 0;
};

class MAG_SIM_Driver : public MAG_Driver {
public:
    MAG_ErrorCodes bmm_init(void) override;
    MAG_ErrorCodes bmm_get_data(Magnetic_Field_Vector *data) override;
};

class Mag_MCU_Driver : public MAG_Driver {
public:
    MAG_ErrorCodes bmm_init(void) override;
    MAG_ErrorCodes bmm_read_register(registers_t start_reg, uint8_t *values, size_t len);
    MAG_ErrorCodes bmm_get_status(uint8_t *status);
    MAG_ErrorCodes bmm_get_data(Magnetic_Field_Vector *data) override;
};
