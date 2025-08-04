#include "ads1158.h"
//Initialization Operation
int8_t ads_init(void) {
	//We can reset the ADC to "initialize" it.
	//Reset command found on Page 33 under "Bits C[2:0]---Command Bits"
	//Reset Command is when C[2:0] = 110, no matter what MUL and A[3:0] are
	uint8_t cmd = build_command(CMD_RST, 0, 0);
	//If there is an error, return -1, otherwise return 0
	if (SPI_WRITE(&cmd, 1) != 0) { //This assumes that SPI_WRITE returns 0 as no errors
		return ADS_WSPI_ERROR;
	}
	return ADS_OK;
}

// Command Byte given by C[2:0] MUL A[3:0] by MSB, where A[3:0] corresponds to the Register Map
uint8_t build_command(uint8_t C, uint8_t MUL, ADS1158_Register A) {
	return(C << 5) | (MUL << 4) | (A & 0x0F); //0x0F so it keeps the last 4 bits but clears everything else (in terms of MSB)
}

//Register Operations
int8_t ads_read_register(ADS1158_Register reg, uint8_t* value) {
	//Page 33 under "Bits C[2:0]---Command Bits"
	uint8_t cmd = build_command(CMD_RREG, 0, reg);
	//If there is an error in writing to SPI
	if (SPI_WRITE(&cmd, 1) != 0){
		return ADS_WSPI_ERROR;
	}
	//If there is an error in reading SPI
	if (SPI_READ(value, 1) != 0){
		return ADS_RSPI_ERROR;
	}
	//If there are no errors return the register value
	return ADS_OK;
}

int8_t ads_write_register(ADS1158_Register reg, uint8_t value) {
	//Page 33 under "Bits C[2:0]---Command Bits"
	uint8_t cmd = build_command(CMD_WREG, 0, reg);
	uint8_t count = 0x00; // Writing 1 register
	//Send command byte first
	if (SPI_WRITE(&cmd, 1) != 0){
		return ADS_WSPI_ERROR;
	}
	//Count byte so it knows that we're only writing 1 register
	if (SPI_WRITE(&count, 1) != 0){
		return ADS_WSPI_ERROR;
	}
	//Send all the register values to write
	if (SPI_WRITE(&value, 1) != 0){
		return ADS_WSPI_ERROR;
	}
	return ADS_OK;
}

int8_t ads_read_register_mul(ADS1158_Register start_reg, uint8_t* buf, uint8_t size) {
	//Page 33 under "Bits C[2:0]---Command Bits"
	uint8_t cmd = build_command(CMD_RREG, 1, start_reg); //MUL = 1 for multiple registers
	//Send command byte
	if(SPI_WRITE(&cmd, 1) != 0){
		return ADS_WSPI_ERROR;
	}
	//Read 'size' registers sequentially
	if(SPI_READ(buf, size) != 0){
		return ADS_RSPI_ERROR;
	}
	return ADS_OK;
}

int8_t ads_write_register_mul(ADS1158_Register start_reg, uint8_t* values, uint8_t size) {
	if (size == 0){
		return ADS_ERROR;
	}
	//Page 33 under "Bits C[2:0]---Command Bits"
	uint8_t cmd = build_command(CMD_WREG, 1, start_reg); //MUL = 1 for multiple registers
	uint8_t count = size-1;
	//Send command byte first
	if (SPI_WRITE(&cmd, 1) != 0){
		return ADS_WSPI_ERROR;
	}
	//Send count byte
	if (SPI_WRITE(&count, 1) != 0){
		return ADS_WSPI_ERROR;
	}
	//Send all the register values to write
	if (SPI_WRITE(values, size) != 0){
		return ADS_WSPI_ERROR;
	}
	return ADS_OK;
}

//Channel Operations
int8_t ads_set_channel(ADS1158_ChannelID channel) {
	return ads_write_register(MUXDIF, (uint8_t)channel);
}

int8_t ads_data_read(int32_t *result, uint8_t *status){
	//If the pointers point to nothing
	if (result == NULL || status == NULL){
		return ADS_ERROR;
	}
	//Page 33 under "Bits C[2:0]---Command Bits"
	uint8_t cmd = build_command(CMD_DATA_READ, 1, 0);
	//Buffer to store the result of the read
	uint8_t rx_buf[4] = {0};
	//Send command byte
	if (SPI_WRITE(&cmd, 1) != 0){
		return ADS_WSPI_ERROR;
	}
	//Read status byte + 3 bytes of conversion data
	if(SPI_READ(rx_buf, 4) != 0) {
		return ADS_RSPI_ERROR;
	}
	//Stores the data, stores it in raw24 so that way sign can be preserved
	int32_t raw24 = (rx_buf[1] << 16) | (rx_buf[2] << 8) | (rx_buf[3]);
	//Sign is preserved with line under
	*result = (int32_t)(raw24 << 8) >> 8;
	//Also saves the status
	*status = rx_buf[0];
	return ADS_OK;
}
