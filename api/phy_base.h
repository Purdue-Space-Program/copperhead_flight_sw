#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

class PHY_Base_Driver {
public:
    
    enum class ErrorCodes {
        PHY_OK                      = 0,
        PHY_ERR_INIT                = -1,
        PHY_ERR_READ_STATUS         = -2,
        PHY_ERR_READ_REG            = -3,
        PHY_ERR_READ_MODE_ERROR     = -4,
        PHY_ERR_WRITE_REG           = -5,
        PHY_ERR_APPLY_MODE          = -6,
        PHY_ERR_MDIO_READ           = 0xFFFF
    };

    virtual ~PHY_Base_Driver() = default;
    virtual ErrorCodes init(void) = 0;
    virtual ErrorCodes read_mode(uint8_t *mode) = 0;
    virtual ErrorCodes apply_mode(const uint8_t mode) = 0;
};
