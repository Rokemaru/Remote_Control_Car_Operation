#include <MeOrion.h>
#include <SoftwareSerial.h>

// =================================================================
// 統合プログラム (修正版)
// =================================================================
// 機能：
// 1. M1/M2による加減速・旋回機能付きの移動
// 2. ポート1に接続した9V補助モーターを一定時間駆動
// 3. 別のArduinoへ信頼性を高めたシリアルコマンドを送信
//
// 修正点：
// - サーボへのコマンドを <CMD> 形式に変更 (例: <A>)
// - サーボの誤動作を防ぐため、デバッグ用のprintlnをコメントアウト
// - リモコンボタンが押された瞬間に1回だけコマンドを送信するよう修正
// =================================================================


// --- モーター定義（移動用）---
MeDCMotor motorR(M1);
MeDCMotor motorL(M2);

// --- IRリモコン ---
MeInfraredReceiver irReceiver(PORT_4);

// --- 補助モーター（9Vモーターをポート1に接続）---
// 資料(IMEE-KISO-TEXT-Ver901.pdf P.48)より、ポート1はピン10, 11に接続
#define AUX_PIN_1 10  // ポート1のS1ピン (Arduino D10)
#define AUX_PIN_2 11  // ポート1のS2ピン (Arduino D11)

// --- 移動用モーターの加減速設定 ---
const uint8_t minSpeed = 0;
const uint8_t maxSpeed = 255;
const uint8_t accelStep = 5;
const uint16_t accelDelay = 1;

// --- 状態管理用の変数 ---
int targetSpeed = 0;
int currentSpeed = 0;
int moveDirection = 0;

// --- 補助モーター動作時間制御用 ---
unsigned long auxMotorStartTime = 0;
const unsigned long auxMotorDuration = 100;  // 補助モーターのON時間(ミリ秒)
bool auxMotorActive = false;

// --- リモコンの連続送信防止用 ---
uint8_t last_ir_cmd = 0;

void setup() {
  // 補助モーターのピンを出力に設定
  pinMode(AUX_PIN_1, OUTPUT);
  pinMode(AUX_PIN_2, OUTPUT);

  // IRリモコンとシリアル通信を開始
  irReceiver.begin();
  Serial.begin(9600);
  // Serial.println("Start Full Control System (Revised)"); // サーボ誤動作防止のためコメントアウト

  // 全てのモーターを停止状態で開始
  stopMotors();
  stopAuxMotor();
}

void loop() {
  uint8_t current_ir_cmd = 0;

  // リモコンからデータを受信した場合
  if (irReceiver.available()) {
    current_ir_cmd = irReceiver.read();
  }

  // ボタンの状態が変化した（押された）瞬間のみ処理
  if (current_ir_cmd != 0 && current_ir_cmd != last_ir_cmd) {

    // =========== ここから各種リモコン操作を割り当て ============

    // --- 補助モーター制御 (ボタン1, 2) ---
    if (current_ir_cmd == IR_BUTTON_1) {
      // Serial.println("Aux Motor: ON (L_H)"); // コメントアウト
      aux_L_H();
      auxMotorActive = true;
      auxMotorStartTime = millis();
    } else if (current_ir_cmd == IR_BUTTON_2) {
      // Serial.println("Aux Motor: ON (H_L)"); // コメントアウト
      aux_H_L();
      auxMotorActive = true;
      auxMotorStartTime = millis();
    }

    // --- シリアル通信 (ボタンA, B) ---
    else if (current_ir_cmd == IR_BUTTON_A) {
      // Serial.println("Serial Command 'A' Sent"); // コメントアウト
      Serial.print("<A>");  // 信頼性を高めたコマンド形式
    } else if (current_ir_cmd == IR_BUTTON_B) {
      // Serial.println("Serial Command 'B' Sent"); // コメントアウト
      Serial.print("<B>");  // 信頼性を高めたコマンド形式
    }

    // --- 移動用モーター制御 (方向キーなど) ---
    switch (current_ir_cmd) {
      case IR_BUTTON_UP:
        targetSpeed = maxSpeed;
        moveDirection = 1;
        break;
      case IR_BUTTON_DOWN:
        targetSpeed = maxSpeed;
        moveDirection = -1;
        break;
      case IR_BUTTON_RIGHT:
        targetSpeed = maxSpeed;
        moveDirection = 2;
        break;
      case IR_BUTTON_LEFT:
        targetSpeed = maxSpeed;
        moveDirection = -2;
        break;
      case IR_BUTTON_E:  // 右カーブ
        targetSpeed = maxSpeed;
        moveDirection = 3;
        break;
      case IR_BUTTON_D:  // 左カーブ
        targetSpeed = maxSpeed;
        moveDirection = -3;
        break;
      case IR_BUTTON_F:  // 右後進カーブ
        targetSpeed = maxSpeed;
        moveDirection = 4;
        break;
      case IR_BUTTON_0:  // 左後進カーブ
        targetSpeed = maxSpeed;
        moveDirection = -4;
        break;
    }
  }

  // ボタンが何も押されていない、または離された場合
  if (!irReceiver.buttonState()) {
    targetSpeed = 0;
    last_ir_cmd = 0;  // コマンドをリセット
  } else {
    // ボタンが押され続けている場合は、最後のコマンドを保持
    last_ir_cmd = current_ir_cmd;
  }


  // --- 補助モーターを一定時間後に自動停止 ---
  if (auxMotorActive && (millis() - auxMotorStartTime > auxMotorDuration)) {
    stopAuxMotor();
    auxMotorActive = false;
    // Serial.println("Aux Motor: Auto OFF"); // コメントアウト
  }

  // --- 移動用モーターの速度を更新し、駆動する ---
  updateSpeed();
  driveMotors();
  delay(accelDelay);
}

