#include "MeOrion.h"
#include <SoftwareSerial.h>

uint8_t motorSpeed = 100;
uint8_t motorSpeed_L = 90;

MeDCMotor motorR(M1);
MeDCMotor motorL(M2);


// ポート定義
MeInfraredReceiver infraredReceiverDecode(PORT_4);  // IRレシーバをPORT_4に接続

// void TurnLight(); // 必要に応じて有効化

void setup() {
  infraredReceiverDecode.begin();  // IRレシーバの初期化
  Serial.begin(9600);              // シリアル通信開始
  Serial.println("InfraredReceiverDecode Start!");
  Stop();  // 初期状態で停止
}

void loop() {
  if (infraredReceiverDecode.available() || infraredReceiverDecode.buttonState()) {
    switch (infraredReceiverDecode.read()) {
      case IR_BUTTON_UP:
        Forward();  // 前進させる処理
        break;
      case IR_BUTTON_DOWN:
        Backward();  // 後進させる処理
        break;
      case IR_BUTTON_RIGHT:
        TurnRight();  //右旋回させる処理
        break;
      case IR_BUTTON_LEFT:
        TurnLeft();  // 左旋回させる処理
        break;
    }
  } else {
    Stop();  // 停止する処理
  }
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

  motorL.run(motorSpeed_L);   // 左モーター前進
  motorR.run(-motorSpeed_L);  // 右モーター前進
}

// 左折
void TurnLeft() {

  motorL.run(-motorSpeed);  // 左モーター前進
  motorR.run(motorSpeed);   // 右モーター前進
}

// 停止
void Stop() {

  motorR.stop();
  motorL.stop();
}