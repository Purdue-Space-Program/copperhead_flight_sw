#pragma once
#include <stdint.h>
#include <stddef.h>
#include "stm32h7xx_hal.h"

#define SPI_READ (dst, size) HAL_SPI_Receive(&hspi1, (uint8_t *)src, size, HAL_MAX_DELAY)
#define SPI_WRITE (src, size) HAL_SPI_Transmit(&hspi1, (uint8_t *)src, size, HAL_MAX_DELAY)

class ADC_Driver {
public:

    typedef enum {
        ADS_OK,
        ADS_ERR,
        ADS_RSPI_ERR,
        ADS_WSPI_ERR,
    } ADC_ErrorCodes;

    virtual ~ADC_Driver() = default;
    virtual ADC_ErrorCodes ads_init(void) = 0;
    virtual ADC_ErrorCodes ads_set_channel(ADC_SIM_ChannelID channel) = 0;
    virtual ADC_ErrorCodes ads_data_read(int32_t *result, uint8_t status) = 0;
};

class ADC_SIM_Driver : public ADC_Driver {
public:
    
    typedef enum {
    
    } ADC_SIM_ChannelID;

    ADC_ErrorCodes ads_init(void) override;
    ADC_ErrorCodes ads_set_channel(ADC_MCU_ChannelID channel) override;
    ADC_ErrorCodes ads_data_read(int32_t *result, uint8_t status) override;
};

class ADC_MCU_Driver : public ADC_Driver {
public:

    typedef enum {
        CMD_DATA_READ = 0x01,
        CMD_RREG = 0x02,
        CMD_WREG = 0x03,
        CMD_RST = 0x06,     
    } ADC_MCU_Commands;

    typedef enum {
        CONFIG0 = 0x00,
        CONFIG1 = 0x01,
        MUXSCH 	= 0x02,
        MUXDIF 	= 0x03,
        MUXSG0 	= 0x04,
        MUXSG1 	= 0x05,
        SYSRED 	= 0x06,
        GPIOC 	= 0x07,
        GPIOD 	= 0x08,
        ID 		= 0x09,
    } ADC_MCU_Register;

    typedef enum {
        ADS_CH_DIFF0 = 0x00,
        ADS_CH_DIFF1 = 0x01,
        ADS_CH_DIFF2 = 0x02,
        ADS_CH_DIFF3 = 0x03,
        ADS_CH_DIFF4 = 0x04,
        ADS_CH_DIFF5 = 0x05,
        ADS_CH_DIFF6 = 0x06,
        ADS_CH_DIFF7 = 0x07,
    } ADC_MCU_ChannelID;

    ADC_ErrorCodes ads_init(void) override;
    ADC_ErrorCodes ads_set_channel(ADC_MCU_ChannelID channel) override;
    ADC_ErrorCodes ads_data_read(int32_t *result, uint8_t status) override;
    ADC_ErrorCodes ads_build_command(uint8_t C, uint8_t MUL, ADC_MCU_Register A);
    ADC_ErrorCodes ads_write_register(ADC_MCU_Register reg, uint8_t value);
    ADC_ErrorCodes ads_read_register_mul(const ADC_MCU_Register start_reg, uint8_t* buf, uint8_t size);
    ADC_ErrorCodes ads_write_register_mul(ADC_MCU_Register start_reg, uint8_t* values, uint8_t size); 
};
