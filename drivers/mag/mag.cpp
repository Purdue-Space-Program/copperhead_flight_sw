#include "mag.h"

class MAG_SIM_Driver : public MAG_Driver {
public:
    MAG_ErrorCodes bmm_init(void) override {

    }

    MAG_ErrorCodes bmm_get_data(Magnetic_Field_Vector *data) {

    }
};

class MAG_MCU_Driver : public MAG_Driver {
public:
    MAG_ErrorCodes bmm_init(void) override {
        uint8_t cmd = 0xb6;
        if (I2C_WRITE(BMM_350_I2C_ADDRESS, CMD, &cmd, 1) != BMM_OK) return BMM_ERR_INIT;
        return BMM_OK;
    } 

    MAG_ErrorCodes bmm_read_register(MAG_MCU_Registers start_reg, uint8_t *values, size_t len) {
        if (values == NULL || len == 0) return BMM_ERR_READ_REG;
        if (I2C_READ(BMM_350_I2C_ADDRESS, start_reg, values, len) != BMM_OK) return BMM_ERR_READ_REG;
        return BMM_OK;
    }

    MAG_ErrorCodes bmm_get_status(uint8_t *status) {
        if (I2C_READ(BMM_350_I2C_ADDRESS, INT_STATUS, status, 1) != BMM_OK) return BMM_ERR_GET_STATUS;
        return BMM_OK;
    }

    MAG_ErrorCodes bmm_get_data(Magnetic_Field_Vector *data) override {
        if (data == NULL) return BMM_ERR_GET_DATA;
        int32_t combined_value;
        uint8_t buffer[9];
        if (bmm_read_register(MAG_X_XLSB, buffer, 9) != BMM_OK) return BMM_ERR_GET_DATA; 
        data->x = ((int32_t)buffer[0] << 16) | ((int32_t)buffer[1] << 8) | (int32_t)buffer[2];    
        data->y = ((int32_t)buffer[3] << 16) | ((int32_t)buffer[4] << 8) | (int32_t)buffer[5];
        data->z = ((int32_t)buffer[6] << 16) | ((int32_t)buffer[7] << 8) | (int32_t)buffer[8];
        return BMM_OK;
    }
};
