/*
 * =================================================================
 * MeOrion 9Vモーター 赤外線リモコン制御プログラム (修正版)
 * =================================================================
 * 接続：
 * ・モータードライバ：ポート1 (制御信号: Pin10, 11 / 電源: V-M, GND)
 * ・赤外線受信機　　：ポート4
 */

#include "MeOrion.h" // MakeBlockのライブラリを読み込む

// === 各種設定 ===
// モータードライバ制御ピン (ポート1に接続した場合)
const int MOTOR_IN1_PIN = 10;
const int MOTOR_IN2_PIN = 11;

// 赤外線受信機 (ポート4に接続)
MeInfraredReceiver infraredReceiverDecode(PORT_4);

void setup() {
  // モーター制御ピンを出力に設定
  pinMode(MOTOR_IN1_PIN, OUTPUT);
  pinMode(MOTOR_IN2_PIN, OUTPUT);

  // 赤外線受信を開始
  infraredReceiverDecode.begin();
  
  // デバッグ用にシリアルモニタを開始（動作確認に便利です）
  Serial.begin(9600);
  Serial.println("Infrared Remote Control Start!");
}

void loop() {
  // 【重要】新しい信号が来たか、またはボタンが押され続けているかを確認
  if (infraredReceiverDecode.available() || infraredReceiverDecode.buttonState()) {
    // 受信したコードを読み取る
    int keyCode = infraredReceiverDecode.read();

    // どのボタンが押されたかに応じて処理を分岐
    switch (keyCode) {
      case IR_BUTTON_UP: // 「上」ボタンが押された場合
        Forward();
        break;
      
      case IR_BUTTON_DOWN: // 「下」ボタンが押された場合
        Backward();
        break;
        
      default:
        // 上下以外のボタン、または不要な信号(0x00, 0xFFなど)の場合は
        // 何もしないで無視する
        break;
    }
  } else {
    // 何もボタンが押されていない場合は停止する
    Stop();
  }
}

// === モーター制御用の関数 ===

// 前進させる関数
void Forward() {
  // Hブリッジの制御則（正転）
  digitalWrite(MOTOR_IN1_PIN, HIGH);
  digitalWrite(MOTOR_IN2_PIN, LOW);
}

// 後進させる関数
void Backward() {
  // Hブリッジの制御則（逆転）
  digitalWrite(MOTOR_IN1_PIN, LOW);
  digitalWrite(MOTOR_IN2_PIN, HIGH);
}

// 停止させる関数
void Stop() {
  // Hブリッジの制御則（ストップ）
  digitalWrite(MOTOR_IN1_PIN, LOW);
  digitalWrite(MOTOR_IN2_PIN, LOW);
}