/**
 *  @filename mag.h  
 *  @brief Magnetometer header file for the SIM_Driver (native)
 *  @date 2025-09-03
 */
#pragma once

#include "mag_base.h"

/**
 * @class Mag_SIM_Driver is derived from Mag_Base_Driver
 * @brief This is the class for the SIM_Driver (native)
 * @note This class will ONLY be built if the environment is 
 * set to "Native" when running cmake commands.
 */

class Mag_SIM_Driver : public Mag_Base_Driver {
public:

    /**
     * @brief Initialized the simulated driver
     * @return Error code, telling us whether or not the function worked
     */
    etl::expected<void, Mag_Base_Driver::ErrorCode> init(void) override;

    /**
     * @brief Gets the data from the simulated driver
     * @param data Magnetic_Field_Vector pointer to a variable to store said data
     * @return Error code, telling us whether or not the function worked
     */
    etl::expected <Mag_Base_Driver::Magnetic_Field_Vector, Mag_Base_Driver::ErrorCode> get_data(void) override;
};

