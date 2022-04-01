#include "ADS1015.h"

// チップのアドレス（チップと基板実装依存）
#define ADDR_1015 0x48  // ADRをGNDに接続した場合

// pinはA0-A3を選択するのに必要
int ADS1015::getVoltage(uint8_t pin) {
  // コンフィグレジスタに値を書き込む
  Wire.beginTransmission(ADDR_1015);  // I2C Address
  Wire.write(0B00000001);
  {
    int os = 1;               // パワーダウンしていたらスイッチを入れる
    //100(4) A0-GND
    //101(5) A1-GND
    //110(6) A2-GND
    //111(7) A3-GND
    int mux = 4 | (pin & 3);  // マルチプレクサの設定を指定ピンとGNDの間でONにする
    int pga = 2;              // プログラマブルゲインをデフォルトとする
    int mode = 1;             // モードはシングルショット比較とする（一度比較するとオフになる）
    int m = (os << 7) | (mux << 4) | (pga << 1) | mode;
    Wire.write(m);
  }
  {
    int dr = 4;               // データレート（4=1600SPS）
    int que = 3;              // キュー（コンパレータキューは無効とする）
    int m = (dr << 5) | que;
    Wire.write(m);
  }
  Wire.endTransmission();

  // コンバージョンレジスタを読み出しの対象とする
  Wire.beginTransmission(ADDR_1015);
  Wire.write(0B00000000);
  Wire.endTransmission();

  // 値が2バイト用意されているはずなので読み出す。
  // このあたりは振る舞いがよくわからないのでコードを直接見て推測した。
  int n = Wire.requestFrom(ADDR_1015, 2);
  if (n != 2) {
    return -1;  // エラー
  }

  int high = Wire.read();
  int low = Wire.read();
  if (low == -1 || high == -1) {
    return -1;  // エラー
  }

  //printf("low=%d, high=%d\n", low, high);

  return (high << 4) | (low >> 4);
}
