/**
 * @file ads1158.c
 * @brief Implementation for driver of the ADS1158 ADC
 * @author Pradyunn Kale
 * @date 2025-08-03
 */

#include "ads1158.h"

/**
 * @brief Initializes the ADS1158 ADC.
 * 
 * @details The function performs a soft reset of the ADS1158 ADC by sending the reset command.
 * The reset command is built using the 'build_command' funtion with CMD_RST as the command type.
 * Found on Page 33 under "Bits C[2:0]---Command Bits".
 * 
 * @return int8_t Returns ADS_OK on success, or an error code on failure.
 */
int8_t ads_init(void) {
	uint8_t cmd = build_command(CMD_RST, 0, 0);
	if (SPI_WRITE(&cmd, 1) != 0) return ADS_WSPI_ERROR;
	return ADS_OK;
}

/**
 * @brief Builds a command byte for the ADS1158 ADC.
 * 
 * @details The command byte is constructed from the provided parameters using the following format:
 * Found on Page 33 under "Bits C[2:0]---Command Bits".
 * 
 * @param C For the Command bits (C[2:0]) (bits 7:5).
 * @param MUL For the multiple register access bit (bit 4)
 * @param A For the register address bits (A[3:0]). (bits 3:0)
 * @return uint8_t Returns the command byte constructed from the provided parameters.
 */
uint8_t build_command(uint8_t C, uint8_t MUL, ADS1158_Register A) {
	return(C << 5) | (MUL << 4) | (A & 0x0F);
}

/**
 * @brief Reads a single register from the ADS1158 ADC. 
 * 
 * @details The function sends a read command to the ADS1158 ADC 
 * and reads the value of the specified register. 
 * Found on Page 33 under "Bits C[2:0]---Command Bits".
 * 
 * @param reg The register to read from
 * @param value The pointer to store the read value
 * @return int8_t Returns ADS_OK on success, or an error code on failure.
 */
int8_t ads_read_register(ADS1158_Register reg, uint8_t* value) {
	uint8_t cmd = build_command(CMD_RREG, 0, reg);
	if (SPI_WRITE(&cmd, 1) != 0) return ADS_WSPI_ERROR;
	if (SPI_READ(value, 1) != 0) return ADS_RSPI_ERROR;
	return ADS_OK;
}

/**
 * @brief Writes a single register in the ADS1158 ADC.
 * 
 * @details The function sends a write command to the ADS1158 ADC
 * Found on Page 33 under "Bits C[2:0]---Command Bits".
 * 
 * @param reg The register to write to. 
 * @param value The value to write to the register.
 * @return int8_t Returns ADS_OK on success, or an error code on failure.
 */
int8_t ads_write_register(ADS1158_Register reg, uint8_t value) {
	uint8_t cmd = build_command(CMD_WREG, 0, reg);
	uint8_t count = 0x00;
	if (SPI_WRITE(&cmd, 1) != 0) return ADS_WSPI_ERROR;
	if (SPI_WRITE(&count, 1) != 0) return ADS_WSPI_ERROR;
	if (SPI_WRITE(&value, 1) != 0) return ADS_WSPI_ERROR;
	return ADS_OK;
}

/**
 * @brief Reads multiple registers from the ADS1158 ADC. 
 * 
 * @details The function sends a read command to the ADS1158 ADC,
 * however, the multiple register access bit (MUL) is set to 1, allowing
 * for multiple registers to be read in one operation.
 * Found on Page 33 under "Bits C[2:0]---Command Bits".
 * 
 * @param reg The register to read from. 
 * @param buf The pointer to the buffer to store the read values. 
 * @param size The number of registers to read.
 * @return int8_t Returns ADS_OK on success, or an error code on failure.
 */
int8_t ads_read_register_mul(ADS1158_Register start_reg, uint8_t* buf, uint8_t size) {
	uint8_t cmd = build_command(CMD_RREG, 1, start_reg);
	if(SPI_WRITE(&cmd, 1) != 0) return ADS_WSPI_ERROR;
	if(SPI_READ(buf, size) != 0) return ADS_RSPI_ERROR;
	return ADS_OK;
}

/**
 * @brief Writes multiple registers in the ADS1158 ADC.
 * 
 * @details The function sends a write command to the ADS1158 ADC,
 * however, the multiple register access bit (MUL) is set to 1, allowing
 * for multiple registers to be written in one operation.
 * Found on Page 33 under "Bits C[2:0]---Command Bits".
 * 
 * @param start_reg The starting register to write to. 
 * @param values The pointer to the buffer containing the values to write. 
 * @param size The number of registers to write.
 * @return int8_t Returns ADS_OK on success, or an error code on failure.
 */
int8_t ads_write_register_mul(ADS1158_Register start_reg, uint8_t* values, uint8_t size) {
	if (size == 0) return ADS_ERROR;
	uint8_t cmd = build_command(CMD_WREG, 1, start_reg);
	uint8_t count = size-1;
	if (SPI_WRITE(&cmd, 1) != 0) return ADS_WSPI_ERROR;
	if (SPI_WRITE(&count, 1) != 0) return ADS_WSPI_ERROR;
	if (SPI_WRITE(values, size) != 0) return ADS_WSPI_ERROR;
	return ADS_OK;
}

/**
 * @brief Sets the input channel for the ADS1158 ADC to read data from.
 * 
 * @details The function writes the specified channel ID to the MUXDIF register,
 * this means that the ADS1158 will read data from the specified channel, when told to read data.
 * Found on Page 32, Table 10, Channel ID and Measurement Order (Auto-Scan Mode).
 * 
 * @param channel The channel to set. This should be one of the ADS1158_ChannelID values.
 * @return int8_t Returns ADS_OK on success, or an error code on failure.
 */
int8_t ads_set_channel(ADS1158_ChannelID channel) {
	return ads_write_register(MUXDIF, (uint8_t)channel);
}

/**
 * @brief Reads data from the ADS1158 ADC and returns the result along with the status.
 * Details found on Page 33 under "Bits C[2:0]---Command Bits
 * 
 * @details The function sends a command to read data from the ADS1158 ADC
 * there are 4 bytes of data returned, the first byte is the status byte,
 * the next three bytes are the 24-bit data value. The 24-bit data value is then
 * converted to a signed 32-bit integer by shifting it left by 8 bits and then
 * sign-extending it to 32 bits. The status byte and the data value are both
 * stored in the provided pointers.
 * 
 * @param result Pointer to store the read data. 
 * @param status Pointer to store the status byte.
 * @return int8_t Returns ADS_OK on success, or an error code on failure.
 */
int8_t ads_data_read(int32_t *result, uint8_t *status){
	if (result == NULL || status == NULL) return ADS_ERROR;
	uint8_t cmd = build_command(CMD_DATA_READ, 1, 0);
	uint8_t rx_buf[4] = {0};
	if (SPI_WRITE(&cmd, 1) != 0) return ADS_WSPI_ERROR;
	if(SPI_READ(rx_buf, 4) != 0) return ADS_RSPI_ERROR;
	int32_t raw24 = (rx_buf[1] << 16) | (rx_buf[2] << 8) | (rx_buf[3]);
	*result = (int32_t)(raw24 << 8) >> 8;
	*status = rx_buf[0];
	return ADS_OK;
}
