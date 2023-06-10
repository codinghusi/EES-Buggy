#include "mpu6050.h"
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <math.h>
#include <chrono>
#include <iostream>
#include "../config.h"

// TODO: implement Master/Slave

void MPU6050::init() {
  std::lock_guard<std::mutex> lock(mtx);

  DEVICE_RESET = 1;
  sleep_for(10ms);
  SLEEP = 0;
  FS_SEL = GYRO_FS_SEL;
}

void MPU6050::setupInterrupt(uint8_t pinNumber, void (*handler)()) {
  std::lock_guard<std::mutex> lock(mtx);

  INT_LEVEL = 0; // Active HIGH
  INT_OPEN = 0; // Push Pull
  LATCH_INT_EN = 0; // 50us long pulse on interrupt
  INT_RD_CLEAR = 0; // Pending Register only cleared when reading INT_STATUS
  DATA_RDY_EN = 1; // Trigger interrupt when new sensor data is ready
  pinMode(pinNumber, INPUT);
  pullUpDnControl(pinNumber, PUD_DOWN);
  wiringPiISR(pinNumber, INT_EDGE_RISING, handler);
  last_measurement = std::chrono::steady_clock::now();
  sleep_for(50ms);
  clear_interrupt_flag();
}

bool MPU6050::calibrate_drift(const std::chrono::duration<float>& duration, const float maxError) {
  Vec3<float> before = {gyroscope.x, gyroscope.y, gyroscope.z};
  sleep_for(duration);
  Vec3<float> after = {gyroscope.x, gyroscope.y, gyroscope.z};
  Vec3<float> delta = (after - before) / duration.count();
  if (delta.abs() > maxError) {
    return false;
  }
  drift = delta;
  return true;
}

float MPU6050::convert_raw_gyroscope_value(float oldValue, float value, float drift, float delta) {
  constexpr float alpha = 0.98f;
  constexpr float beta = 1.f - alpha;
  const float mult = delta * (GYRO_RANGE / 32768.f);
  return fmod(alpha * (oldValue + value * mult - drift * delta) + beta * oldValue, 360.f);
}

float MPU6050::convert_raw_accelerometer_value(float value, float delta) {
  constexpr float mult = 2.f / 32768.f;
  return value * mult * delta;
}


void MPU6050::interruptTriggered() {
  std::lock_guard<std::mutex> lock(mtx);
  auto now = std::chrono::steady_clock::now();
  std::chrono::duration<float> duration = now - last_measurement;
  last_measurement = now;
  
  Vec3<int16_t> gyro = { GYRO_X, GYRO_Y, GYRO_Z };
  Vec3<int16_t> accel = { ACCEL_X, ACCEL_Y, ACCEL_Z };

  float delta = duration.count();
  gyroscope.x = convert_raw_gyroscope_value(gyroscope.x, gyro.x, drift.x, delta);
  gyroscope.y = convert_raw_gyroscope_value(gyroscope.y, gyro.y, drift.y, delta);
  gyroscope.z = convert_raw_gyroscope_value(gyroscope.z, gyro.z, drift.z, delta);
  // TODO: remove gravity from accelerometer
  accelerometer.x = convert_raw_accelerometer_value(accel.x, delta);
  accelerometer.y = convert_raw_accelerometer_value(accel.y, delta);
  accelerometer.z = convert_raw_accelerometer_value(accel.z, delta);
  clear_interrupt_flag();
}

void MPU6050::clear_interrupt_flag() {
  (uint8_t) INT_STATUS;
}