// 移動用モーターの速度を滑らかに変化させる関数
void updateSpeed() {
  if (currentSpeed < targetSpeed) {
    currentSpeed += accelStep;
    if (currentSpeed > targetSpeed) currentSpeed = targetSpeed;
  } else if (currentSpeed > targetSpeed) {
    currentSpeed -= accelStep;
    if (currentSpeed < 0) currentSpeed = 0;  // 速度がマイナスにならないように
  }
}

// 移動用モーターを駆動する関数
void driveMotors() {
  if (targetSpeed == 0 && currentSpeed == 0) {
    stopMotors();
    moveDirection = 0;
    return;
  }

  switch (moveDirection) {
    case 1:  // 前進
      motorL.run(currentSpeed);
      motorR.run(currentSpeed);
      break;
    case -1:  // 後進
      motorL.run(-currentSpeed);
      motorR.run(-currentSpeed);
      break;
    case 2:  // 右その場旋回
      motorL.run(currentSpeed);
      motorR.run(-currentSpeed);
      break;
    case -2:  // 左その場旋回
      motorL.run(-currentSpeed);
      motorR.run(currentSpeed);
      break;
    case 3:  // 右カーブ
      motorL.run(currentSpeed);
      motorR.run((int)(currentSpeed * 0.5));
      break;
    case -3:  // 左カーブ
      motorL.run((int)(currentSpeed * 0.5));
      motorR.run(currentSpeed);
      break;
    case 4:  // 右後進カーブ
      motorL.run(-currentSpeed);
      motorR.run((int)(-currentSpeed * 0.5));
      break;
    case -4:  // 左後進カーブ
      motorL.run((int)(-currentSpeed * 0.5));
      motorR.run(-currentSpeed);
      break;
    default:  // 停止
      stopMotors();
      break;
  }
}

// 移動用モーターを停止する関数
void stopMotors() {
  motorL.stop();
  motorR.stop();
  currentSpeed = 0;
}

// === 補助モーター制御関数 (ポート1を制御) ===
void aux_H_L() {
  digitalWrite(AUX_PIN_1, HIGH);
  digitalWrite(AUX_PIN_2, LOW);
}

void aux_L_H() {
  digitalWrite(AUX_PIN_1, LOW);
  digitalWrite(AUX_PIN_2, HIGH);
}

void stopAuxMotor() {
  digitalWrite(AUX_PIN_1, LOW);
  digitalWrite(AUX_PIN_2, LOW);
}