#include "MeOrion.h"
#include <SoftwareSerial.h>

uint8_t motorSpeed = 100;
uint8_t motorSpeed_L = 90;

MeDCMotor motorR(M1); 
MeDCMotor motorL(M2); 


// ポート定義
MeInfraredReceiver infraredReceiverDecode(PORT_4); // IRレシーバをPORT_4に接続

// 関数プロトタイプ
void Forward();
void Backward();
void TurnRight();
void TurnLeft();
void Stop();
// void TurnLight(); // 必要に応じて有効化

void setup() {
  infraredReceiverDecode.begin(); // IRレシーバの初期化
  Serial.begin(9600); // シリアル通信開始
  Serial.println("InfraredReceiverDecode Start!");
  Stop(); // 初期状態で停止
}

void loop() {

  Forward();  // 前進
  Serial.println("Forward");

  delay(2000);

  TurnRight(); // 右折
  Serial.println("Right");

  delay(2000);

  Forward();  // 前進
  Serial.println("Forward");

  delay(2000);

  TurnRight(); // 右折
  Serial.println("Right");

  delay(2000);

  Forward();  // 前進
  Serial.println("Forward");

  delay(2000);

  TurnRight(); // 右折
  Serial.println("Right");

  delay(2000);

  Forward();  // 前進
  Serial.println("Forward");

  Stop();
  Serial.println("stop");



}

// 前進
void Forward() {
  motorL.run(motorSpeed);  // 左モーター前進
  motorR.run(motorSpeed);  // 右モーター前進
}

// 後退
void Backward() {
  motorL.run(-motorSpeed);  // 左モーター前進
  motorR.run(-motorSpeed);  // 右モーター前進
}

// 右折
void TurnRight() {
 
  motorL.run(motorSpeed_L);  // 左モーター前進
  motorR.run(-motorSpeed_L);  // 右モーター前進
}

// 左折
void TurnLeft() {

  motorL.run(-motorSpeed);  // 左モーター前進
  motorR.run(motorSpeed);  // 右モーター前進
}

// 停止
void Stop() {

  motorR.stop();   
  motorL.stop();
}

/*
// ライト点灯/消灯（必要に応じて有効化）
void TurnLight() {
  // TODO: ライト処理を実装
  // 例: MeRGBLed led(PORT_6); led.setColor(255, 255, 255); led.show(); // 白色点灯
}
*/