#include "mag.h"


Mag_Driver::ErrorCodes Mag_SIM_Driver::bmm_init(void) {
    return ErrorCodes::BMM_OK;
}

Mag_Driver::ErrorCodes Mag_SIM_Driver::bmm_get_data(Mag_Driver::Magnetic_Field_Vector *data) {
    return ErrorCodes::BMM_OK;
}



Mag_Driver::ErrorCodes Mag_MCU_Driver::bmm_init(void) {
    uint8_t cmd = 0xb6;
    if (I2C_WRITE(BMM_350_I2C_ADDRESS, static_cast<uint16_t>(Registers::CMD), &cmd, 1) != static_cast<HAL_StatusTypeDef>(ErrorCodes::BMM_OK)) return ErrorCodes::BMM_ERR_INIT;
    return ErrorCodes::BMM_OK;
} 

Mag_Driver::ErrorCodes Mag_MCU_Driver::bmm_read_register(Mag_MCU_Driver::Registers start_reg, uint8_t *values, size_t len) {
    if (values == NULL || len == 0) return ErrorCodes::BMM_ERR_READ_REG;
    if (I2C_READ(BMM_350_I2C_ADDRESS, static_cast<uint16_t>(start_reg), values, len) != static_cast<HAL_StatusTypeDef>(ErrorCodes::BMM_OK)) return ErrorCodes::BMM_ERR_READ_REG;
    return ErrorCodes::BMM_OK;
}

Mag_Driver::ErrorCodes Mag_MCU_Driver::bmm_get_status(uint8_t *status) {
    if (I2C_READ(BMM_350_I2C_ADDRESS, static_cast<uint16_t>(Registers::INT_STATUS), status, 1) != static_cast<HAL_StatusTypeDef>(ErrorCodes::BMM_OK)) return ErrorCodes::BMM_ERR_GET_STATUS;
    return ErrorCodes::BMM_OK;
}

Mag_Driver::ErrorCodes Mag_MCU_Driver::bmm_get_data(Mag_Driver::Magnetic_Field_Vector *data) {
    if (data == NULL) return ErrorCodes::BMM_ERR_GET_DATA;
    uint8_t buffer[9];
    if (bmm_read_register(Registers::MAG_X_XLSB, buffer, 9) != ErrorCodes::BMM_OK) return ErrorCodes::BMM_ERR_GET_DATA; 
    data->x = ((int32_t)buffer[0] << 16) | ((int32_t)buffer[1] << 8) | (int32_t)buffer[2];    
    data->y = ((int32_t)buffer[3] << 16) | ((int32_t)buffer[4] << 8) | (int32_t)buffer[5];
    data->z = ((int32_t)buffer[6] << 16) | ((int32_t)buffer[7] << 8) | (int32_t)buffer[8];
    return ErrorCodes::BMM_OK;
}