MPU6050::MPU6050(uint8_t address):
    fd(wiringPiI2CSetup(address)),

    SELF_TEST_X(fd, 0x0D),
    SELF_TEST_Y(fd, 0x0E),
    SELF_TEST_Z(fd, 0x0F),
    SELF_TEST_A(fd, 0x10),
    XA_TEST({
      Bits<>(SELF_TEST_X, 7, 5),
      Bits<>(SELF_TEST_A, 5, 4)
    }),
    YA_TEST({
      Bits<>(SELF_TEST_Y, 7, 5),
      Bits<>(SELF_TEST_A, 3, 2)
    }),
    ZA_TEST({
      Bits<>(SELF_TEST_Z, 7, 5),
      Bits<>(SELF_TEST_A, 1, 0)
    }),
    XG_TEST(SELF_TEST_X, 4, 0),
    YG_TEST(SELF_TEST_Y, 4, 0),
    ZG_TEST(SELF_TEST_Z, 4, 0),

    SMPLRT_DIV(fd, 0x19),

    CONFIG(fd, 0x1A),
    EXT_SYNC_SET(CONFIG, 5, 3),
    DLPF_CFG(CONFIG, 2, 0),

    GYRO_CONFIG(fd, 0x1B),
    XG_ST(GYRO_CONFIG, 7),
    YG_ST(GYRO_CONFIG, 6),
    ZG_ST(GYRO_CONFIG, 5),
    FS_SEL(GYRO_CONFIG, 4, 3),
    
    ACCEL_CONFIG(fd, 0x1C),
    XA_ST(GYRO_CONFIG, 7),
    YA_ST(GYRO_CONFIG, 6),
    ZA_ST(GYRO_CONFIG, 5),
    AFS_SEL(GYRO_CONFIG, 4, 3),

    FIFO_EN_REG(fd, 0x23),
    TEMP_FIFO_EN(FIFO_EN_REG, 7),
    XG_FIFO_EN(FIFO_EN_REG, 6),
    YG_FIFO_EN(FIFO_EN_REG, 5),
    ZG_FIFO_EN(FIFO_EN_REG, 4),
    ACCEL_FIFO_EN(FIFO_EN_REG, 3),

    INT_PIN_CFG(fd, 0x37),
    INT_LEVEL(INT_PIN_CFG, 7),
    INT_OPEN(INT_PIN_CFG, 6),
    LATCH_INT_EN(INT_PIN_CFG, 5),
    INT_RD_CLEAR(INT_PIN_CFG, 4),
    FSYNC_INT_LEVEL(INT_PIN_CFG, 3),
    FSYNC_INT_EN(INT_PIN_CFG, 2),

    INT_ENABLE(fd, 0x38),
    FIFO_OFLOW_EN(INT_ENABLE, 4),
    DATA_RDY_EN(INT_ENABLE, 0),
    
    INT_STATUS(fd, 0x3A),
    FIFO_OFLOW_INT(INT_STATUS, 4),
    DATA_RDY_INT(INT_STATUS, 0),

    ACCEL_X(fd, 0x3B),
    ACCEL_Y(fd, 0x3D),
    ACCEL_Z(fd, 0x3F),

    TEMP(fd, 0x41),

    GYRO_X(fd, 0x43),
    GYRO_Y(fd, 0x45),
    GYRO_Z(fd, 0x47),

    SIGNAL_PATH_RESET(fd, 0x68),
    GYRO_RESET(SIGNAL_PATH_RESET, 2),
    ACCEL_RESET(SIGNAL_PATH_RESET, 1),
    TEMP_RESET(SIGNAL_PATH_RESET, 0),

    USER_CTRL(fd, 0x6A),
    FIFO_EN_BIT(USER_CTRL, 6),
    FIFO_RESET(USER_CTRL, 2),
    SIG_COND_RESET(USER_CTRL, 0),

    PWR_MGMT_1(fd, 0x6B),
    DEVICE_RESET(PWR_MGMT_1, 7),
    SLEEP(PWR_MGMT_1, 6),
    CYCLE(PWR_MGMT_1, 5),
    TEMP_DIS(PWR_MGMT_1, 3),
    CLKSEL(PWR_MGMT_1, 2, 0),

    PWR_MGMT_2(fd, 0x6C),
    LP_WAKE_CTRL(PWR_MGMT_2, 7, 6),
    STBY_XA(PWR_MGMT_2, 5),
    STBY_YA(PWR_MGMT_2, 4),
    STBY_ZA(PWR_MGMT_2, 3),
    STBY_XG(PWR_MGMT_2, 2),
    STBY_YG(PWR_MGMT_2, 1),
    STBY_ZG(PWR_MGMT_2, 0),

    FIFO_COUNT(fd, 0x72),
    FIFO_R_W(fd, 0x74),
    FIFO_DATA(fd, 0x74),
    WHO_AM_I(Byte(fd, 0x75), 6, 1)
  {
}
