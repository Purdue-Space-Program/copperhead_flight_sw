#include "mag.h"

etl::expected<void, Mag_Base_Driver::ErrorCode> Mag_MCU_Driver::init(void)
{
    uint8_t cmd = 0xb6;
    if (I2C_WRITE(BMM_350_I2C_ADDRESS, static_cast<uint16_t>(Registers::CMD), &cmd, 1) !=
        static_cast<HAL_StatusTypeDef>(ErrorCode::BMM_OK))
        return etl::unexpected(ErrorCode::BMM_ERR_INIT);
    return {};
}

etl::expected<void, Mag_Base_Driver::ErrorCode> Mag_MCU_Driver::read_registers(
    const Mag_MCU_Driver::Registers start_reg, uint8_t *values, size_t len)
{
    if (values == NULL || len == 0)
        return etl::unexpected(ErrorCode::BMM_ERR_READ_REG);
    if (I2C_READ(BMM_350_I2C_ADDRESS, static_cast<uint16_t>(start_reg), values, len) !=
        static_cast<HAL_StatusTypeDef>(ErrorCode::BMM_OK))
        return etl::unexpected(ErrorCode::BMM_ERR_READ_REG);
    return {};
}

etl::expected<void, Mag_Base_Driver::ErrorCode> Mag_MCU_Driver::get_status(uint8_t *status)
{
    if (I2C_READ(BMM_350_I2C_ADDRESS, static_cast<uint16_t>(Registers::INT_STATUS), status, 1) !=
        static_cast<HAL_StatusTypeDef>(ErrorCode::BMM_OK))
        return etl::unexpected(ErrorCode::BMM_ERR_GET_STATUS);
    return {};
}

etl::expected<Mag_Base_Driver::Magnetic_Field_Vector, Mag_Base_Driver::ErrorCode> Mag_MCU_Driver::get_data(void)
{
    Magnetic_Field_Vector mag;
    uint8_t buffer[9];
    if (!read_registers(Registers::MAG_X_XLSB, buffer, 9).has_value())
        return etl::unexpected(ErrorCode::BMM_ERR_GET_DATA);
    mag.x = ((int32_t)buffer[0] << 16) | ((int32_t)buffer[1] << 8) | (int32_t)buffer[2];
    mag.y = ((int32_t)buffer[3] << 16) | ((int32_t)buffer[4] << 8) | (int32_t)buffer[5];
    mag.z = ((int32_t)buffer[6] << 16) | ((int32_t)buffer[7] << 8) | (int32_t)buffer[8];
    return mag;
}
