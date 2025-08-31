#include "adc.h"



ADC_Driver::ErrorCodes ADC_SIM_Driver::init(void) {
    return ErrorCodes::ADS_OK; 
}

ADC_Driver::ErrorCodes ADC_SIM_Driver::set_channel(ADC_Driver::ChannelID channel) {
    return ErrorCodes::ADS_OK;
}

ADC_Driver::ErrorCodes ADC_SIM_Driver::data_read(int32_t *result, uint8_t *status) {
    return ErrorCodes::ADS_OK;
}



ADC_Driver::ErrorCodes ADC_MCU_Driver::init(void) {
    uint8_t cmd = build_command(Commands::CMD_RST, 0, Register::CONFIG0);
    if(SPI_WRITE(&cmd, 1) != 0) return ErrorCodes::ADS_WSPI_ERR;
    return ErrorCodes::ADS_OK; 
}

uint8_t ADC_MCU_Driver::build_command(Commands C, uint8_t MUL, Register A) {
    return (static_cast<uint8_t>(C) << 5) | (MUL << 4) | (static_cast<uint8_t>(A) & 0x0F);
} 

ADC_Driver::ErrorCodes ADC_MCU_Driver::read_register(const Register reg, uint8_t* value) {
    uint8_t cmd = build_command(Commands::CMD_RREG, 0, reg);
    if (SPI_WRITE(&cmd, 1) != 0) return ErrorCodes::ADS_WSPI_ERR;
    if (SPI_READ(value, 1) != 0) return ErrorCodes::ADS_RSPI_ERR;
    return ErrorCodes::ADS_OK;
}

ADC_Driver::ErrorCodes ADC_MCU_Driver::write_register(Register reg, uint8_t value) {
    uint8_t cmd = build_command(Commands::CMD_WREG, 0, reg);
    uint8_t count = 0x00;
    if (SPI_WRITE(&cmd, 1) != 0) return ErrorCodes::ADS_WSPI_ERR;
    if (SPI_WRITE(&count, 1) != 0) return ErrorCodes::ADS_WSPI_ERR;
    if (SPI_WRITE(&value, 1) != 0) return ErrorCodes::ADS_WSPI_ERR;
    return ErrorCodes::ADS_OK;
}

ADC_Driver::ErrorCodes ADC_MCU_Driver::read_register_mul(const Register start_reg, uint8_t* buf, uint8_t size) {
    uint8_t cmd = build_command(Commands::CMD_RREG, 1, start_reg);
    if(SPI_WRITE(&cmd, 1) != 0) return ErrorCodes::ADS_WSPI_ERR;
    if(SPI_READ(buf, size) != 0) return ErrorCodes::ADS_RSPI_ERR;
    return ErrorCodes::ADS_OK;
}

ADC_Driver::ErrorCodes ADC_MCU_Driver::write_register_mul(Register start_reg, uint8_t* values, uint8_t size) {
    if (size == 0) return ErrorCodes::ADS_ERR;
    uint8_t cmd = build_command(Commands::CMD_WREG, 1, start_reg);
    uint8_t count = size-1;
    if (SPI_WRITE(&cmd, 1) != 0) return ErrorCodes::ADS_WSPI_ERR;
    if (SPI_WRITE(&count, 1) != 0) return ErrorCodes::ADS_WSPI_ERR;
    if (SPI_WRITE(values, size) != 0) return ErrorCodes::ADS_WSPI_ERR;
    return ErrorCodes::ADS_OK;
}

ADC_Driver::ErrorCodes ADC_MCU_Driver::set_channel(ChannelID channel) {
    return write_register(Register::MUXDIF, (uint8_t) channel);
}

ADC_Driver::ErrorCodes ADC_MCU_Driver::data_read(int32_t *result, uint8_t *status) {
    if (result == NULL || status == NULL) return ErrorCodes::ADS_ERR;
    uint8_t cmd = build_command(Commands::CMD_DATA_READ, 1, Register::CONFIG0);
    uint8_t rx_buf[4] = {0};
    if (SPI_WRITE(&cmd, 1) != 0) return ErrorCodes::ADS_WSPI_ERR;
    if(SPI_READ(rx_buf, 4) != 0) return ErrorCodes::ADS_RSPI_ERR;
    int32_t raw24 = (rx_buf[1] << 16) | (rx_buf[2] << 8) | (rx_buf[3]);
    *result = (int32_t)(raw24 << 8) >> 8;
    *status = rx_buf[0];
    return ErrorCodes::ADS_OK;
}

