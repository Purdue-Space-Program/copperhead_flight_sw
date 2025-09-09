#include <imu_base.h>

class IMU_SIM_Driver : public IMU_Base_Driver
{
  public:
    etl::expected<void, ErrorCode> acceleration_init(void) override;
    etl::expected<Acceleration_Vector_mg, ErrorCode> get_acceleration(void) override;
};
