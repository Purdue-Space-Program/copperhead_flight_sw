#include "adc.h"


etl::expected<void, ADC_Base_Driver::ErrorCodes> ADC_SIM_Driver::init(void) {
    return {}; 
}

etl::expected<void, ADC_Base_Driver::ErrorCodes> ADC_SIM_Driver::set_channel(ADC_Base_Driver::ChannelID channel) {
    return {};
}

etl::expected<uint16_t, ADC_Base_Driver::ErrorCodes> ADC_SIM_Driver::data_read(void) {
    return {};
}
