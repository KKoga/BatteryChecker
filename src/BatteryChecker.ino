#include <M5StickCPlus.h>
#include "AutoPowerOff.h"
#include "Handler.h"

#define SDA 26
#define SCL 0

extern void Annotation();

#define POWEROFF 300
AutoPowerOff autoPowerOff;

Handler handler;

void ensurePowerOff() {
  // オートパワーオフ条件を満たすと電源オフ
  autoPowerOff.checkChanged();
  autoPowerOff.ensure(Annotation);

  // 1秒以上フロントボタンを押すと電源オフ
  if (M5.BtnA.pressedFor(1000)) {
    Annotation();
    M5.Axp.PowerOff();
  }
}

void setup() {
  // I2C通信を行うための共通の初期化（mcp4725/mcp4726/ads1015が利用する共通のピン設定）
  Wire.begin(SDA, SCL);

  setCpuFrequencyMhz(10);
  M5.Axp.ScreenBreath(7);;

  M5.begin();

  // M5.begin()の後で初期化する。前だとRTCがおかしいためにうまく初期化できない。
  // 無操作で3分経過するとパワーオフ
  autoPowerOff = AutoPowerOff(POWEROFF);

  handler.reset();

  M5.Lcd.setTextSize(2);
  M5.Lcd.setRotation(3);
  Annotation();
}

void loop() {
  delay(20);

  M5.update();
  ensurePowerOff();

  // 電池の取り付けー計測ー取り外しを一括管理する（画面表示もそれぞれで行う）
  handler.process();
}
