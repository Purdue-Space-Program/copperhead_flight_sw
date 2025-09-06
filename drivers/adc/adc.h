/**
 * @file adc.h
 * @brief ADC_MCU_Driver header file
 * @date 2025-09-03
 */

#pragma once
#include <stdint.h>
#include <stddef.h>
#include "adc_base.h"
#include "stm32h7xx_hal.h"

/**
 * @brief Expects the user to figure out which spi peripheral they want to communicate with BEFORE
 * calling this function
 */

extern SPI_HandleTypeDef hspi1;

/**
 * @brief Macro because I did not feel like changing the code.
 */

#define SPI_READ(dst, size) HAL_SPI_Receive(&hspi1, (uint8_t *)dst , size, HAL_MAX_DELAY)

/**
 * @brief Macro because I did not feeling like changing the code
 */

#define SPI_WRITE(src, size) HAL_SPI_Transmit(&hspi1, (uint8_t *)src, size, HAL_MAX_DELAY)

/**
 * @class ADC_MCU_Driver derived from ADC_Base_Driver
 * @brief Driver for the ADS1158
 * @note Will only build when the environment is set to "STM32H730"
 */

class ADC_MCU_Driver : public ADC_Base_Driver {
public:
    
    /**
     * @enum Commands
     * @brief Organizes command names and values to call such command
     */

    enum class Commands {
        CMD_DATA_READ = 0x01,
        CMD_RREG = 0x02,
        CMD_WREG = 0x03,
        CMD_RST = 0x06,     
    };

    /**
     * @enum Register
     * @brief Organizes the Register names and addresses, no magic numbers
     */
    
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

    /**
     * @brief Initialized the ADS1158, overrides the base function
     * @return Error code so we know whether or not the function worked
     */

    etl::expected<void, ADC_Base_Driver::ErrorCode> init(void) override;

    /**
     * @brief Sets the channel for the ADS1158, overrides the base function
     * @param channel ChannelID variable to set the current channel to
     * @return Error code so we know whether or not the function worked
     */

    etl::expected<void, ADC_Base_Driver::ErrorCode> set_channel(ADC_Base_Driver::ChannelID channel) override;

    /**
     * @brief Reads the data from the register, overrides the base function
     * @param result int32_t pointer to copy result data into
     * @param status uint8_t pointer to copy the status data into
     * @return Error code so we know whether or not the function worked
     */

    etl::expected<uint16_t, ADC_Base_Driver::ErrorCode> data_read(void) override;
    
    /**
     * @brief Builds a command, makes functions 10x easier to implement and easier to use commands
     * @param C Commands type which takes in the specific command you want to use
     * @param MUL uint8_t type which asks whether or not you want to use the multiplexer
     * @param A Register type which finishes off the rest of the command byte
     * @return Error code so we know whether or not the function worked 
     */

    uint8_t build_command(Commands C, uint8_t MUL, Register A);
    
    /**
     * @brief Reads a register
     * @param reg const Register which specifies which register to read from
     * @param value uint8_t pointer to copy the value of the register into
     * @return Error code so we know whether or not the function worked
     */

    etl::expected<void, ADC_Base_Driver::ErrorCode> read_register(const Register reg, uint8_t* value);
    
    /**
     * @brief Writes to a register
     * @param reg Register type to write into
     * @param value The value to write into the register
     * @return Error code so we know whether or not the function worked
     */

    etl::expected<void, ADC_Base_Driver::ErrorCode> write_register(Register reg, uint8_t value);

    /**
     * @brief Reads multiple registers
     * @param start_reg const Register which is the first register we want to read from
     * @param buf uint8_t pointer buffer to copy the values into from each register (array)
     * @param size uint8_t however many registers we want to read from
     * @return Error code so we know whether or not the function worked
     */

    etl::expected<void, ADC_Base_Driver::ErrorCode> read_register_mul(const Register start_reg, uint8_t* buf, uint8_t size);
    
    /**
     * @brief Writes to multiple registers
     * @param start_reg Register which is the first register we want to write to
     * @param values uint8_t array of values to write into each register respectively
     * @param size the number of registers we want to write to
     * @return Error code so we know whether or not the function worked
     */

    etl::expected<void, ADC_Base_Driver::ErrorCode> write_register_mul(Register start_reg, uint8_t* values, uint8_t size); 
};
