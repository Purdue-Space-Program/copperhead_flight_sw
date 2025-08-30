/**
 * @file lan8742a.h
 * @brief Header for driver of the LAN8742A Ethernet PHY
 * @author Pradyunn Kale
 * @date 2025-08-03
 */

#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define LAN8742A_PHY_ADDRESS 0x01 
#define mdio_read(addr, reg) 0 
#define mdio_write(addr, reg, value) 0

#define PHY_CONTROL_REGISTER 0x00 
#define PHY_STATUS_REGISTER 0x01 
#define PHY_AUTO_NEGOTIATION_REGISTER 0x04

/**
 * @brief Map of the basic control registers and their respective values
 * @details This structure holds the register addresses for the LAN8742A PHY.
 * 7:0 are reserved, 15:8 are RW bits, 15 and 9 are both SC (self-clearing).
 * These are found on Page 67 of the datasheet. Default values for all registers are 0b,
 * except for PHY_CTRL_SPEED_SELECT, PHY_CTRL_AUTO_NEGOTIATION_ENABLE, and PHY_CTRL_DUPLEX_MODE,
 * which are dependent on the mode of the PHY.
 */
typedef enum {
    PHY_CTRL_SOFT_RESET                 = 15,
    PHY_CTRL_LOOPBACK                   = 14, 
    PHY_CTRL_SPEED_SELECT               = 13,
    PHY_CTRL_AUTO_NEGOTIATION_ENABLE    = 12,
    PHY_CTRL_POWER_DOWN                 = 11,
    PHY_CTRL_ISOLATE                    = 10,
    PHY_CTRL_RESTART_AUTO_NEGOTIATION   = 9,
    PHY_CTRL_DUPLEX_MODE                = 8
}phy_control_reg_t;

/**
 * @brief Map of the basic status registers and their respective values
 * @details This structure holds the register addresses for the LAN8742A PHY.
 * 15:1 are RO bits, 4 and 1 has LH, 2 has LL. Default values for all registers are 0b,
 * except for PHY_STATUS_100BASE_TX_Full_Duplex, PHY_STATUS_100BASE_TX_Half_Duplex,
 * PHY_STATUS_10BASE_T_Full_Duplex, and PHY_STATUS_10BASE_T_Half_Duplex, and Link Status,
 * and Extended Capabilities, which are all 1b. These are found on Page 68 of the datasheet. 
 */
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
}phy_status_reg_t;

/**
 * @brief Map of the Auto-negotiation Advertisement registers and their respective values
 * @details This structure holds the register addresses for the LAN8742A PHY. Bits 8:5 are R/W.
 *  This enum is needed to set the mode of the PHY. These are found on Page 72 of the datasheet.
 * 
 */
typedef enum {
    PHY_ANEG_100BASE_TX_FULL_DUPLEX = 8, 
    PHY_ANEG_100BASE_TX             = 7, 
    PHY_ANEG_10BASE_T_FULL_DUPLEX   = 6,
    PHY_ANEG_10BASE_T               = 5
}phy_aneg_adv_reg_t;

/**
 * @brief Error codes for the PHY driver
 * @details These error codes are used to indicate the status of various operations in the PHY driver
 * such as initialization, reading, writing, and applying modes.
 */
typedef enum {
    PHY_OK                      = 0,
    PHY_ERR_INIT                = -1,
    PHY_ERR_READ_STATUS         = -2,
    PHY_ERR_READ_REG            = -3,
    PHY_ERR_READ_MODE_ERROR     = -4,
    PHY_ERR_WRITE_REG           = -5,
    PHY_ERR_APPLY_MODE          = -6,
    PHY_ERR_MDIO_READ           = 0xFFFF
}phy_error_t;

int8_t phy_init(void);
int8_t phy_read_register(uint8_t reg_addr, uint16_t *value);
int8_t phy_write_register_bit(const uint8_t reg_addr, uint8_t bit_pos, bool set_bit);
int8_t phy_low_level_write (uint8_t reg_addr, uint16_t value);
int8_t phy_read_status(uint16_t *status);
int8_t phy_read_mode(uint8_t *mode);
int8_t phy_apply_mode(uint8_t mode);
