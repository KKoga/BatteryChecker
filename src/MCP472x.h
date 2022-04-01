#ifndef __MCP472x_h__
#define __MCP472x_h__

#include "Arduino.h"
#include <Wire.h>

// チップのアドレス（チップと回路依存）
#define MCP_472x_ADDR_4726 0x60    // 4726は0x60に固定
#define MCP_472x_ADDR_4725 0x61    // 4725は基盤のジャンパまたはプログラム設定依存

class MCP472x {
  public:
    MCP472x() {};
    MCP472x(uint8_t i2caddr): MCP472x() {
      _i2caddr = i2caddr;
    };
    void setVoltage(uint16_t output);

  private:
    uint8_t _i2caddr;
};

class MCP472xDummy: public MCP472x {
  public:
    MCP472xDummy() {};
    void setVoltage(uint16_t output) {};
};

#endif
