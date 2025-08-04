#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define LAN8742A_PHY_ADDRESS 0x01 // Example address, adjust as needed
#define mdio_read(addr, reg) 0 // Placeholder for MDIO read function
#define mdio_write(addr, reg, value) 0 // Placeholder for MDIO read function
#define MDIO_READ_ERROR 0xFFFF // Example error value for MDIO read

//Map of the status and mode registers and their respective values
typedef struct {
    uint8_t control; // Control register address    
    uint8_t status; // Status register address
    uint8_t auto_negotiation; // Auto-negotiation register address
}PHY_Registers_t;

extern const PHY_Registers_t phy_registers;

typedef enum {
    //Basic Control Register Index (in decimal): 0, Size: 16 bits (Found on Page 67 of the datasheet)
    //7:0 are reserved, 15:8 are RW bits, 15 and 9 are both SC (self-clearing)
    Control_SOFT_RESET = 15, //0x8000, 15th bit, 1 for soft reset, bit is self-clearing
    //Default value is 0b
    Control_LOOPBACK = 14, //0x4000, 14th bit, 1 for loopback mode, 0 for normal operation
    //Default value is 0b
    Control_SPEED_SELECT = 13, //0x2000, 13th bit, 1 for 100mbps, 0 for 10mbps
    //Default value is dependent on the mode
    Control_AUTO_NEGOTIATION_ENABLE = 12, //0x1000, 12th bit, 1 for auto negotiation enabled, 0 for disabled
    //Default value is dependent on the mode
    Control_POWER_DOWN = 11, //0x0800, 11th bit, 1 for power down mode, 0 for normal operation
    //Default value is 0b
    Control_ISOLATE = 10, //0x0400, 10th bit, 1 for isolate mode, 0 for normal operation
    //Default value is 0b
    Control_RESTART_AUTO_NEGOTIATION = 9, //0x0200, 9th bit, 1 for restart auto negotiation, 0 for no restart
    //Default value is 0b
    Control_DUPLEX_MODE = 8, //0x0100, 8th bit, 1 for full duplex mode, 0 for half duplex mode
    //Default value is dependent on the mode, which is then dependent on Registers 0 and 4
}Basic_Control_Register_t;

typedef enum {
    // Basic Status Register Index (in decimal): 1, Size: 16 bits (Found on Page 68 of the datasheet)
    //RO bits 15:1, 4 and 1 has LH, 2 has LL
    Status_100BASE_T4 = 15, //1 for enabled, 0 for disabled
    //Default value is 0b
    Status_100BASE_TX_Full_Duplex = 14, //1 for enabled 100mbps full duplex, 0 for disabled
    //Default value is 1b
    Status_100BASE_TX_Half_Duplex = 13, //1 for enabled 100mbps half duplex, 0 for disabled
    //Default value is 1b
    Status_10BASE_T_Full_Duplex = 12, //1 for enabled 10mbps full duplex, 0 for disabled
    //Default value is 1b
    Status_10BASE_T_Half_Duplex = 11, //1 for enabled 10mbps half duplex, 0 for disabled
    //Default value is 1b
    Status_100BASE_T2_Full_Duplex = 10, //1 to be able to perform full duplex 100BASE-T2, 0 for disabled
    //Default value is 0b
    Status_100BASE_T2_Half_Duplex = 9, //1 to be able to perform half duplex 100BASE-T2, 0 for disabled
    //Default value is 0b
    Extended_Status = 8, //1 extended status register information in register 15, 0 for no extended status information in register 15
    //Default value is 0b
    Reserved_7 = 7, //reserved
    Reserved_6 = 6, //reserved
    Auto_Negotiate_Complete = 5 , //1 for auto negotiation complete, 0 for not complete
    //Default value is 0b
    Remote_FAULT = 4, //1 for remote fault detected, 0 for no remote fault
    //Default value is 0b
    Auto_Negotiate_Ability = 3, //1 for auto negotiation ability, 0 for no auto negotiation ability
    //Default value is 1b
    Link_Status = 2, //1 for link status up, 0 for link status down
    //Default value is 0b
    Jabber_Detected = 1, //1 for jabber detected, 0 for no jabber detected
    //Default value is 0b
    Extended_Capabilities = 0, //1 for supported extended capabilities registers, 0 no supported extended capabilities registers
    //Default value is 1b
}Basic_Status_Register_t;

typedef enum {
    //Auto-negotiation Advertisement Register (in decimal): 4, Size 16 bits (Found on Page 72 of the datasheet)
    //This enum is needed to set the mode of the PHY
    //Bits 8:5 are R/W
    AUTO_NEG_100BASE_TX_Full_Duplex = 8, //1 for TX full duplex, 0 for no full duplex ability
    AUTO_NEG_100BASE_TX = 7, //1 for TX ability, 0 for no TX ability
    AUTO_NEG_10BASE_T_Full_Duplex = 6, //1 for 10 mbps full duplex, 0 for no 10mbps full duplex ability
    AUTO_NEG_10BASE_T = 5, //1 for 10 mbps ability, 0 for no 10mbps ability
}Auto_Negotiation_Register_t;

//Just a list of error codes for the PHY driver
typedef enum {
    PHY_OK = 0,
    PHY_INIT_ERROR = -1,
    PHY_READ_STATUS_ERROR = -2,
    PHY_READ_REGISTER_ERROR = -3,
    PHY_READ_MODE_ERROR = -4,
    PHY_WRITE_REGISTER_ERROR = -5,
    PHY_APPLY_MODE_ERROR = -6,
}Errors_t;

int8_t phy_init(void);
int8_t phy_read_register(uint8_t reg_addr, uint16_t *value);
int8_t phy_write_register_bit(const uint8_t reg_addr, uint8_t bit_pos, bool set_bit);
int8_t phy_low_level_write (uint8_t reg_addr, uint16_t value);
int8_t phy_read_status(uint16_t *status);
int8_t phy_read_mode(uint8_t *mode);
int8_t phy_apply_mode(uint8_t mode);
