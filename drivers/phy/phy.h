/**
 * @file phy.h
 * @brief PHY_MCU_Driver header file, data sheet: https://ww1.microchip.com/downloads/en/DeviceDoc/8742a.pdf
 * @date 2025-09-03
 */

#include "phy_base.h"
#include "stm32h7xx_hal.h"

/**
 * @brief Assumes that user will handle the ethernet peripheral outside of this class
 */
extern ETH_HandleTypeDef heth1;

/**
 * @brief MDIO Address for the PHY, this define is a place holder because
 * the address is not available on the data sheet, we need to look for it
 * ourselves
 */

#define MDIO_LAN8742A_PHY_ADDRESS 0x01 

/**
 * @brief Macro because I did not feel like changing my code
 */

#define mdio_read(addr, reg, value_ptr) \
    HAL_ETH_ReadPHYRegister(&heth1, (addr), (reg), (value_ptr))

/**
 * @brief Macro because I did not feel like changing my code
 */

#define mdio_write(addr, reg, value) \
    HAL_ETH_WritePHYRegister(&heth1, (addr), (reg), (uint16_t)(value))

/**
 * These are on page 66 of the data sheet
 */

#define PHY_CONTROL_REGISTER 0x00 
#define PHY_STATUS_REGISTER 0x01 
#define PHY_AUTO_NEGOTIATION_REGISTER 0x04

/**
 * @class PHY_MCU_Driver derived from PHY_Base_Driver
 * @brief PHY Driver for the LAN8742A 
 * @note Will only build if the target environment is set to "STM32H730"
 */

class PHY_MCU_Driver : public PHY_Base_Driver {
public:
    
    /**
     * @enum ControlRegister
     * @brief Control Register position numbers
     * @note Page 67 of the data sheet
     */     
    
    enum class ControlRegister {
        PHY_CTRL_SOFT_RESET                 = 15,
        PHY_CTRL_LOOPBACK                   = 14, 
        PHY_CTRL_SPEED_SELECT               = 13,
        PHY_CTRL_AUTO_NEGOTIATION_ENABLE    = 12,
        PHY_CTRL_POWER_DOWN                 = 11,
        PHY_CTRL_ISOLATE                    = 10,
        PHY_CTRL_RESTART_AUTO_NEGOTIATION   = 9,
        PHY_CTRL_DUPLEX_MODE                = 8,
    };

    /**
     * @enum StatusRegister
     * @brief Status Register position numbers
     * @note Page 68 of the data sheet
     */

    enum class StatusRegister {
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
    };

    /**
     * @enum ANEG_ADV_Register
     * @brief Autonegotiation advertisement register position numbers
     * @note Page 72 of the data sheet
     */
    
     enum class ANEG_ADV_Register {
        PHY_ANEG_100BASE_TX_FULL_DUPLEX = 8, 
        PHY_ANEG_100BASE_TX             = 7, 
        PHY_ANEG_10BASE_T_FULL_DUPLEX   = 6,
        PHY_ANEG_10BASE_T               = 5
    };
    
    /**
     * @brief Initializes the LAN8742A, overrides the base function
     * @return Error code so we know whether or not the function worked
     */

    PHY_Base_Driver::ErrorCodes init(void) override;

    /**
     * @brief Reads a register for the LAN8472A
     * @param reg_addr The address of the register to read
     * @param value variable to copy the value of the register into
     */

    PHY_Base_Driver::ErrorCodes read_register(const uint8_t reg_addr, uint16_t *value);
    
    /**
     * @brief Writes to a register bit (only one bit of the register)
     * @param reg_addr address of the register to write to
     * @param bit_pos position of the bit to write to 
     * @param set_bit bool for whether the bit is equal to 1 or 0
     * @return Error codes so we know whether or not the function worked 
     */

    PHY_Base_Driver::ErrorCodes write_register_bit(uint8_t reg_addr, uint8_t bit_pos, bool set_bit);

    /**
     * @brief Helper variable for the write_register_bit function
     * @param reg_addr address of the register to write to
     * @param value to value to write to said register
     * @return Error codes so we know whether or not the function worked
     */

    PHY_Base_Driver::ErrorCodes low_level_write(uint8_t reg_addr, uint16_t value);

    /**
     * @brief Reads the current status of the LAN8742A
     * @param status pointer to a variable to copy the status value into
     * @return Error codes so we know whether or not the function worked
     */

    PHY_Base_Driver::ErrorCodes read_status(uint16_t *status);

    /**
     * @brief Reads the current status of the LAN8742A, overrides the base function
     * @param mode pointer to copy the current mode into
     * @return Error code so we know whether or not the function worked
     */

    PHY_Base_Driver::ErrorCodes read_mode(uint8_t *mode) override;

    /**
     * @brief Applies a mode to the LAN8742A, overrides the base function
     * @param mode the mode we want to set the PHY to
     * @return Error code so we know whether or not the function worked
     */
    PHY_Base_Driver::ErrorCodes apply_mode(uint8_t mode) override;
};
