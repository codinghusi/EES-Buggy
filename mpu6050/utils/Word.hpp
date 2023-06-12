/**
 * Dieser Code ist im Abschlussprojekt des Moduls "Einführung in Embedded Software" entstanden.
 * Hannah Lehnen, Timur Gönül und Gerrit Weiermann
 * Gruppe 18, Slot D
*/

#pragma once

#include <stdint.h>
#include <wiringPiI2C.h>
#include <thread>
using namespace std::chrono_literals;
using namespace std::this_thread;

/**
 * Don't use complex dataTypes such as structs (because of endianess)
 */
template<class Type>
class Word {
private:
  uint8_t fd;
  uint8_t location;

  Type read() {
    const uint8_t count = sizeof(Type);
    uint8_t val[sizeof(Type)];
    for (int i = 0; i < count; ++i) {
      val[i] = wiringPiI2CReadReg8(fd, location + (count - 1 - i));
    }
    return *static_cast<Type*>((void*) &val);
  }

public:
  Word() {}
  Word(const int& fd, const uint8_t& location): fd(fd), location(location) {}

  operator Type() {
    return read();
  }

  void write_nolock(Type value) {
    const uint8_t count = sizeof(Type);
    uint8_t* data = (uint8_t*) &value;
    for (int i = 0; i < count; ++i) {
      wiringPiI2CWriteReg8(fd, location + i, data[count - 1 - i]);
    }
  }

  void write(Type value) {
    write_nolock(value);
  }

  Word& operator=(Type value) {
    write(value);
    return *this;
  }

  Word& operator=(Word<Type> w) {
    fd = w.fd;
    location = w.location;
    return *this;
  }
};

typedef Word<uint8_t> Byte;
typedef Word<int16_t> Byte16;
