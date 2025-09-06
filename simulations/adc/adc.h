/**
 * @file adc.h
 * @brief ADC simulation driver
 * @date 2025-09-03
 */

#include "adc_base.h"


/**
 * @class ADC_SIM_Driver derived from the ADC_Base_Driver class
 * @brief Simulated driver for the ADS1158
 * @note Only builds when the environment is set to "Native"
 */

class ADC_SIM_Driver : public ADC_Base_Driver {
public:
    /**
     * @brief Initializes the simulated ADC, overrides the base function
     * @return Error code telling us whether or not the function worked 
     */
    etl::expected<void, ErrorCodes> init(void) override;

    /**
     * @brief Sets the channel of the simulated ADC, overrides the base function
     * @param channel ChannelID so we know what channel to set to look at
     * @return Error code telling us whether or not the function worked
     */

    etl::expected<void, ErrorCodes> set_channel(ChannelID channel) override;

    /**
     * @brief Reads the data from the simulated ADC, overrides the base function
     * @param result int32_t pointer which contains the result data from the simulated ADC
     * @param status uint8_t pointer which contains the status data from the simulated ADC
     * @return Error code telling us whether or not the function worked
     */

    etl::expected<uint16_t, ErrorCodes> data_read(void) override;
};
