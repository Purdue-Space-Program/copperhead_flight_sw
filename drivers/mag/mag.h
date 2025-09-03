/**
 *  @filename mag.h  
 *  @brief Magnetometer header file for the MCU_Driver (hardware)
 *  @date 2025-09-03
 */

#include "../../api/mag_base.h"
#include "stm32h7xx_hal.h"

/**
 * @brief I2C Hardware peripheral assumed to be declared outside of this file
 * by the user.
 */

extern I2C_HandleTypeDef hi2c1;

/**
 * @brief Macro for the HAL_I2C_Mem_Read function because I did not feel like changing my code.
 */

#define I2C_READ(device_addr, reg_addr, buffer, len) \
HAL_I2C_Mem_Read(&hi2c1, (device_addr) << 1, (reg_addr), I2C_MEMADD_SIZE_8BIT, (uint8_t *)(buffer), (len), HAL_MAX_DELAY)

/**
 * @brief Macro for the HAL_I2C_Mem_Write because I did not feel like changing my code.
 */

#define I2C_WRITE(device_addr, reg_addr, data, len) \
HAL_I2C_Mem_Write(&hi2c1, (device_addr) << 1, (reg_addr), I2C_MEMADD_SIZE_8BIT, (uint8_t *)(data), (len), HAL_MAX_DELAY)

#define BMM_350_I2C_ADDRESS 0x10

/**
 * @class Mag_MCU_Driver is derived from Mag_Base_Driver
 * @brief This is the class for the MCU_Driver (hardware)
 * @note This class will ONLY be built if the environment is 
 * set to "STM32H730" when running cmake commands.
 */

class Mag_MCU_Driver : public Mag_Base_Driver {
public:
    
    /**
     * @enum Registers
     * @brief Contains all the register names and addresses for the 
     * BMM350 magnetometer
     */

    enum class Registers {
        CMD         = 0x7E,
        INT_STATUS  = 0x30,
        MAG_X_XLSB  = 0x31,
        MAG_X_LSB   = 0x32,
        MAG_X_MSB   = 0x33,
        MAG_Y_XLSB  = 0x34,
        MAG_Y_LSB   = 0x35,
        MAG_Y_MSB   = 0x36,
        MAG_Z_XLSB  = 0x37,
        MAG_Z_LSB   = 0x38,
        MAG_Z_MSB   = 0x39,
    };
    
    /**
     * @brief Initializes the hardware for the BMM350, overrides the base_driver
     * @return Error code, tells us if the function worked or not
     */

    Mag_Base_Driver::ErrorCodes init(void) override;

    /**
     * @brief Reads a register and pastes that value into a variable
     * @param start_reg const Registers address of the start_register
     * @param values uint8_t pointer variable to copy the value or values in the register to
     * @param len size_t which determines how many registers are being read
     * @return Error code telling us whether or not the function worked
     * @note This function works for multiple registers or just 1 register, you just need
     * to specify the length
     */

    Mag_Base_Driver::ErrorCodes read_registers(const Registers start_reg, uint8_t *values, size_t len);
    
    /**
     * @brief Gets the status of the BMM350 magnetometer
     * @param status uint8_t pointer to copy the status into
     * @return Error code telling us whether or not the function worked
     */

    Mag_Base_Driver::ErrorCodes get_status(uint8_t *status);

    /**
     * @brief Gets the data from the BMM350 magnetometer, overrides the base_driver
     * @param data Magnetic_Field_Vector pointer to copy the data for the Magnetic Field Vector
     * into
     * @return Error code telling us whether or not the function worked
     */
    Mag_Base_Driver::ErrorCodes get_data(Mag_Base_Driver::Magnetic_Field_Vector *data) override;
};

