#include <Servo.h>

// ======================================================
// 設定値
// ======================================================
const int SERVO_PIN = 9;      // サーボモーターを接続するピン番号
const long BAUD_RATE = 9600;  // シリアル通信の速度

// コマンドに対応するサーボの角度
const int ANGLE_CMD_A = 0;
const int ANGLE_CMD_B = 90;


// ======================================================
// プログラム本体
// ======================================================

Servo myservo;

// 【追加】最後に実行したコマンドを記憶しておく変数
char lastCommand = ' '; 

void setup() {
  // PCとのシリアル通信を開始
  Serial.begin(9600);
  
  // サーボの準備
  myservo.attach(SERVO_PIN);
  
  // 起動時は真ん中(90度)に設定
  myservo.write(ANGLE_CMD_B);
  lastCommand = 'B'; // 最初の状態を記録しておく

  // 準備ができたことをシリアルモニターに表示
  Serial.println("サーボモーター制御システム 起動 (最終修正版)");
  Serial.println("コマンド (A, B) を待っています...");
}

void loop() {
  // もしシリアルモニターから何かしらのデータが送信されていたら
  if (Serial.available() > 0) {
    // 送信されたデータを1文字読み込む
    char command = Serial.read();

    // 受信したコマンドをシリアルモニターに表示
    Serial.print("受信コマンド: ");
    Serial.println(command);

    // 受信したコマンドに応じて、サーボを動かす角度を決定する
    switch (command) {
      case 'A':
      case 'a':
        // 【修正】前回と違うコマンドの場合だけ、サーボを動かす
        if (lastCommand != 'A') {
          myservo.write(ANGLE_CMD_A);
          Serial.println("角度を 0 度にしました");
          lastCommand = 'A'; // 今回実行したコマンドを記録
        }
        break;
      
      case 'B':
      case 'b':
        // 【修正】前回と違うコマンドの場合だけ、サーボを動かす
        if (lastCommand != 'B') {
          myservo.write(ANGLE_CMD_B);
          Serial.println("角度を 90 度にしました");
          lastCommand = 'B'; // 今回実行したコマンドを記録
        }
        break;
    }
  }
}