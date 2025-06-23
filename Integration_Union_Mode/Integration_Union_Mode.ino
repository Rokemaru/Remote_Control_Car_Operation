#include <MeOrion.h>
#include <SoftwareSerial.h>

// =================================================================
// 統合プログラム
// =================================================================
// 機能：
// 1. M1/M2による加減速・旋回機能付きの移動
// 2. ポート1に接続した9V補助モーターを一定時間駆動
// 3. 別のArduinoへシリアルコマンドを送信
// =================================================================


// --- モーター定義（移動用）---
MeDCMotor motorR(M1);
MeDCMotor motorL(M2);

// --- IRリモコン ---
MeInfraredReceiver irReceiver(PORT_4);

// --- 補助モーター（9Vモーターをポート1に接続）---
#define AUX_PIN_1 10 // ポート1のデジタルピン
#define AUX_PIN_2 11 // ポート1のデジタルピン

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
const unsigned long auxMotorDuration = 100; // 補助モーターのON時間(ミリ秒)
bool auxMotorActive = false;

void setup() {
  // 補助モーターのピンを出力に設定
  pinMode(AUX_PIN_1, OUTPUT);
  pinMode(AUX_PIN_2, OUTPUT);

  // IRリモコンとシリアル通信を開始
  irReceiver.begin();
  Serial.begin(9600);
  Serial.println("Start Full Control System");

  // 全てのモーターを停止状態で開始
  stopMotors();
  stopAuxMotor();
}

void loop() {
  // リモコンのボタンが押された、または押され続けている場合
  if (irReceiver.available() || irReceiver.buttonState()) {
    uint8_t cmd = irReceiver.read();

    // =========== ここから各種リモコン操作を割り当て ============
    
    // --- 補助モーター制御 (ボタン1, 2) ---
    if (cmd == IR_BUTTON_1) {
      Serial.println("Aux Motor: ON (L_H)");
      aux_L_H();
      auxMotorActive = true;
      auxMotorStartTime = millis();
    } else if (cmd == IR_BUTTON_2) {
      Serial.println("Aux Motor: ON (H_L)");
      aux_H_L();
      auxMotorActive = true;
      auxMotorStartTime = millis();
    }
    
    // --- シリアル通信 (ボタンA, B) ---
    else if (cmd == IR_BUTTON_A) {
      Serial.println("Serial Command 'A' Sent");
      Serial.write('A');
    } else if (cmd == IR_BUTTON_B) {
      Serial.println("Serial Command 'B' Sent");
      Serial.write('B');
    }

    // --- 移動用モーター制御 (方向キーなど) ---
    switch (cmd) {
      case IR_BUTTON_UP:    targetSpeed = maxSpeed; moveDirection = 1;  break;
      case IR_BUTTON_DOWN:  targetSpeed = maxSpeed; moveDirection = -1; break;
      case IR_BUTTON_RIGHT: targetSpeed = maxSpeed; moveDirection = 2;  break;
      case IR_BUTTON_LEFT:  targetSpeed = maxSpeed; moveDirection = -2; break;
      case IR_BUTTON_E:     targetSpeed = maxSpeed; moveDirection = 3;  break;
      case IR_BUTTON_D:     targetSpeed = maxSpeed; moveDirection = -3; break;
      case IR_BUTTON_F:     targetSpeed = maxSpeed; moveDirection = 4;  break;
      case IR_BUTTON_0:     targetSpeed = maxSpeed; moveDirection = -4; break;
    }
  } else {
    // ボタンが何も押されていない場合は、移動用モーターの目標速度を0にする
    targetSpeed = 0;
  }

  // --- 補助モーターを一定時間後に自動停止 ---
  if (auxMotorActive && (millis() - auxMotorStartTime > auxMotorDuration)) {
    stopAuxMotor();
    auxMotorActive = false;
    Serial.println("Aux Motor: Auto OFF");
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
    if (currentSpeed < targetSpeed) currentSpeed = targetSpeed;
  }
}

// 移動用モーターを駆動する関数
void driveMotors() {
  switch (moveDirection) {
    case 1:  motorL.run(currentSpeed); motorR.run(currentSpeed); break;  // 前進
    case -1: motorL.run(-currentSpeed); motorR.run(-currentSpeed); break; // 後進
    case 2:  motorL.run(currentSpeed); motorR.run(-currentSpeed); break; // 右その場旋回
    case -2: motorL.run(-currentSpeed); motorR.run(currentSpeed); break; // 左その場旋回
    case 3:  motorL.run(currentSpeed); motorR.run((int)(currentSpeed * 0.5)); break; // 右カーブ
    case -3: motorL.run((int)(currentSpeed * 0.5)); motorR.run(currentSpeed); break; // 左カーブ
    case -4: motorL.run((int)(-currentSpeed * 0.5)); motorR.run(-currentSpeed); break; // 左後進カーブ
    case 4:  motorL.run(-currentSpeed); motorR.run((int)(-currentSpeed * 0.5)); break; // 右後進カーブ
    default: motorL.stop(); motorR.stop(); break; // その他
  }
}

// 移動用モーターを停止する関数
void stopMotors() {
  motorL.stop();
  motorR.stop();
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
  // ストップ (LOW, LOW) または ブレーキ (HIGH, HIGH) を選択
  digitalWrite(AUX_PIN_1, LOW);
  digitalWrite(AUX_PIN_2, LOW);
}