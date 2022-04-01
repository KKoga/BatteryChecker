#include <M5StickCPlus.h>
#include "Battery.h"

// バッテリーの電圧モニタに必要なDAC/ADC設定
// ハードウェアは一つずつしかないので一つだけ対応するものを定義する
ADS1015 ads1015;
MCP472x mcp4725(MCP_472x_ADDR_4725);
MCP472x mcp4726(MCP_472x_ADDR_4726);

// NULL代わりの設定用
static ADS1015Dummy ads1015Dummy;
static MCP472xDummy mcp472xDummy;

extern BatteryProfile Battery_dummy;

BatteryProfile Battery_1_5 = {
  "1.5V",
  FACTOR_1_5V,
  MULTIPLY_1_5V,
  0,
  500, 1700, 400,
  0, 1,
  mcp4725,
  ads1015
};

BatteryProfile Battery_3 = {
  "CR2032",
  FACTOR_3V,
  MULTIPLY_3V,
  2,
  600, 1600, 300,
  2, 3,
  mcp4726, ads1015
};

BatteryProfile Battery_dummy = {
  "dummy",
  FACTOR_NA,
  MULTIPLY_NA,
  0,
  0, 0, 1,
  -1, -1,
  mcp472xDummy, ads1015Dummy
};

void Annotation() {
  M5.Beep.tone(3300); // 329.628
  delay(100);
  M5.Beep.tone(2600); // 329.628
  delay(50);
  M5.Beep.mute();
}

Battery::Battery() {
  this->profile = &Battery_dummy;
  this->state = END;
  this->value = 0;
};

// プロファイルを受け取って電圧を初期状態にする
void Battery::reset(BatteryProfile *profile) {
  this->profile = profile;
  this->value = this->profile->min;
  this->profile->dac.setVoltage(this->value);
  this->state = MEASURE;
}

void Battery::start() {
  char buf[64];
  sprintf(buf, "%s :\n", this->profile->name);
  printf("%s", buf);
  M5.Lcd.fillRect(0, 0, 239, 134, BLACK);
  this->showPosition = 10;
  M5.Lcd.drawString(buf, 10, this->showPosition);
  nextPosition();
}

void Battery::setVoltage(int value) {
  this->profile->dac.setVoltage(value);
}

int Battery::getSourceVoltage() {
  return this->profile->ads1015.getVoltage(this->profile->sourcePin);
}

int Battery::getCollectorVoltage() {
  return this->profile->ads1015.getVoltage(this->profile->collectorPin);
}

Battery::State Battery::next() {
  int v0 = this->getSourceVoltage();
  int v1 = this->getCollectorVoltage();
  this->showStatus(this->value, v0, v1);
  this->value += profile->step;
  if (this->value <= profile->max) {
    this->setVoltage(this->value);
    return MEASURE;
  } else {
    this->value = 0;
    this->setVoltage(this->value);
    Annotation();
    return END;
  }
}

void Battery::showStatus(int value, int v0, int v1) {
  float v = v0 * this->profile->multiply / 1000;
  float mA = (v0 - v1) * this->profile->factor;

  char buf[64];
  sprintf(buf, " %.2fV %.*fmA\n", v, this->profile->precision, mA);
  printf("%s", buf);
  M5.Lcd.drawString(buf, 10, this->showPosition);
  nextPosition();
}

void Battery::nextPosition() {
  this->showPosition += 20;
}
