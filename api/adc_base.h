/**
 * @file adc_base.h
 * @brief Contains the base class for the ADC_Driver
 * @date 2025-09-03
 */

#pragma once
#include <stdint.h>
#include <stddef.h>
#include <etl/expected.h>

/**
 * @class ADC_Base_Driver
 * @brief The API for the ADC Drivers
 * @details This is class will allow us to use the exact same code,
 * no matter wheter we use STM32H730 environment or the Native environment
 * we can change the enviroment when setting the build target
 */

class ADC_Base_Driver {
public:
    /**
     * @enum ErrorCodes
     * @brief Puts the error codes neatly so we know what error we have 
     */
    enum class ErrorCodes {
        ADS_OK,
        ADS_ERR,
        ADS_RSPI_ERR,
        ADS_WSPI_ERR,
    };

    /**
     * @enum ChannelID
     * @brief Puts the channel names and IDs so it is organized neatly, no magic numbers
     */
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

    virtual ~ADC_Base_Driver() = default;
    
    /**
     * @brief Initialization function for the ADC_Driver
     * @return Error Codes telling us whether or not the function worked
     */

    virtual etl::expected<void, ErrorCodes> init(void) = 0;

    /**
     * @brief Sets the current channel
     * @param channel ChannelID, so we know what channel we are setting the current channel to
     * @return Error codes so we know whether or not the function worked
     */

    virtual etl::expected<void, ErrorCodes> set_channel(ChannelID channel) = 0;

    /**
     * @brief Reads the data from the ADC
     * @param result int32_t pointer to copy the result from the read into
     * @param status int8_t pointer to copy the status byte into
     * @note At first looking at the parameters it seems kind of weird, however when looking at the data sheet
     * you see that data is given in 32bits total, 1 byte for status, 3 bytes for actual data
   */
    virtual etl::expected<uint16_t, ErrorCodes> data_read(void) = 0;
};
