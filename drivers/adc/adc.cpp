#include "adc.h"

class ADC_SIM_Driver {
public:
    ADC_ErrorCodes ads_init(void) override {
        
    }

    ADC_ErrorCodes ads_set_channel(ADC_SIM_ChannelID channel) override {

    }

    ADC_ErrorCodes ads_data_read(int32_t *result, uint8_t status) override {

    }

};

class ADC_MCU_Driver {
public:
    ADC_ErrorCodes ads_init(void) override {
        uint8_t cmd = ads_build_command(CMD_RST, 0, 0,);
        if(SPI_WRITE(&cmd, 1) != 0) return ADS_WSPI_ERROR;
        return ADS_OK; 
    }

    ADC_ErrorCodes ads_build_command(uint8_t C, uint8_t MUL, ADC_MCU_Register A) {
        return (C << 5) | (MUL << 4) | (A & 0x0F);
    } 

    ADC_ErrorCodes ads_read_register(const ADC_MCU_Register reg, uint8_t* value) {
        uint8_t cmd = build_command(CMD_RREG, 0, reg);
        if (SPI_WRITE(&cmd, 1) != 0) return ADS_WSPI_ERROR;
        if (SPI_READ(value, 1) != 0) return ADS_RSPI_ERROR;
        return ADS_OK;
    }

    ADC_ErrorCodes ads_write_register(ADC_MCU_Register reg, uint8_t value) {
        uint8_t cmd = build_command(CMD_WREG, 0, reg);
        uint8_t count = 0x00;
        if (SPI_WRITE(&cmd, 1) != 0) return ADS_WSPI_ERROR;
        if (SPI_WRITE(&count, 1) != 0) return ADS_WSPI_ERROR;
        if (SPI_WRITE(&value, 1) != 0) return ADS_WSPI_ERROR;
        return ADS_OK;
    }

    ADC_ErrorCodes ads_read_register_mul(const ADC_MCU_Register start_reg, uint8_t* buf, uint8_t size) {
        uint8_t cmd = build_command(CMD_RREG, 1, start_reg);
        if(SPI_WRITE(&cmd, 1) != 0) return ADS_WSPI_ERROR;
        if(SPI_READ(buf, size) != 0) return ADS_RSPI_ERROR;
        return ADS_OK;
    }

    ADC_ErrorCodes ads_write_register_mul(ADC_MCU_Register start_reg, uint8_t* values, uint8_t size) {
        if (size == 0) return ADS_ERROR;
        uint8_t cmd = build_command(CMD_WREG, 1, start_reg);
        uint8_t count = size-1;
        if (SPI_WRITE(&cmd, 1) != 0) return ADS_WSPI_ERROR;
        if (SPI_WRITE(&count, 1) != 0) return ADS_WSPI_ERROR;
        if (SPI_WRITE(values, size) != 0) return ADS_WSPI_ERROR;
        return ADS_OK;
    }

    ADC_ErrorCodes ads_set_channel(ADC_MCU_ChannelID channel) override {
        return ads_write_register(MUXDIF, (uint8_t) channel);
    }
    
    ADC_ErrorCodes ads_data_read(int32_t *result, uint8_t status) override {
	    if (result == NULL || status == NULL) return ADS_ERROR;
        uint8_t cmd = build_command(CMD_DATA_READ, 1, 0);
        uint8_t rx_buf[4] = {0};
        if (SPI_WRITE(&cmd, 1) != 0) return ADS_WSPI_ERROR;
        if(SPI_READ(rx_buf, 4) != 0) return ADS_RSPI_ERROR;
        int32_t raw24 = (rx_buf[1] << 16) | (rx_buf[2] << 8) | (rx_buf[3]);
        *result = (int32_t)(raw24 << 8) >> 8;
        *status = rx_buf[0];
        return ADS_OK;
    }
};
