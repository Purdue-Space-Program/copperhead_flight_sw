#include "phy.h"

class PHY_SIM_Driver : public PHY_Driver {
public:
    PHY_Error phy_init(void) override {

    }

    PHY_Error phy_read_mode(uint8_t *mode) override {

    }

    PHY_Error phy_apply_mode(const uint8_t mode) override {

    }
};

class PHY_MCU_Driver : public PHY_Driver {
    PHY_Error phy_init(void) override {
        if (phy_write_register_bit(PHY_CONTROL_REGISTER, PHY_CTRL_SOFT_RESET, true) != PHY_OK)
            return PHY_ERR_INIT;
        return PHY_OK;
    }

    
    PHY_Error phy_read_register(uint8_t reg_addr, uint16_t *value){
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

    PHY_Error phy_write_register_bit(const uint8_t reg_addr, uint8_t bit_pos, bool set_bit) {  
        if (bit_pos > 15) return PHY_ERR_WRITE_REG;
        uint16_t reg_value;
        if (phy_read_register(reg_addr, &reg_value) != PHY_OK) return PHY_ERR_WRITE_REG;
        if (set_bit) reg_value |= (1 << bit_pos);
        else reg_value &= ~(1 << bit_pos);
        return phy_low_level_write(reg_addr, reg_value); 
    }

    PHY_Error phy_low_level_write(uint8_t reg_addr, uint16_t value) {
        if (mdio_write(LAN8742A_PHY_ADDRESS, reg_addr, value) != PHY_OK)
            return PHY_ERR_WRITE_REG; 
        return PHY_OK;
    }

    PHY_Error phy_read_status(uint16_t *status) {
        if (status == NULL) return PHY_ERR_READ_STATUS;
        return phy_read_register(PHY_STATUS_REGISTER, status);
    }

    PHY_Error phy_read_mode(uint8_t *mode) override {
        uint16_t control_reg, auto_neg_reg;
        if (phy_read_register(PHY_CONTROL_REGISTER, &control_reg) != PHY_OK)
            return PHY_ERR_READ_REG;
        if (phy_read_register(PHY_AUTO_NEGOTIATION_REGISTER, &auto_neg_reg) != PHY_OK)
            return PHY_ERR_READ_REG;
        uint8_t remapped_bits = 0;
        const uint8_t control_positions[4] = {13, 12, 10, 8};
        for (int i = 0; i < 4; i++) {
            remapped_bits |= ((control_reg >> control_positions[i])) 
        }
    }
};
