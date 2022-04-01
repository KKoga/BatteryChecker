#ifndef __Battery_h__
#define __Battery_h__

#include "MCP472x.h"
#include "ADS1015.h"

// 電流計算の係数（回路の分圧と負荷抵抗に依存する）
#define FACTOR_1_5V (1/1.0)
#define FACTOR_3V (2/500.0)
#define FACTOR_NA (0)

#define MULTIPLY_1_5V (1.0)
#define MULTIPLY_3V (2.0)
#define MULTIPLY_NA (0)

// バッテリプロファイル設定
// 計測のための値範囲、計測ピン番号、対象DAC/ADCを保持する
struct BatteryProfile {
  char *name;
  float factor;       // 電流計算の係数
  float multiply;     // 電圧補正係数
  int precision;      // 電流表示時の小数点以下の桁数
  int min;
  int max;
  int step;
  int sourcePin;
  int collectorPin;
  MCP472x dac;
  ADS1015 ads1015;
};

class Battery {
  public:
    enum State {
      MEASURE,      // 計測中
      END           // 終了
    };

    Battery();

    void reset(BatteryProfile *profile);
    void start();
    void setVoltage(int value);
    int getSourceVoltage();
    int getCollectorVoltage();

    State next();  // 戻り値で継続または終了を検知する

    BatteryProfile *profile;

  private:
    State state;
    int value;
    int showPosition;
    void showStatus(int current, int v0, int v1);
    void nextPosition();
};

#endif
