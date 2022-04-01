#include "AutoPowerOff.h"

AutoPowerOff::AutoPowerOff(uint16_t nsec) {
  this->nsec = nsec;
  getDateTime();
  copyDateTime();
}

void AutoPowerOff::getDateTime() {
  M5.Rtc.GetData(&nowdate);
  M5.Rtc.GetTime(&nowtime);
}

void AutoPowerOff::copyDateTime() {
  curdate = nowdate;
  curtime = nowtime;
}

void AutoPowerOff::checkChanged() {
  getDateTime();
  if (M5.BtnA.isPressed() | M5.BtnB.isPressed()) {
    // ボタンが押されたら変化ありとして日時を記憶する
    copyDateTime();
  }
}

uint32_t AutoPowerOff::getDiff() {
  uint32_t s = ((uint32_t)curtime.Hours * 60 + curtime.Minutes) * 60 + curtime.Seconds;
  uint32_t n = ((uint32_t)nowtime.Hours * 60 + nowtime.Minutes) * 60 + nowtime.Seconds;
  if (curdate.Date != nowdate.Date) {
    n += 24 * 60 * 60;    // ずれていたら強制的に1日ずれていることにする
  }
  return n - s;
}

void AutoPowerOff::ensure(void (*callback)()) {
  // 時刻がnsecを超えたらensureが呼ばれた時点でパワーオフする
  getDateTime();
  if (getDiff() >= nsec) {
    callback();
    M5.Axp.PowerOff();
  }
}
