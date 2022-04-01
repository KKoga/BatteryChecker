#include <M5StickCPlus.h>
#include "Handler.h"
#include <Arduino.h>

extern BatteryProfile Battery_1_5;
extern BatteryProfile Battery_3;
extern BatteryProfile Battery_dummy;
extern MCP472x mcp4725;
extern MCP472x mcp4726;
extern ADS1015 ads1015;

Handler::Handler() {
  mode = INIT;
}

Handler::Mode Handler::init() {
  M5.Lcd.setTextSize(2);
  M5.Lcd.fillRect(0, 0, 239, 134, BLACK);
  M5.Lcd.drawString("Battery Checker", 10, 10);
  return WAIT_BATTERY_SET;
}

void Handler::reset() {
  mcp4725.setVoltage(0);
  mcp4726.setVoltage(0);
}

Handler::Mode Handler::wait_set_battery() {
  battery.reset(&Battery_1_5);
  int v0 = battery.getSourceVoltage();
  int v1 = battery.getCollectorVoltage();
  if (v0 > 500 && v0 < 2000) {
    // 1.5V電源投入を検知（浮遊電圧をうまく解消できないので2V以上は無視）
    // 実計測すると1Ωの抵抗に対して値が1000のとき410mA、2000のとき853mAとなっている。
    // 比例するのであれば1500で600mA、1600で660mA、1700で720mA、1800で780mAとなりそうなので、
    // 1600あたりまで0, 400, 800, 1200, 1600と5ステップ計測する。
    battery.start();
    return MEASURE;
  }

  battery.reset(&Battery_3);
  int v2 = battery.getSourceVoltage();
  int v3 = battery.getCollectorVoltage();
  if (v2 > 500 && v2 < 3000) {
    // 3V電源投入を検知
    battery.start();
    return MEASURE;
  }
  return mode;
}

Handler::Mode Handler::wait_remove_battery() {
  int v0 = battery.getSourceVoltage();
  if (v0 < 500 || v0 > 2000) {
    battery.reset(&Battery_dummy);
    return INIT;
  }

  return mode;
}

Handler::Mode Handler::process() {
  switch (mode) {
    case INIT:
      mode = init();
      break;
    case WAIT_BATTERY_SET:
      mode = wait_set_battery();
      break;
    case MEASURE:
      if (battery.next() == Battery::END) {
        printf("wait remove battery\n");
        mode = TERMINATED;
      }
      break;
    case TERMINATED:
      mode = wait_remove_battery();
      break;
    default:
      printf("unknown state %d\n", mode);
      break;
  }
  return mode;
}
