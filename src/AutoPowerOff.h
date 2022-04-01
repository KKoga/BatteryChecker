#ifndef __AutoPowerOff_h__
#define __AutoPowerOff_h__

#include <M5StickCPlus.h>

class AutoPowerOff {
  public:
    AutoPowerOff() {};
    AutoPowerOff(uint16_t nsec);
    void getDateTime();
    void checkChanged();  // 今はボタンが押されたことを確認
    uint32_t getDiff();
    void ensure(void (*callback)());
  private:
    uint16_t nsec;
    RTC_DateTypeDef curdate;
    RTC_DateTypeDef nowdate;
    RTC_TimeTypeDef curtime;
    RTC_TimeTypeDef nowtime;
    void copyDateTime();
};

#endif
