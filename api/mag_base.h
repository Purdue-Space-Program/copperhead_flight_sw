/**
 * @filename mag_base.h
 * @brief Magnetometer Driver API
 * @date 2025-09-03
 */

#pragma once
#include <etl/expected.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @class Mag_Base_Driver
 * @brief This is the API for the Magnetometer driver
 *
 * @details This class contains the overlap of the simulated drivers
 * and the hardware drivers. It's main purpose is to provide
 * allow for the same code to be used in different environments,
 * this allows for modularity and less reconfiguration when switching
 * between the 2 environments Native and STM32H730.
 */

class Mag_Base_Driver
{
  public:
    /**
     * @struct Magnetic_Field_Vector
     * @brief Represents the magnetic field vector in 3 dimensions
     */

    struct Magnetic_Field_Vector
    {
        int32_t x;
        int32_t y;
        int32_t z;
    };

    /**
     * @enum ErrorCode
     * @brief Removes the usage of magic numbers in code, return values mean something
     */

    enum class ErrorCode
    {
        BMM_OK = 0,
        BMM_ERR_INIT = -1,
        BMM_ERR_READ_REG = -2,
        BMM_ERR_GET_STATUS = -3,
        BMM_ERR_GET_DATA = -4,
        BMM_ERR_COMBINE_AXIS_DATA = -5,
    };

    virtual ~Mag_Base_Driver() = default;

    /**
     * @brief virtual function Initializes the magnetometer
     * @return An error code, telling us whether or not the function worked
     */

    virtual etl::expected<void, ErrorCode> init(void) = 0;

    /**
     * @brief virtual function Gets the magnetic field vector from the magnetometer
     * @param data Magnetic_Field_Vector pointer to copy the value into
     * @return An error code, telling us whether or not the function worked
     */

    virtual etl::expected<Magnetic_Field_Vector, ErrorCode> get_data(void) = 0;
};
