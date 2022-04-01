#ifndef __ADS1015_h__
#define __ADS1015_h__

#include "Arduino.h"
#include <Wire.h>

class ADS1015 {
  public:
    ADS1015() {};
    int getVoltage(uint8_t pin);
};

class ADS1015Dummy: public ADS1015 {
  public:
    ADS1015Dummy() {};
    int getVoltage(uint8_t pin) {
      return 0;
    };
};

#endif
