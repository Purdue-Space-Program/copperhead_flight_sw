#include "phy.h"

etl::expected<void, PHY_Base_Driver::ErrorCode> PHY_SIM_Driver::init(void)
{
    return {};
}

etl::expected<uint16_t, PHY_Base_Driver::ErrorCode> PHY_SIM_Driver::read_mode(void)
{
    return 0;
}

etl::expected<void, PHY_Base_Driver::ErrorCode> PHY_SIM_Driver::apply_mode(const uint8_t mode)
{
    return {};
}
