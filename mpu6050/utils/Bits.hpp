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
  Word<Type> data;
  uint8_t bit_position;
  uint8_t bit_length;
  Type bit_mask;

public:
  Bits() {}
  Bits(Word<Type> data, uint8_t bit_highest, uint8_t bit_lowest): data(data), bit_position(bit_lowest), bit_length(bit_highest - bit_lowest + 1) {
    bit_mask = ((1 << (bit_position + bit_length)) - 1) & ~((1 << bit_position) - 1);
  }

  Bits(Word<Type> data, uint8_t bit_highest): Bits(data, bit_highest, bit_highest) {}

  operator Type() {
    return (((Type) (data & bit_mask)) >> bit_position);
  }

  Bits& operator=(Bits<Type> bits) {
    data = bits.data;
    bit_position = bits.bit_position;
    bit_length = bit_length;
    bit_mask = bit_mask;
    return *this;
  }

  Bits& operator=(Type value) {
    Type val = data;
    Type before = val;
    val &= ~(bit_mask);
    val |= (value << bit_position);
    data.write_nolock(val);
    return *this;
  }
};
