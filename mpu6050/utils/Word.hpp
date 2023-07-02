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
template <class Type>
class Word
{
private:
  /**
   * I2C handle to the sensor
   */
  uint8_t fd;

  /**
   * Address of referenced register (of sensor)
   */
  uint8_t location;

  /**
   * Reads contents from I2C of referenced register and converts value into `Type`
   *
   * Example: `Type` is uint8_t
   * -> reads first 8-Bit from I2C and converts it to uint8_t
   *
   * Example: `Type` is uint16_t
   * -> reads first 16-Bit from I2C and converts it to uint16_t
   * -> Storing it in reverse order because of endianess
   *
   * @return read value from I2C
   */
  Type read()
  {
    const uint8_t count = sizeof(Type);
    uint8_t val[sizeof(Type)];
    for (int i = 0; i < count; ++i)
    {
      val[i] = wiringPiI2CReadReg8(fd, location + (count - 1 - i));
    }
    return *static_cast<Type *>((void *)&val);
  }

  /**
   * Writes given value to referenced register on I2C
  */
  void write(Type value)
  {
    const uint8_t count = sizeof(Type);
    uint8_t *data = (uint8_t *)&value;
    for (int i = 0; i < count; ++i)
    {
      wiringPiI2CWriteReg8(fd, location + i, data[count - 1 - i]);
    }
  }

public:
  /**
   * Empty constructor
  */
  Word() {}

  /**
   * Constructor for initialization
  */
  Word(const int &fd, const uint8_t &location) : fd(fd), location(location) {}

  /**
   * Copy assign operator
  */
  Word &operator=(Word<Type> w)
  {
    fd = w.fd;
    location = w.location;
    return *this;
  }

  /**
   * Instance behaves like it is `Type`.
   * Implicit typecasts to 'Type' will read the value of I2C and return it
   *
   * Example:
   * uint8_t register_value = MY_REGISTER; // MY_REGISTER has type `Word<uint8_t>`
   */
  operator Type()
  {
    return read();
  }

  /**
   * Instance behaves like it is `Type`
   * = operator will automatically send the given value to I2C
   * 
   * Example:
   * MY_REGISTER = 20; // MY_REGISTER has type `Word<uint8_t>`, 20 is sent to I2C
  */
  Word &operator=(Type value)
  {
    write(value);
    return *this;
  }
};

typedef Word<uint8_t> Byte;
typedef Word<int16_t> Byte16;
