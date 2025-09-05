#include "adc.h"


ADC_Base_Driver::ErrorCodes ADC_SIM_Driver::init(void) {
    return ErrorCodes::ADS_OK; 
}

ADC_Base_Driver::ErrorCodes ADC_SIM_Driver::set_channel(ADC_Base_Driver::ChannelID channel) {
    return ErrorCodes::ADS_OK;
}

ADC_Base_Driver::ErrorCodes ADC_SIM_Driver::data_read(int32_t *result, uint8_t *status) {
    return ErrorCodes::ADS_OK;
}
