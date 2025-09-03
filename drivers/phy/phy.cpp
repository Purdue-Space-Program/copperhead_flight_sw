#include "phy.h"
    
PHY_Base_Driver::ErrorCodes PHY_MCU_Driver::init(void) {
    if (write_register_bit(PHY_CONTROL_REGISTER, static_cast<uint8_t>(ControlRegister::PHY_CTRL_SOFT_RESET), true) != ErrorCodes::PHY_OK)
        return ErrorCodes::PHY_ERR_INIT;
    return ErrorCodes::PHY_OK;
}

PHY_Base_Driver::ErrorCodes PHY_MCU_Driver::read_register(const uint8_t reg_addr, uint16_t *value){
    if (value == NULL) return ErrorCodes::PHY_ERR_READ_REG;
    //Validate the register address
    switch (reg_addr) {
        case 0x00:
        case 0x01:
        case 0x04:
            break;
        default:
            return ErrorCodes::PHY_ERR_READ_REG;
    }
    uint32_t temp;
    if (mdio_read(MDIO_LAN8742A_PHY_ADDRESS, reg_addr, &temp) != static_cast<HAL_StatusTypeDef>(ErrorCodes::PHY_OK)) return ErrorCodes::PHY_ERR_READ_REG;    
    *value = static_cast<uint16_t>(temp & 0xFFFF);
    return ErrorCodes::PHY_OK;
}

PHY_Base_Driver::ErrorCodes PHY_MCU_Driver::write_register_bit(uint8_t reg_addr, uint8_t bit_pos, bool set_bit) {  
    if (bit_pos > 15) return ErrorCodes::PHY_ERR_WRITE_REG;
    uint16_t reg_value;
    if (read_register(reg_addr, &reg_value) != ErrorCodes::PHY_OK) return ErrorCodes::PHY_ERR_WRITE_REG;
    if (set_bit) reg_value |= (1 << bit_pos);
    else reg_value &= ~(1 << bit_pos);
    return low_level_write(reg_addr, reg_value); 
}

PHY_Base_Driver::ErrorCodes PHY_MCU_Driver::low_level_write(uint8_t reg_addr, uint16_t value) {
    if (mdio_write(MDIO_LAN8742A_PHY_ADDRESS, reg_addr, value) != static_cast<HAL_StatusTypeDef>(ErrorCodes::PHY_OK))
        return ErrorCodes::PHY_ERR_WRITE_REG; 
    return ErrorCodes::PHY_OK;
}

PHY_Base_Driver::ErrorCodes PHY_MCU_Driver::read_status(uint16_t *status) {
    if (status == NULL) return ErrorCodes::PHY_ERR_READ_STATUS;
    return read_register(PHY_STATUS_REGISTER, status);
}

PHY_Base_Driver::ErrorCodes PHY_MCU_Driver::read_mode(uint8_t *mode) {
    uint16_t control_reg, auto_neg_reg;
    if (read_register(PHY_CONTROL_REGISTER, &control_reg) != ErrorCodes::PHY_OK)
        return ErrorCodes::PHY_ERR_READ_REG;
    if (read_register(PHY_AUTO_NEGOTIATION_REGISTER, &auto_neg_reg) != ErrorCodes::PHY_OK)
        return ErrorCodes::PHY_ERR_READ_REG;
    uint8_t remapped_bits = 0;
    const uint8_t control_positions[4] = {static_cast<uint8_t>(ControlRegister::PHY_CTRL_SPEED_SELECT),
        static_cast<uint8_t>(ControlRegister::PHY_CTRL_AUTO_NEGOTIATION_ENABLE), 
        static_cast<uint8_t>(ControlRegister::PHY_CTRL_ISOLATE), 
        static_cast<uint8_t>(ControlRegister::PHY_CTRL_DUPLEX_MODE)};
    for (int i = 0; i < 4; i++) {
        remapped_bits |= ((control_reg >> control_positions[i]) & 0x01) << (3-i); 
    }
    control_reg = remapped_bits;
    remapped_bits = 0;
    const uint8_t auto_neg_positions[4] = {static_cast<uint8_t>(ANEG_ADV_Register::PHY_ANEG_100BASE_TX_FULL_DUPLEX),
        static_cast<uint8_t>(ANEG_ADV_Register::PHY_ANEG_100BASE_TX), 
        static_cast<uint8_t>(ANEG_ADV_Register::PHY_ANEG_10BASE_T_FULL_DUPLEX), 
        static_cast<uint8_t>(ANEG_ADV_Register::PHY_ANEG_10BASE_T)};
    for (int i = 0; i < 4; i++) {
        remapped_bits |= ((auto_neg_reg >> auto_neg_positions[i]) & 0x01) << (3 - i);
    }
    auto_neg_reg = remapped_bits;
    *mode = (control_reg << 4) | (auto_neg_reg & 0x0F);
    return ErrorCodes::PHY_OK;
}
