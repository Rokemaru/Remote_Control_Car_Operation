#include <Servo.h>

// ======================================================
// 設定値
// ======================================================
const int SERVO_PIN = 9;      // サーボモーターを接続するピン番号
const long BAUD_RATE = 9600;  // シリアル通信の速度

// コマンドに対応するサーボの角度
const int ANGLE_CMD_A = 0;   // 開く
const int ANGLE_CMD_B = 90;  // 閉じる


// ======================================================
// プログラム本体 (修正版)
// ======================================================
// 修正点：
// - '<CMD>' 形式のコマンドを受信・解釈するロジックに変更
// - ノイズなど不正なデータを無視し、安定性を向上
// ======================================================

Servo myservo;

// --- コマンド受信用の状態管理 ---
String receivedCommand;
bool receiving = false;

void setup() {
  // PCとのシリアル通信を開始
  Serial.begin(BAUD_RATE);

  // サーボの準備
  myservo.attach(SERVO_PIN);

  // 起動時は閉じる(90度)に設定
  myservo.write(ANGLE_CMD_B);

  // 準備ができたことをシリアルモニターに表示
  Serial.println("サーボモーター制御システム 起動 (修正版)");
  Serial.println("コマンド (<A>, <B>) を待っています...");
}

void loop() {
  // シリアルポートからデータを受信処理
  while (Serial.available() > 0) {
    char receivedChar = Serial.read();

    if (receivedChar == '<') {
      // 開始文字を受け取ったら、受信開始
      receivedCommand = "";
      receiving = true;
    } else if (receiving && receivedChar == '>') {
      // 終了文字を受け取ったら、受信完了
      receiving = false;

      // 受信したコマンドを処理
      processCommand(receivedCommand);

    } else if (receiving) {
      // 受信中であれば、文字をコマンドに追加
      receivedCommand += receivedChar;
    }
  }
}

// 受信したコマンドを解釈してサーボを動かす関数
void processCommand(String cmd) {
  Serial.print("受信コマンド: <");
  Serial.print(cmd);
  Serial.println(">");

  if (cmd.equalsIgnoreCase("A")) {
    // 現在の角度がAでなければ、サーボを動かす
    if (myservo.read() != ANGLE_CMD_A) {
      myservo.write(ANGLE_CMD_A);
      Serial.println("サーボを開きました (0度)");
    }
  } else if (cmd.equalsIgnoreCase("B")) {
    // 現在の角度がBでなければ、サーボを動かす
    if (myservo.read() != ANGLE_CMD_B) {
      myservo.write(ANGLE_CMD_B);
      Serial.println("サーボを閉じました (90度)");
    }
  } else {
    Serial.println("未定義のコマンドです。");
  }
}
