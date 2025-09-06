#include "phy.h"
    
PHY_Base_Driver::ErrorCodes PHY_MCU_Driver::init(void) {
    if (write_register_bit(PHY_CONTROL_REGISTER, static_cast<uint8_t>(ControlRegister::PHY_CTRL_SOFT_RESET), true) == ErrorCodes::PHY_ERR_WRITE_REG)
        return PHY_Base_Driver::ErrorCodes::PHY_ERR_INIT;
    return PHY_Base_Driver::ErrorCodes::PHY_OK;
}

etl::expected<uint16_t, PHY_Base_Driver::ErrorCodes> PHY_MCU_Driver::read_register(const uint8_t reg_addr){
    //Validate the register address
    switch (reg_addr) {
        case PHY_CONTROL_REGISTER:
        case PHY_STATUS_REGISTER:
        case PHY_AUTO_NEGOTIATION_REGISTER:
            break;
        default:
            return etl::unexpected(ErrorCodes::PHY_ERR_READ_REG);
    }
    uint32_t temp;
    return mdio_read(MDIO_LAN8742A_PHY_ADDRESS, reg_addr, &temp);
    return static_cast<uint16_t>(temp & 0xFFFF);
}

PHY_Base_Driver::ErrorCodes PHY_MCU_Driver::write_register_bit(uint8_t reg_addr, uint8_t bit_pos, bool set_bit) {  
    if (bit_pos > 15) return ErrorCodes::PHY_ERR_WRITE_REG;
    if (!read_register(reg_addr).has_value()){
        return ErrorCodes::PHY_ERR_READ_REG;
    }
    uint16_t reg_value = read_register(reg_addr).value();
    if (set_bit) {
        reg_value |= (1 << bit_pos);
    }
    else {
        reg_value &= ~(1 << bit_pos);
    } 
    return static_cast<ErrorCodes>(mdio_write(MDIO_LAN8742A_PHY_ADDRESS, reg_addr, reg_value)); 
}

etl::expected<uint16_t, PHY_Base_Driver::ErrorCodes> PHY_MCU_Driver::read_status(void) {
    return read_register(PHY_STATUS_REGISTER);
}

etl::expected<uint16_t, PHY_Base_Driver::ErrorCodes> PHY_MCU_Driver::read_mode(void) {
    uint16_t control_reg = 0;
    uint16_t auto_neg_reg = 0;
    if (!read_register(PHY_CONTROL_REGISTER).has_value())
        return etl::unexpected(ErrorCodes::PHY_ERR_READ_REG);
    if (!read_register(PHY_AUTO_NEGOTIATION_REGISTER).has_value())
        return etl::unexpected(ErrorCodes::PHY_ERR_READ_REG);
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
    return (control_reg << 4) | (auto_neg_reg & 0x0F);
}

PHY_Base_Driver::ErrorCodes PHY_MCU_Driver::apply_mode(uint8_t mode) {
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
        static_cast<const uint8_t>(ControlRegister::PHY_CTRL_SPEED_SELECT), 
        static_cast<const uint8_t>(ControlRegister::PHY_CTRL_AUTO_NEGOTIATION_ENABLE), 
        static_cast<const uint8_t>(ControlRegister::PHY_CTRL_ISOLATE), 
        static_cast<const uint8_t>(ControlRegister::PHY_CTRL_DUPLEX_MODE),   
        static_cast<const uint8_t>(ANEG_ADV_Register::PHY_ANEG_100BASE_TX_FULL_DUPLEX), 
        static_cast<const uint8_t>(ANEG_ADV_Register::PHY_ANEG_100BASE_TX), 
        static_cast<const uint8_t>(ANEG_ADV_Register::PHY_ANEG_10BASE_T_FULL_DUPLEX), 
        static_cast<const uint8_t>(ANEG_ADV_Register::PHY_ANEG_10BASE_T)       
    };
    for (int i = 0; i < 8; i++) {
        uint8_t bit_val = (mode >> (7 - i)) & 0x01;
        if (write_register_bit(regs[i], bits[i], bit_val) != ErrorCodes::PHY_OK) {
            return ErrorCodes::PHY_ERR_READ_REG;
        }
    }
    return ErrorCodes::PHY_OK;
}
