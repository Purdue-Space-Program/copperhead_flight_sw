#include "mag.h"

etl::expected<void, Mag_Base_Driver::ErrorCode> Mag_SIM_Driver::init(void)
{
    return {};
}

etl::expected<Mag_Base_Driver::Magnetic_Field_Vector, Mag_Base_Driver::ErrorCode> Mag_SIM_Driver::get_data(void)
{
    return {};
}
