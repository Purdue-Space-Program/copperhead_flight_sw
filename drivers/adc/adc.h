#pragma once
#include <stdint.h>
#include <stddef.h>
#include "stm32h7xx_hal.h"

extern SPI_HandleTypeDef hspi1;

#define SPI_READ(dst, size) HAL_SPI_Receive(&hspi1, (uint8_t *)dst , size, HAL_MAX_DELAY)
#define SPI_WRITE(src, size) HAL_SPI_Transmit(&hspi1, (uint8_t *)src, size, HAL_MAX_DELAY)

class ADC_Driver {
public:

    enum class ErrorCodes {
        ADS_OK,
        ADS_ERR,
        ADS_RSPI_ERR,
        ADS_WSPI_ERR,
    };

    enum class ChannelID {
        ADS_CH_DIFF0 = 0x00,
        ADS_CH_DIFF1 = 0x01,
        ADS_CH_DIFF2 = 0x02,
        ADS_CH_DIFF3 = 0x03,
        ADS_CH_DIFF4 = 0x04,
        ADS_CH_DIFF5 = 0x05,
        ADS_CH_DIFF6 = 0x06,
        ADS_CH_DIFF7 = 0x07,
    };

    virtual ~ADC_Driver() = default;
    virtual ErrorCodes init(void) = 0;
    virtual ErrorCodes set_channel(ChannelID channel) = 0;
    virtual ErrorCodes data_read(int32_t *result, uint8_t *status) = 0;
};

class ADC_SIM_Driver : public ADC_Driver {
public:

    ADC_Driver::ErrorCodes init(void) override;
    ADC_Driver::ErrorCodes set_channel(ADC_Driver::ChannelID channel) override;
    ADC_Driver::ErrorCodes data_read(int32_t *result, uint8_t *status) override;
};

class ADC_MCU_Driver : public ADC_Driver {
public:
    

    enum class Commands {
        CMD_DATA_READ = 0x01,
        CMD_RREG = 0x02,
        CMD_WREG = 0x03,
        CMD_RST = 0x06,     
    };

    enum class Register {
        CONFIG0 = 0x00,
        CONFIG1 = 0x01,
        MUXSCH 	= 0x02,
        MUXDIF 	= 0x03,
        MUXSG0 	= 0x04,
        MUXSG1 	= 0x05,
        SYSRED 	= 0x06,
        GPIO_C 	= 0x07,
        GPIO_D 	= 0x08,
        ID 		= 0x09,
    };

    ADC_Driver::ErrorCodes init(void) override;
    ADC_Driver::ErrorCodes set_channel(ChannelID channel) override;
    ADC_Driver::ErrorCodes data_read(int32_t *result, uint8_t *status) override;
    uint8_t build_command(Commands C, uint8_t MUL, Register A);
    ADC_Driver::ErrorCodes read_register(const Register reg, uint8_t* value);
    ADC_Driver::ErrorCodes write_register(Register reg, uint8_t value);
    ADC_Driver::ErrorCodes read_register_mul(const Register start_reg, uint8_t* buf, uint8_t size);
    ADC_Driver::ErrorCodes write_register_mul(Register start_reg, uint8_t* values, uint8_t size); 
};
