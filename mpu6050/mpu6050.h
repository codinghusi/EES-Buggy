#include "utils/distributed_bits.hpp"
#include "utils/word.hpp"
#include "utils/bits.hpp"
#include "utils/vec3.hpp"
#include <chrono>
#include <mutex>

// TODO: implement Master/Slave

class MPU6050 {
private:
  int fd;
  std::chrono::time_point<std::chrono::steady_clock> last_measurement;
  std::mutex mtx;

  float convert_raw_gyroscope_value(float oldValue, float value, float drift, float delta);
  float convert_raw_accelerometer_value(float value, float delta);

  volatile Vec3<float> gyroscope, accelerometer;
  Vec3<float> drift;

public:
  MPU6050(uint8_t address = 0x68);

  void setup_interrupt(uint8_t pinNumber, void (*handler)());
  void interrupt_triggered();
  void init();
  void clear_interrupt_flag();
  bool calibrate_drift(const std::chrono::duration<float>& duration, const float max_error = 2.f);

  void standby_except_gyro_z();
  Vec3<float> get_gyroscope() const;
  Vec3<float> get_drift() const;
  void reset_gyroscope();

  Byte SELF_TEST_X;
  Byte SELF_TEST_Y;
  Byte SELF_TEST_Z;
  Byte SELF_TEST_A;
  DistributedBits<> XA_TEST;
  DistributedBits<> YA_TEST;
  DistributedBits<> ZA_TEST;
  Bits<> XG_TEST;
  Bits<> YG_TEST;
  Bits<> ZG_TEST;

  Byte SMPLRT_DIV;

  Byte CONFIG;
  Bits<> EXT_SYNC_SET;
  Bits<> DLPF_CFG;

  Byte GYRO_CONFIG;
  Bits<> FS_SEL;
  Bits<> XG_ST;
  Bits<> YG_ST;
  Bits<> ZG_ST;

  Byte ACCEL_CONFIG;
  Bits<> AFS_SEL;
  Bits<> XA_ST;
  Bits<> YA_ST;
  Bits<> ZA_ST;

  Byte FIFO_EN_REG;
  Bits<> TEMP_FIFO_EN;
  Bits<> XG_FIFO_EN;
  Bits<> YG_FIFO_EN;
  Bits<> ZG_FIFO_EN;
  Bits<> ACCEL_FIFO_EN;

  Byte INT_PIN_CFG;
  Bits<> INT_LEVEL;
  Bits<> INT_OPEN;
  Bits<> LATCH_INT_EN;
  Bits<> INT_RD_CLEAR;
  Bits<> FSYNC_INT_LEVEL;
  Bits<> FSYNC_INT_EN;

  Byte INT_ENABLE;
  Bits<> FIFO_OFLOW_EN;
  Bits<> DATA_RDY_EN;

  Byte INT_STATUS;
  Bits<> FIFO_OFLOW_INT;
  Bits<> DATA_RDY_INT;

  Byte16 ACCEL_X;
  Byte16 ACCEL_Y;
  Byte16 ACCEL_Z;

  Byte16 TEMP;

  Byte16 GYRO_X;
  Byte16 GYRO_Y;
  Byte16 GYRO_Z;

  Byte SIGNAL_PATH_RESET;
  Bits<> GYRO_RESET;
  Bits<> ACCEL_RESET;
  Bits<> TEMP_RESET;

  Byte USER_CTRL;
  Bits<> FIFO_EN_BIT;
  Bits<> FIFO_RESET;
  Bits<> SIG_COND_RESET;

  Byte PWR_MGMT_1;
  Bits<> DEVICE_RESET;
  Bits<> SLEEP;
  Bits<> CYCLE;
  Bits<> TEMP_DIS;
  Bits<> CLKSEL;

  Byte PWR_MGMT_2;
  Bits<> LP_WAKE_CTRL;
  Bits<> STBY_XA;
  Bits<> STBY_YA;
  Bits<> STBY_ZA;
  Bits<> STBY_XG;
  Bits<> STBY_YG;
  Bits<> STBY_ZG; 

  Byte FIFO_COUNT;
  Byte FIFO_R_W;
  Byte FIFO_DATA;
  Bits<> WHO_AM_I;
};
