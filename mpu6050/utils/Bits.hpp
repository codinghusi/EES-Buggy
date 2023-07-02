/**
 * Dieser Code ist im Abschlussprojekt des Moduls "Einführung in Embedded Software" entstanden.
 * Hannah Lehnen, Timur Gönül und Gerrit Weiermann
 * Gruppe 18, Slot D
*/

#pragma once

#include "word.hpp"
#include <mutex>

template<class Type = uint8_t>
class Bits {
public:
  /**
   * Referenced register
  */
  Word<Type> data;

  /**
   * Bit position in referenced register
  */
  uint8_t bit_lowest;

  /**
   * Count of bits
   * 
   * Example:
   * bit_lowest = 2;
   * bit_highest = 5;
   * Represented value takes bits data[5:2]
  */
  uint8_t bit_highest;

  /**
   * Bit masks will be calculated automatically
   * Helps during bit manipulations
  */
  Type bit_mask;

public:
  /**
   * Empty constructor
  */
  Bits() {}

  /**
   * Constructor for initialization
  */
  Bits(Word<Type> data, uint8_t bit_highest, uint8_t bit_lowest): data(data), bit_highest(bit_highest), bit_lowest(bit_lowest) {
    bit_mask = ((1 << (bit_highest)) - 1) & ~((1 << bit_lowest) - 1);
  }

  /**
   * Constructor for bit-length of 1
  */
  Bits(Word<Type> data, uint8_t bit_highest): Bits(data, bit_highest, bit_highest) {}

  /**
   * Copy assign operator
  */
  Bits& operator=(Bits<Type> bits) {
    data = bits.data;
    bit_position = bits.bit_position;
    bit_length = bit_length;
    bit_mask = bit_mask;
    return *this;
  }

  /**
   * For typecast so it behaves like it is `Type`
  */
  operator Type() {
    return (((Type) (data & bit_mask)) >> bit_position);
  }

  /**
   * Only writes to specified bits
   * FIXME: data races
  */
  Bits& operator=(Type value) {
    Type val = data;
    Type before = val;
    val &= ~(bit_mask);
    val |= (value << bit_position);
    data = val;
    return *this;
  }
};
