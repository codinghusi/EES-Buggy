#include <iostream>
#include <chrono>
#include <thread>
#include "mpu6050.h"
#include <wiringPi.h>
#include <mutex>

using namespace std;
using namespace std::chrono_literals;
using namespace std::this_thread;

mutex mtx;

MPU6050 gyro;
const uint8_t GYRO_INTERRUPT_PIN = 22;

void gyroInterrupt() {
  lock_guard<mutex> guard(mtx);
  gyro.interrupt_triggered();
}

void loop();
void setup();

void setup() {
  {
    lock_guard<mutex> guard(mtx);
    wiringPiSetup();
    gyro.init();
    gyro.setup_interrupt(GYRO_INTERRUPT_PIN, gyroInterrupt);  
  }
  
  sleep_for(1s);

  // --- Calibrate ---

  cout << "Calibrating..." << endl
       << "Don't move!" << endl;
      
  while (!gyro.calibrate_drift(1s)) {
    cout << "Like I said... DON'T the fuck MOVE!" << endl;
  }

  cout << "OK." << endl;

  gyro.gyroscope.x = 0;
  gyro.gyroscope.y = 0;
  gyro.gyroscope.z = 0;


  // --- End Calibrate ---

  // --- Some Experiments
  // Configure low pass filter
  // gyro.DLPF_CFG = 3; // 100Hz, 2ms delay
  // gyro.DLPF_CFG = 6;

  // --- End Some Experiments ---

  system("clear");
  cout << "calculated drift: " << gyro.drift << endl;

}

void loop() {
  cout << '\r';
  cout << (int) gyro.gyroscope.x << "   ";
  cout << (int) gyro.gyroscope.y << "   ";
  cout << (int) gyro.gyroscope.z << "   ";
  cout.flush();
  sleep_for(100ms);
}
