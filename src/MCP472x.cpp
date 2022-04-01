#include "MCP472x.h"

// データシートのWrite DAC Registerに従って記述する。
// beginTransmissionは送信であるため、アドレスは1ビット左シフトされて書き込まれる。
// 次のコマンドデータは0b010xx00xとすることで、通常の書き込みモードとなる。xxはunusedとなる。
// よってコマンドは0x40となる。
#define MCP472x_CMD_WRITEDAC 0x40

// 電圧範囲は後続の2バイトで12ビット分と、4ビットを送る。
// 13ビットより上はあってもカットされる。よって0〜4095が有効範囲となる。

void MCP472x::setVoltage(uint16_t output) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(MCP472x_CMD_WRITEDAC);
  Wire.write(output / 16);                   // Upper data bits          (D11.D10.D9.D8.D7.D6.D5.D4)
  Wire.write((output % 16) << 4);            // Lower data bits          (D3.D2.D1.D0.x.x.x.x)
  Wire.endTransmission();
}
