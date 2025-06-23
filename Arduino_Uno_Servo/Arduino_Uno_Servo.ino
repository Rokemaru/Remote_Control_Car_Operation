#include <Servo.h>
// const は、この値がプログラムの途中で変わらない「定数」であることを示します
const int SERVO_PIN = 9;      // サーボモーターを接続するピン番号

const int ANGLE_INITIAL = 90; // 起動時の初期角度（度）
const int ANGLE_A = 0;        // 往復運動する角度その1（度）
const int ANGLE_B = 90;       // 往復運動する角度その2（度）

const int INTERVAL_MS = 1000; // 動作の合間の待機時間（ミリ秒）

Servo myservo; // サーボを扱うための準備

void setup() {
  // 設定したピン番号でサーボの準備をする
  myservo.attach(SERVO_PIN);
  
  // 設定した初期角度に動かす
  myservo.write(ANGLE_INITIAL);
  
  // 設定した時間だけ待機する
  delay(INTERVAL_MS);
}

void loop() {
  // 角度Aに動かして、指定時間待機
  myservo.write(ANGLE_A);
  delay(INTERVAL_MS);

  // 角度Bに動かして、指定時間待機
  myservo.write(ANGLE_B);
  delay(INTERVAL_MS);
}