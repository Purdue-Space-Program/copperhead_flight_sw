#pragma once
#include <stdint.h>
#include <stddef.h> //For NULL
#define SPI_READ(dst, size) (0)
#define SPI_WRITE(src, size) (0)
#define ADS_OK 0
#define ADS_ERROR 1
#define ADS_RSPI_ERROR 2
#define ADS_WSPI_ERROR 3
#define CMD_DATA_READ 0x01
#define CMD_RREG 0x02
#define CMD_WREG 0x03
#define CMD_RST 0x06



//Page 34, Table 11. Register Map
typedef enum {
	CONFIG0 = 0x00,
	CONFIG1 = 0x01,
	MUXSCH = 0x02,
	MUXDIF = 0x03,
	MUXSG0 = 0x04,
	MUXSG1 = 0x05,
	SYSRED = 0x06,
	GPIOC = 0x07,
	GPIOD = 0x08,
	ID = 0x09,
}ADS1158_Register;

//Page 32, Table 10, Channel ID and Measurement Order (Auto-Scan Mode)
typedef enum {
	ADS_CH_DIFF0 = 0x00,
	ADS_CH_DIFF1 = 0x01,
	ADS_CH_DIFF2 = 0x02,
	ADS_CH_DIFF3 = 0x03,
	ADS_CH_DIFF4 = 0x04,
	ADS_CH_DIFF5 = 0x05,
	ADS_CH_DIFF6 = 0x06,
	ADS_CH_DIFF7 = 0x07,
}ADS1158_ChannelID;

//Initialization Operation
int8_t ads_init(void);
uint8_t build_command(uint8_t C, uint8_t MUL, ADS1158_Register A);

//Register Operations
int8_t ads_read_register(ADS1158_Register reg, uint8_t* value);
int8_t ads_write_register(ADS1158_Register reg, uint8_t value);
int8_t ads_read_register_mul(ADS1158_Register start_reg, uint8_t* buf, uint8_t size);
int8_t ads_write_register_mul(ADS1158_Register start_reg, uint8_t* values, uint8_t size);

//Channel Operations
int8_t ads_set_channel(ADS1158_ChannelID channel);
int8_t ads_data_read(int32_t *result, uint8_t *status);



