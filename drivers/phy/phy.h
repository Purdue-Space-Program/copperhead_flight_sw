#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "stm32h7xx_hal_conf.h"

#define LAN8742A_PHY_ADDRESS 0x01 
#define mdio_read(addr, reg) 0 
#define mdio_write(addr, reg, value) 0

#define PHY_CONTROL_REGISTER 0x00 
#define PHY_STATUS_REGISTER 0x01 
#define PHY_AUTO_NEGOTIATION_REGISTER 0x04

typedef enum {
    PHY_CTRL_SOFT_RESET                 = 15,
    PHY_CTRL_LOOPBACK                   = 14, 
    PHY_CTRL_SPEED_SELECT               = 13,
    PHY_CTRL_AUTO_NEGOTIATION_ENABLE    = 12,
    PHY_CTRL_POWER_DOWN                 = 11,
    PHY_CTRL_ISOLATE                    = 10,
    PHY_CTRL_RESTART_AUTO_NEGOTIATION   = 9,
    PHY_CTRL_DUPLEX_MODE                = 8
}PHY_MCU_ControlRegister;


typedef enum {
    PHY_STATUS_100BASE_T4               = 15,
    PHY_STATUS_100BASE_TX_FULL_DUPLEX   = 14,
    PHY_STATUS_100BASE_TX_HALF_DUPLEX   = 13,
    PHY_STATUS_10BASE_T_FULL_DUPLEX     = 12,
    PHY_STATUS_10BASE_T_HALF_DUPLEX     = 11,
    PHY_STATUS_100BASE_T2_FULL_DUPLEX   = 10,
    PHY_STATUS_100BASE_T2_HALF_DUPLEX   = 9,
    PHY_STATUS_EXTENDED_STATUS          = 8,
    PHY_STATUS_RESERVED_7               = 7,
    PHY_STATUS_RESERVED_6               = 6,
    PHY_STATUS_AUTO_NEGOTIATE_COMPLETE  = 5,
    PHY_STATUS_REMOTE_FAULT             = 4,
    PHY_STATUS_AUTO_NEGOTIATE_ABILITY   = 3,
    PHY_STATUS_LINK_STATUS              = 2,
    PHY_STATUS_JABBER_DETECTED          = 1,
    PHY_STATUS_EXTENDED_CAPABILITIES    = 0
}PHY_MCU_StatusRegister;


typedef enum {
    PHY_ANEG_100BASE_TX_FULL_DUPLEX = 8, 
    PHY_ANEG_100BASE_TX             = 7, 
    PHY_ANEG_10BASE_T_FULL_DUPLEX   = 6,
    PHY_ANEG_10BASE_T               = 5
}PHY_MCU_ANEG_ADV_Register;


typedef enum {
    PHY_OK                      = 0,
    PHY_ERR_INIT                = -1,
    PHY_ERR_READ_STATUS         = -2,
    PHY_ERR_READ_REG            = -3,
    PHY_ERR_READ_MODE_ERROR     = -4,
    PHY_ERR_WRITE_REG           = -5,
    PHY_ERR_APPLY_MODE          = -6,
    PHY_ERR_MDIO_READ           = 0xFFFF
}PHY_Error;


class PHY_Driver {
public:
    virtual ~PHY_Driver = default;
    virtual PHY_Error phy_init(void) = 0;
    virtual PHY_Error phy_read_mode(uint8_t *mode) = 0;
    virtual PHY_Error phy_apply_mode(const uint8_t mode) = 0;
};

class PHY_SIM_Driver : public PHY_Driver {
public:
    PHY_Error phy_init(void) override;
    PHY_Error phy_read_mode(uint8_t *mode) override;
    PHY_Error phy_apply_mode(const uint8_t mode) override;
};

class PHY_MCU_Driver : public PHY_Driver {
    PHY_Error phy_init(void) override;
    PHY_Error phy_read_register(uint8_t reg_addr, uint16_t *value);
    PHY_Error phy_write_register_bit(const uint8_t reg_addr, uint8_t bit_pos, bool set_bit);
    PHY_Error phy_low_level_write(uint8_t reg_addr, uint16_t value);
    PHY_Error phy_read_status(uint16_t *status);
    PHY_Error phy_read_mode(uint8_t *mode) override;
    PHY_Error phy_apply_mode(uint8_t mode) override;
};
