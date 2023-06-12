/**
 * Dieser Code ist im Abschlussprojekt des Moduls "Einführung in Embedded Software" entstanden.
 * Hannah Lehnen, Timur Gönül und Gerrit Weiermann
 * Gruppe 18, Slot D
*/

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
  void clear_interrupt_flag();

  volatile Vec3<float> gyroscope, accelerometer;
  Vec3<float> drift;

public:
  /**
   * @brief Construct a new MPU6050 object
   * @param address The I2C address of the MPU6050. AD0 = LOW -> 0x68, AD0 = HIGH -> 0x69
  */
  MPU6050(uint8_t address = 0x68);

  /**
   * @brief Initialize the MPU6050
  */
  void init();

  /**
   * @brief Initialize the use of the interrupt. Doesn't work without.
   * @param pinNumber The pin number of the interrupt pin
   * @param handler The handler function that will be called when the interrupt is triggered
  */
  void setup_interrupt(uint8_t pinNumber, void (*handler)());

  /**
   * @brief Call this method in the interrupt handler
  */
  void interrupt_triggered();
  
  /**
   * @brief Call when the MPU6050 is not moving. This will calibrate the gyroscope drift that will be automatically subtracted from the gyroscope values
   * @param duration The duration of the calibration
   * @param max_error The maximum error that is allowed
   * @return true if the error exceeded the max_error
  */
  bool calibrate_drift(const std::chrono::duration<float>& duration, const float max_error = 2.f);

  /**
   * @brief Puts all sensors in standby mode except the gyroscope z-axis
  */
  void standby_except_gyro_z();

  /**
   * @return returns the absolute gyroscope values (calculated in background through interrupts)
  */
  Vec3<float> get_gyroscope() const;

  /**
   * @brief @return returns what drift was measured by calibrate_drift(...)
  */
  Vec3<float> get_drift() const;

  /**
   * sets all gyroscope values xyz to 0
  */
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
