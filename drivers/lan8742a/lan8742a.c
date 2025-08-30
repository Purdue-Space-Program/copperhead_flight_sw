/**
 * @file lan8742a.c
 * @brief Implementation for driver of the LAN8742A Ethernet PHY
 * @author Pradyunn Kale
 * @date 2025-08-03
*/
#include "lan8742a.h"

/**
 * @brief Initializes the LAN8742A Ethernet PHY.
 * 
 * @details The function performs a soft reset of the LAN8742A PHY by writing to the control register.
 * The soft reset is done by setting the Control_SOFT_RESET bit in the Basic Control Register.
 * Found on page 67 under "Basic Control Register Index".
 * 
 * @return int8_t Returns PHY_OK on success, or an error code on failure.
 */
int8_t phy_init(void){
    if (phy_write_register_bit(PHY_CONTROL_REGISTER, PHY_CTRL_SOFT_RESET, true) != PHY_OK)
        return PHY_ERR_INIT;
    return PHY_OK;
}

/**
 * @brief Reads a specific register from the LAN8742A PHY.
 * 
 * @details The function reads the value of the specified register using the MDIO interface.
 * It validates the register address and returns an error if the read fails.
 * 
 * @param reg_addr The address of the register to read.
 * @param value Pointer to store the read value.
 * @return int8_t Returns PHY_OK on success, or an error code on failure.
 */
int8_t phy_read_register(uint8_t reg_addr, uint16_t *value){
    if (value == NULL) return PHY_ERR_READ_REG;
    //Validate the register address
    switch (reg_addr) {
        case 0x00:
        case 0x01:
        case 0x04:
            break;
        default:
            return PHY_ERR_READ_REG;
    }
    uint16_t val = mdio_read(LAN8742A_PHY_ADDRESS, reg_addr);
    if (val == PHY_ERR_MDIO_READ) return PHY_ERR_READ_REG;    
    *value = val;
    return PHY_OK;
}

/**
 * @brief Writes a specific register bit in the LAN8742A PHY.
 * 
 * @details The function writes the specified value to the given register address using 
 * the MDIO interface. It validates the register address and returns an error if the write fails.
 * 
 * @param reg_addr The address of the register to write to.
 * @param bit_pos The bit position to write (0-15).
 * @param set_bit The value to set the bit to (true for 1, false for 0).
 * @return int8_t Returns PHY_OK on success, or an error code on failure.
 */
int8_t phy_write_register_bit(const uint8_t reg_addr, uint8_t bit_pos, bool set_bit){
    if (bit_pos > 15) return PHY_ERR_WRITE_REG;
    uint16_t reg_value;
    if (phy_read_register(reg_addr, &reg_value) != PHY_OK) return PHY_ERR_WRITE_REG;
    if (set_bit) reg_value |= (1 << bit_pos);
    else reg_value &= ~(1 << bit_pos);
    return phy_low_level_write(reg_addr, reg_value); 
}


/**
 * @brief Low-level write to a specific register in the LAN8742A PHY.
 * 
 * @details The function writes the entire register value to the PHY after only changing the bits 
 * that need to be changed. This is because the MDIO interface requires the entire register to be 
 * written at once.
 * 
 * @param reg_addr The address of the register to write to.
 * @param value The value to write to the register.
 * @return int8_t Returns PHY_OK on success, or an error code on failure.
 */
int8_t phy_low_level_write (uint8_t reg_addr, uint16_t value){
    if (mdio_write(LAN8742A_PHY_ADDRESS, reg_addr, value) != PHY_OK)
        return PHY_ERR_WRITE_REG; 
    return PHY_OK;
}

/**
 * @brief Reads the status of the LAN8742A PHY.
 * 
 * @details The function reads the status register of the PHY and stores in uint16_t pointer.
 * It checks for errors in reading the status and returns an appropriate error code if any issues are
 * detected.
 * @param status Pointer to store the read status value.
 * @return int8_t Returns PHY_OK on success, or an error code on failure.
 */
int8_t phy_read_status(uint16_t *status){
    if (status == NULL) return PHY_ERR_READ_STATUS;
    return phy_read_register(PHY_STATUS_REGISTER, status);
}


/**
 * @brief Reads the mode of the LAN8742A PHY.
 * 
 * @details The function reads the control and auto-negotiation registers to determine 
 * the current mode of the PHY. It remaps the bits from these registers to a single 8-bit value 
 * representing the mode. Where bits 7:4 are control bits and bits 3:0 are auto-negotiation bits.
 * Control bits are read from the Control Register and auto-negotiation bits are read from the
 * Auto-Negotiation Advertisement Register. These bits are 13, 12, 10, 8 for control bits and
 * 8, 7, 6, 5 for auto-negotiation bits.
 * 
 * @param mode Pointer to store the read mode value.
 * @return int8_t Returns PHY_OK on success, or an error code on failure.
 */
int8_t phy_read_mode(uint8_t *mode){
    uint16_t control_reg, auto_neg_reg;
    if (phy_read_register(PHY_CONTROL_REGISTER, &control_reg) != PHY_OK)
       return PHY_ERR_READ_REG;
    if (phy_read_register(PHY_AUTO_NEGOTIATION_REGISTER, &auto_neg_reg) != PHY_OK)
       return PHY_ERR_READ_REG;
    uint8_t remapped_bits = 0;
    const uint8_t control_positions[4] = {13, 12, 10, 8};
    for (int i = 0; i < 4; i++) {
        remapped_bits |= ((control_reg >> control_positions[i]) & 0x01) << (3 - i);
    }
    control_reg = remapped_bits;
    remapped_bits = 0;
    const uint8_t auto_neg_positions[4] = {8, 7, 6, 5};
    for (int i = 0; i < 4; i++) {
        remapped_bits |= ((auto_neg_reg >> auto_neg_positions[i]) & 0x01) << (3 - i);
    }
    auto_neg_reg = remapped_bits;
    *mode = (control_reg << 4) | (auto_neg_reg & 0x0F);
    return PHY_OK;
}

/**
 * @brief Applies the specified mode to the LAN8742A PHY.
 * 
 * @details The function writes the mode bits to the corresponding registers in the PHY.
 * It uses the phy_write_register_bit function to set each bit in the control and auto-negotiation 
 * registers. The mode is an 8-bit value where bits 7:4 correspond to control bits and bits 3:0 
 * correspond to auto-negotiation bits.
 * 
 * @param mode The mode value to apply, represented as an 8-bit value.
 * @return int8_t Returns PHY_OK on success, or an error code on failure.
 */
int8_t phy_apply_mode(uint8_t mode) {
    const uint16_t regs[] = {
        PHY_CONTROL_REGISTER,       
        PHY_CONTROL_REGISTER,
        PHY_CONTROL_REGISTER,
        PHY_CONTROL_REGISTER,
        PHY_AUTO_NEGOTIATION_REGISTER, 
        PHY_AUTO_NEGOTIATION_REGISTER,
        PHY_AUTO_NEGOTIATION_REGISTER,
        PHY_AUTO_NEGOTIATION_REGISTER
    };
    const uint8_t bits[] = {
        13, 12, 10, 8,   
        8, 7, 6, 5       
    };
    for (int i = 0; i < 8; i++) {
        uint8_t bit_val = (mode >> (7 - i)) & 0x01;
        if (phy_write_register_bit(regs[i], bits[i], bit_val) != PHY_OK) {
            return PHY_ERR_READ_REG;
        }
    }
    return PHY_OK;
}