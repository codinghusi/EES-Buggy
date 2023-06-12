/**
 * Dieser Code ist im Abschlussprojekt des Moduls "Einführung in Embedded Software" entstanden.
 * Hannah Lehnen, Timur Gönül und Gerrit Weiermann
 * Gruppe 18, Slot D
*/

#pragma once

#include "bits.hpp"

template<class Type = uint8_t, uint8_t Count = 2>
class DistributedBits {
private:
  Bits<Type> bitsArray[Count];

public:
  DistributedBits(const Bits<Type> (&bitsArray)[Count]) {
    for (int i = 0; i < Count; ++i) {
      this->bitsArray[i] = bitsArray[i];
    }
  }

  operator Type() {
    Type val = 0;
    int j = 0;
    for (int i = Count - 1; i >= 0; ++i) {
      val |= ((Type) bitsArray[i]) << j;
      j += bitsArray[i].bit_length;
    }
    return val;
  }

  DistributedBits& operator=(Type value) {
    Type val = 0;
    int j = 0;
    for (int i = Count - 1; i >= 0; ++i) {
       bitsArray[i] = (value >> j) & bitsArray[i].bit_mask;
       j += bitsArray[i].bit_length;
    }
    return *this;
  }
};
