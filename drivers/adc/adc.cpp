#include "adc.h"

etl::expected<void, ADC_Base_Driver::ErrorCode> ADC_MCU_Driver::init(void) {
    uint8_t cmd = build_command(Commands::CMD_RST, 0, Register::CONFIG0);
    if(SPI_WRITE(&cmd, 1) != 0) return etl::unexpected(ErrorCode::ADS_WSPI_ERR);
    return {}; 
}

uint8_t ADC_MCU_Driver::build_command(Commands C, uint8_t MUL, Register A) {
    return (static_cast<uint8_t>(C) << 5) | (MUL << 4) | (static_cast<uint8_t>(A) & 0x0F);
} 

etl::expected<void, ADC_Base_Driver::ErrorCode> ADC_MCU_Driver::read_register(const Register reg, uint8_t* value) {
    uint8_t cmd = build_command(Commands::CMD_RREG, 0, reg);
    if (SPI_WRITE(&cmd, 1) != 0) return etl::unexpected(ErrorCode::ADS_WSPI_ERR);
    if (SPI_READ(value, 1) != 0) return etl::unexpected(ErrorCode::ADS_RSPI_ERR);
    return {};
}

etl::expected<void, ADC_Base_Driver::ErrorCode> ADC_MCU_Driver::write_register(Register reg, uint8_t value) {
    uint8_t cmd = build_command(Commands::CMD_WREG, 0, reg);
    uint8_t count = 0x00;
    if (SPI_WRITE(&cmd, 1) != 0) return etl::unexpected(ErrorCode::ADS_WSPI_ERR);
    if (SPI_WRITE(&count, 1) != 0) return etl::unexpected(ErrorCode::ADS_WSPI_ERR);
    if (SPI_WRITE(&value, 1) != 0) return etl::unexpected(ErrorCode::ADS_WSPI_ERR);
    return {};
}

etl::expected<void, ADC_Base_Driver::ErrorCode> ADC_MCU_Driver::read_register_mul(const Register start_reg, uint8_t* buf, uint8_t size) {
    uint8_t cmd = build_command(Commands::CMD_RREG, 1, start_reg);
    if(SPI_WRITE(&cmd, 1) != 0) return etl::unexpected(ErrorCode::ADS_WSPI_ERR);
    if(SPI_READ(buf, size) != 0) return etl::unexpected(ErrorCode::ADS_RSPI_ERR);
    return {};
}

etl::expected<void, ADC_Base_Driver::ErrorCode> ADC_MCU_Driver::write_register_mul(Register start_reg, uint8_t* values, uint8_t size) {
    if (size == 0) return etl::unexpected(ErrorCode::ADS_ERR);
    uint8_t cmd = build_command(Commands::CMD_WREG, 1, start_reg);
    uint8_t count = size-1;
    if (SPI_WRITE(&cmd, 1) != 0) return etl::unexpected(ErrorCode::ADS_WSPI_ERR);
    if (SPI_WRITE(&count, 1) != 0) return etl::unexpected(ErrorCode::ADS_WSPI_ERR);
    if (SPI_WRITE(values, size) != 0) return etl::unexpected(ErrorCode::ADS_WSPI_ERR);
    return {};
}

etl::expected<void, ADC_Base_Driver::ErrorCode> ADC_MCU_Driver::set_channel(ChannelID channel) {
    return write_register(Register::MUXDIF, (uint8_t) channel);
}

etl::expected<uint16_t, ADC_Base_Driver::ErrorCode> ADC_MCU_Driver::data_read(void) {
    uint8_t cmd = build_command(Commands::CMD_DATA_READ, 1, Register::CONFIG0);
    uint8_t rx_buf[3] = {0};
    if (SPI_WRITE(&cmd, 1) != 0) return etl::unexpected(ErrorCode::ADS_WSPI_ERR);
    if(SPI_READ(rx_buf, 4) != 0) return etl::unexpected(ErrorCode::ADS_RSPI_ERR);
    return (rx_buf[1] << 8) | (rx_buf[2]); 
}

