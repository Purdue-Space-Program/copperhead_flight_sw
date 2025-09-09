#include "stm32h7xx_hal.h"
#include <imu_base.h>

extern SPI_HandleTypeDef hspi1;

#define SPI_READ(dst, size) HAL_SPI_Receive(&hspi1, (uint8_t *)dst, size, HAL_MAX_DELAY)
#define SPI_WRITE(src, size) HAL_SPI_Transmit(&hspi1, (uint8_t *)src, size, HAL_MAX_DELAY)

class IMU_MCU_Driver : public IMU_Base_Driver
{
  public:
    enum class IMU_Accel_Reg
    {
        //
        // RO = Read only
        // WO = Write only
        // WR = Read or Write
        //
        ACC_CHIP_ID = 0x00,    // RO
        ACC_ERR_REG = 0x02,    // RO
        ACC_STATUS = 0x03,     // RO
        ACC_X_LSB = 0x12,      // RO
        ACC_X_MSB = 0x13,      // RO
        ACC_Y_LSB = 0x14,      // RO
        ACC_Y_MSB = 0x15,      // RO
        ACC_Z_LSB = 0x16,      // RO
        ACC_Z_MSB = 0x17,      // RO
        SENSOR_TIME_0 = 0x18,  // RO
        SENSOR_TIME_1 = 0x19,  // RO
        SENSOR_TIME_2 = 0x1A,  // RO
        ACC_INT_STAT_1 = 0x1D, // RO
        TEMP_MSB = 0x22,       // RO
        TEMP_LSB = 0x23,       // RO
        FIFO_LENGTH_0 = 0x24,  // RO
        FIFO_LENGTH_1 = 0x25,  // RO
        FIFO_DATA = 0x26,      // RO
        ACC_CONF = 0x40,       // WR
        ACC_RANGE = 0x41,      // WR
        FIFO_DOWNS = 0x45,     // WR
        FIFO_WTM_0 = 0x46,     // WR
        FIFO_WTM_1 = 0x47,     // WR
        FIFO_CONFIG_0 = 0x48,  // WR
        FIFO_CONFIG_1 = 0x49,  // WR
        INT1_IO_CTRL = 0x53,   // WR
        INT2_IO_CTRL = 0x54,   // WR
        INT_MAP_DATA = 0x58,   // WR
        ACC_SELF_TEST = 0x6D,  // WR
        ACC_PWR_CONF = 0x7C,   // WR
        ACC_PWR_CTRL = 0x7D,   // WR
        ACC_SOFTRESET = 0x7E,  // WO
    };

    etl::expected<void, IMU_Base_Driver::ErrorCode> acceleration_init(void) override;
    etl::expected<uint8_t, IMU_Base_Driver::ErrorCode> acceleration_read_register(IMU_Accel_Reg reg);
    etl::expected<void, IMU_Base_Driver::ErrorCode> acceleration_write_register(IMU_Accel_Reg reg, uint8_t value);
    etl::expected<void, IMU_Base_Driver::ErrorCode> acceleration_read_registers(IMU_Accel_Reg starting_reg,
                                                                                uint8_t *buf, uint8_t size);
    etl::expected<IMU_Base_Driver::Acceleration_Vector_mg, IMU_Base_Driver::ErrorCode> get_acceleration(void) override;
};
