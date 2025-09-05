/**
 * @file phy_base.h
 * @brief PHY_Base_Driver class header file
 * @date 2025-09-03
 */

#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @class PHY_Base_Driver
 * @brief API for the PHY Drivers
 * @details This class will allow us to use the same code in any environment
 * we can change the environment by assigning the build target to "STM32H730"
 * or "Native"
 */

class PHY_Base_Driver {
public:
    /**
     * @enum ErrorCodes
     * @brief Provides error codes so there are no magic numbers
     */    
    enum class ErrorCodes {
        PHY_OK                      = 0,
        PHY_ERR_INIT                = -1,
        PHY_ERR_READ_STATUS         = -2,
        PHY_ERR_READ_REG            = -3,
        PHY_ERR_READ_MODE_ERROR     = -4,
        PHY_ERR_WRITE_REG           = -5,
        PHY_ERR_APPLY_MODE          = -6,
        PHY_ERR_MDIO_READ           = 0xFFFF,
        NULL_PTR                    = -7
    };

    virtual ~PHY_Base_Driver() = default;
    /**
     * @brief Initialization function for the PHY driver
     * @return Error code so we know whether or not the function worked
     */
    virtual ErrorCodes init(void) = 0;

    /**
     * @brief Read the current mode of the PHY
     * @param mode uint8_t pointer to copy the current mode into
     * @return Error code so we know whether or not the function worked
     */
    virtual ErrorCodes read_mode(uint8_t *mode) = 0;

    /**
     * @brief Applies a mode to the PHY
     * @param mode const uint8_t mode to apply to the PHY
     * @return Error code so we know whether or not the function worked
     */
    virtual ErrorCodes apply_mode(const uint8_t mode) = 0;
};
