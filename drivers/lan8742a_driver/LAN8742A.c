#include "LAN8742A.h"

const PHY_Registers_t phy_registers = {
    .control = 0x00, // Control register address
    .status = 0x01, // Status register address
    .auto_negotiation = 0x04 // Auto-negotiation register address
};

int8_t phy_init(void){
    // Initialize the PHY by performing a soft reset
    if (phy_write_register_bit(phy_registers.control, Control_SOFT_RESET, true) != PHY_OK) {
        return PHY_INIT_ERROR; // If the reset fails, return an error.
    }
    return PHY_OK;
}

// This function reads a specific register value from the PHY.
int8_t phy_read_register(uint8_t reg_addr, uint16_t *value){
    if (value == NULL) return PHY_READ_REGISTER_ERROR; //If the register value pointer is NULL, return an error.
    //Validate the register address
    switch (reg_addr) {
        case 0x00: // Control register
        case 0x01: // Status register
        case 0x04: // Auto-negotiation register
            break; // Valid registers, continue.
        default:
            return PHY_READ_REGISTER_ERROR; // Invalid register address, return error.
    }

    uint16_t val = mdio_read(LAN8742A_PHY_ADDRESS, reg_addr); // Read the register value using the mdio_read function.
    
    if (val == MDIO_READ_ERROR) return PHY_READ_REGISTER_ERROR; // If the read fails, return an error.
    
    *value = val; // If the read is successful, set the value to the pointer provided.
    
    return PHY_OK; // Return PHY_OK to indicate success.
}


//Write a specific bit value to a register on PHY. MDIO only allows for writing the entire register at once, 
//so we have to copy the register value, modify the bit we want to change, and then write the entire register back.
int8_t phy_write_register_bit(const uint8_t reg_addr, uint8_t bit_pos, bool set_bit){
    if (bit_pos > 15) return PHY_WRITE_REGISTER_ERROR; // If the bit position is out of range, return an error.
    
    uint16_t reg_value;

    // Read the current value of the register
    if (phy_read_register(reg_addr, &reg_value) != PHY_OK) return PHY_WRITE_REGISTER_ERROR; // If reading the register fails, return an error.

    if (set_bit){
        reg_value |= (1 << bit_pos); // Set the specified bit to 1.
    } else {
        reg_value &= ~(1 << bit_pos); // Clear the specified bit to 0.
    }

    return phy_low_level_write(reg_addr, reg_value); // Write the modified value back to the register.
}

//Low level writing to the entire register, this is because the MDIO interface requires the entire register to be written at once
//So what this function does is write the entire register value to the PHY after only changing the bits that need to be changed
int8_t phy_low_level_write (uint8_t reg_addr, uint16_t value){
    if (mdio_write(LAN8742A_PHY_ADDRESS, reg_addr, value) != PHY_OK){
        return PHY_WRITE_REGISTER_ERROR; // If writing the register fails, return an error.
    }
    return PHY_OK; // Return PHY_OK to indicate success.
}

int8_t phy_read_status(uint16_t *status){
    /*
    Read the status registers and populate the status structure.
    */
    if (status == NULL) return PHY_READ_STATUS_ERROR; // If the status pointer is NULL, return an error.
    return phy_read_register(phy_registers.status, status); // Read the status register and return the result.
}

int8_t phy_read_mode(uint8_t *mode){
    /*
    Read the PHY mode registers and populate the mode structure.
    */
   uint16_t control_reg, auto_neg_reg;
   if (phy_read_register(phy_registers.control, &control_reg) != PHY_OK)
       return PHY_READ_REGISTER_ERROR; // If reading the control register fails, return an error.
   if (phy_read_register(phy_registers.auto_negotiation, &auto_neg_reg) != PHY_OK)
       return PHY_READ_REGISTER_ERROR; // If reading the auto-negotiation register fails, return an error.
   uint8_t remapped_bits = 0;
   // Remap the bits from the control and auto-negotiation registers to the mode structure.
   remapped_bits |= ((control_reg >> 13) & 0x01) << 3;
    remapped_bits |= ((control_reg >> 12) & 0x01) << 2;
    remapped_bits |= ((control_reg >> 10) & 0x01) << 1;
    remapped_bits |= ((control_reg >> 8) & 0x01) << 0;
    control_reg = remapped_bits; // Store the remapped bits in the control register variable.
    remapped_bits = 0;
    remapped_bits |= ((auto_neg_reg >> 8) & 0x01) << 3;
    remapped_bits |= ((auto_neg_reg >> 7) & 0x01) << 2;
    remapped_bits |= ((auto_neg_reg >> 6) & 0x01) << 1;
    remapped_bits |= ((auto_neg_reg >> 5) & 0x01) << 0;
    auto_neg_reg = remapped_bits; // Store the remapped bits in the auto-negotiation register variable.
    *mode = (control_reg << 4) | (auto_neg_reg & 0x0F); // Combine the control and auto-negotiation registers into the mode variable.
    return PHY_OK; // Return PHY_OK to indicate success.
}

//Given that mode is an 8 bit value where bits 7:4 are control bits and bits 3:0 are auto-negotiation bits,
int8_t phy_apply_mode(uint8_t mode) {
    // Arrays of registers and bit positions corresponding to each mode bit
    const uint16_t regs[] = {
        phy_registers.control,       // bits 7..4 map here
        phy_registers.control,
        phy_registers.control,
        phy_registers.control,
        phy_registers.auto_negotiation, // bits 3..0 map here
        phy_registers.auto_negotiation,
        phy_registers.auto_negotiation,
        phy_registers.auto_negotiation
    };

    const uint8_t bits[] = {
        13, 12, 10, 8,   // control register bits for mode bits 7..4
        8, 7, 6, 5       // auto-negotiation register bits for mode bits 3..0
    };
    // Iterate through each bit of the mode and write it to the corresponding register
    for (int i = 0; i < 8; i++) {
        uint8_t bit_val = (mode >> (7 - i)) & 0x01;
        if (phy_write_register_bit(regs[i], bits[i], bit_val) != PHY_OK) {
            return PHY_APPLY_MODE_ERROR;
        }
    }
    return PHY_OK;
}