#include "../../api/phy_base.h"

class PHY_SIM_Driver : public PHY_Driver {
public:
    PHY_Base_Driver::ErrorCodes init(void) override;
    PHY_Base_Driver::ErrorCodes read_mode(uint8_t *mode) override;
    PHY_Base_Driver::ErrorCodes apply_mode(const uint8_t mode) override;
};
