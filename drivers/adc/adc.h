#pragma once
#include <stdint.h>
#include <stddef.h>
#include "../../api/adc_base.h"
#include "stm32h7xx_hal.h"

extern SPI_HandleTypeDef hspi1;

#define SPI_READ(dst, size) HAL_SPI_Receive(&hspi1, (uint8_t *)dst , size, HAL_MAX_DELAY)
#define SPI_WRITE(src, size) HAL_SPI_Transmit(&hspi1, (uint8_t *)src, size, HAL_MAX_DELAY)

class ADC_MCU_Driver : public ADC_Base_Driver {
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

    ADC_Base_Driver::ErrorCodes init(void) override;
    ADC_Base_Driver::ErrorCodes set_channel(ADC_Base_Driver::ChannelID channel) override;
    ADC_Base_Driver::ErrorCodes data_read(int32_t *result, uint8_t *status) override;
    uint8_t build_command(Commands C, uint8_t MUL, Register A);
    ADC_Base_Driver::ErrorCodes read_register(const Register reg, uint8_t* value);
    ADC_Base_Driver::ErrorCodes write_register(Register reg, uint8_t value);
    ADC_Base_Driver::ErrorCodes read_register_mul(const Register start_reg, uint8_t* buf, uint8_t size);
    ADC_Base_Driver::ErrorCodes write_register_mul(Register start_reg, uint8_t* values, uint8_t size); 
};
