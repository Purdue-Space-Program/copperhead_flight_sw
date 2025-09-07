/**
 * @file bmm350.c
 * @brief Implementation for driver of the BMM350 Magnetometer
 * @author Pradyunn Kale
 * @date 2025-08-03
 */

#include "bmm350.h"

/**
 * @brief Initializes the BMM350 Magnetometer.
 *
 * @details The function initializes the I2C communication for the BMM350 magnetometer.
 * It returns an error code if the initialization fails.
 *
 * @return int8_t Returns BMM_OK on success, or an error code on failure.
 */
int8_t bmm_init(void)
{
    uint8_t cmd = 0xb6;
    if (I2C_WRITE(BMM_350_I2C_ADDRESS, CMD, &cmd, 1) != BMM_OK)
        return BMM_ERR_INIT;
    return BMM_OK;
}

/**
 * @brief Reads a single/multiple registers from the BMM350 Magnetometer.
 *
 * @details The function reads the value of the specified register from the BMM350 magnetometer.
 * It returns an error code if the read operation fails.
 *
 * @param reg The register to read from.
 * @param values Pointer to store the read values.
 * @param len The number of bytes to read.
 * @return int8_t Returns BMM_OK on success, or an error code on failure.
 */
int8_t bmm_read_register(registers_t start_reg, uint8_t *values, size_t len)
{
    if (values == NULL || len == 0)
        return BMM_ERR_READ_REG;
    if (I2C_READ(BMM_350_I2C_ADDRESS, start_reg, values, len) != BMM_OK)
        return BMM_ERR_READ_REG;
    return BMM_OK;
}

/**
 * @brief Gets the status of the BMM350 Magnetometer.
 *
 * @details The function reads the status register of the BMM350 magnetometer.
 * It returns an error code if the read operation fails.
 *
 * @param status Pointer to store the status value.
 * @return int8_t Returns BMM_OK on success, or an error code on failure.
 */
int8_t bmm_get_status(uint8_t *status)
{
    if (I2C_READ(BMM_350_I2C_ADDRESS, INT_STATUS, status, 1) != BMM_OK)
        return BMM_ERR_GET_STATUS;
    return BMM_OK;
}

/**
 * @brief Gets the magnetic field data from the BMM350 Magnetometer.
 *
 * @details The function reads the magnetic field data from the BMM350 magnetometer.
 * It combines the x, y, and z components into a magnetic_field_vector_t structure.
 *
 * @param data Pointer to store the magnetic field vector data.
 * @return int8_t Returns BMM_OK on success, or an error code on failure.
 */
int8_t bmm_get_data(magnetic_field_vector_t *data)
{
    if (data == NULL)
        return BMM_ERR_GET_DATA;
    int32_t combined_value;
    uint8_t buffer[9];
    if (bmm_read_register(MAG_X_XLSB, buffer, 9) != BMM_OK)
        return BMM_ERR_GET_DATA;
    data->x = ((int32_t)buffer[0] << 16) | ((int32_t)buffer[1] << 8) | (int32_t)buffer[2];
    data->y = ((int32_t)buffer[3] << 16) | ((int32_t)buffer[4] << 8) | (int32_t)buffer[5];
    data->z = ((int32_t)buffer[6] << 16) | ((int32_t)buffer[7] << 8) | (int32_t)buffer[8];
    return BMM_OK;
}
