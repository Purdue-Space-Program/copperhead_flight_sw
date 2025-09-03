#include "../../api/adc_base.h"


class ADC_SIM_Driver : public ADC_Base_Driver {
public:

    ErrorCodes init(void) override;
    ErrorCodes set_channel(ChannelID channel) override;
    ErrorCodes data_read(int32_t *result, uint8_t *status) override;
};
