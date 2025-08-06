/**
 * @file bmm350.h
 * @brief Header for driver of the BMM350 Magnetometer
 * @author Pradyunn Kale
 * @date 2025-08-03
 */
#pragma once 
#include <stdint.h>
#include <stddef.h>

#define I2C_READ(device_addr, reg_addr, buffer, len) (0)
#define I2C_WRITE(device_addr, reg_addr, data, len) (0)
#define BMM_350_I2C_ADDRESS 0x10 // Example I2C address, change as per your device

/**
 * @brief Structure to hold the magnetic field vector data
 * 
 * @details This structure contains the x, y, and z components of the magnetic field vector
 * measured by the BMM350 magnetometer. The BMM350 Magnetometer gives the values to 
 * the I2C protocol as floating point values. Hence the use of float type.
 */
typedef struct {
    int32_t x;
    int32_t y;
    int32_t z;
} magnetic_field_vector_t;

/**
 * @brief Enumeration for BMM350 registers
 * 
 * @details This enumeration defines the registers used in the BMM350 magnetometer.
 * The CMD register is used to send commands to the device.
 */
typedef enum {
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
}registers_t;

/**
 * @brief Enumeration for BMM350 error codes
 * 
 * @details This enumeration defines the error codes that can be returned by the BMM350 driver functions.
 * These error codes indicate various types of errors that can occur during communication with the BMM350
 * magnetometer or during data processing.
 */
typedef enum {
    BMM_OK = 0,
    BMM_ERR_INIT = -1,
    BMM_ERR_READ_REG = -2,
    BMM_ERR_GET_STATUS = -3,
    BMM_ERR_GET_DATA = -4,
    BMM_ERR_COMBINE_AXIS_DATA = -5,
}error_codes_t;

int8_t bmm_init(void);
int8_t bmm_read_register(registers_t start_reg, uint8_t *values, size_t len);
int8_t bmm_get_status(uint8_t *status);
int8_t bmm_get_data(magnetic_field_vector_t *data);
int8_t bmm_combine_axis_data(uint8_t xlsb, uint8_t lsb, uint8_t msb, int32_t *result);