/**
 * Dieser Code ist im Abschlussprojekt des Moduls "Einführung in Embedded Software" entstanden.
 * Hannah Lehnen, Timur Gönül und Gerrit Weiermann
 * Gruppe 18, Slot D
*/

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
      
  while (!gyro.calibrate_drift(1s, 20.f)) {
    cout << "Please don't move :)" << endl;
  }

  cout << "OK." << endl;

  gyro.reset_gyroscope();


  // --- End Calibrate ---

  // --- Some Experiments
  // Configure low pass filter
  // gyro.DLPF_CFG = 3; // 100Hz, 2ms delay
  // gyro.DLPF_CFG = 6;

  // --- End Some Experiments ---

  system("clear");
  cout << "calculated drift: " << gyro.get_drift() << ", error: " << gyro.get_drift().abs() << endl;

}

void loop() {
  cout << '\r';
  cout << (int) gyro.ACCEL_X;
  cout << (int) gyro.get_gyroscope().x << "   ";
  cout << (int) gyro.get_gyroscope().y << "   ";
  cout << (int) gyro.get_gyroscope().z << "   ";
  cout.flush();
  sleep_for(100ms);
}

int main() {
  setup();
  while (true) {
    loop();
  }
}
