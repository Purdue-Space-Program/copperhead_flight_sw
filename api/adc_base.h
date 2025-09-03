#pragma once
#include <stdint.h>
#include <stddef.h>


class ADC_Base_Driver {
public:

    enum class ErrorCodes {
        ADS_OK,
        ADS_ERR,
        ADS_RSPI_ERR,
        ADS_WSPI_ERR,
    };

    enum class ChannelID {
        ADS_CH_DIFF0 = 0x00,
        ADS_CH_DIFF1 = 0x01,
        ADS_CH_DIFF2 = 0x02,
        ADS_CH_DIFF3 = 0x03,
        ADS_CH_DIFF4 = 0x04,
        ADS_CH_DIFF5 = 0x05,
        ADS_CH_DIFF6 = 0x06,
        ADS_CH_DIFF7 = 0x07,
    };

    virtual ~ADC_Base_Driver() = default;
    virtual ErrorCodes init(void) = 0;
    virtual ErrorCodes set_channel(ChannelID channel) = 0;
    virtual ErrorCodes data_read(int32_t *result, uint8_t *status) = 0;
};
