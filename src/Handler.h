#ifndef __Handler_h__
#define __Handler_h__

#include "Battery.h"

class Handler {
  public:
    enum Mode {
      INIT,               // 初期状態
      WAIT_BATTERY_SET,   // 電池セット待ち
      MEASURE,            // 計測中
      TERMINATED          // 終了状態
    };

    Handler();
    Mode init();
    void reset();
    Mode process();

  private:
    Mode mode;
    Battery battery;

    Mode wait_set_battery();
    Mode wait_remove_battery();
};

#endif
