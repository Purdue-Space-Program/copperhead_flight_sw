/**
 * @file phy.h
 * @brief Header file for the PHY_SIM_Driver class
 * @date 2025-09-03
 */

#include "phy_base.h"

/**
 * @class PHY_SIM_Driver derived from PHY_Base_Driver
 * @brief PHY simulated driver
 * @note Will only build if the environment is set to "Native"
 */

class PHY_SIM_Driver : public PHY_Base_Driver
{
  public:
    /**
     * @brief Initializes the simualated PHY driver, overrides the base function
     * @return Error code so we know whether or not the function worked
     */

    etl::expected<void, PHY_Base_Driver::ErrorCode> init(void) override;

    /**
     * @brief Reads the current mode of the simulated PHY, overrides the base function
     * @param mode pointer copies the current mode into the variable passed
     * @return Error code so we know whether or not the function worked
     */
    etl::expected<uint16_t, PHY_Base_Driver::ErrorCode> read_mode(void) override;

    /**
     * @brief Applies a mode to the simulated PHY, overrides the base function
     * @param mode value to write as the mode for the simulated PHY
     * @return Error code so we know whether or not the function worked
     */

    etl::expected<void, PHY_Base_Driver::ErrorCode> apply_mode(const uint8_t mode) override;
};
