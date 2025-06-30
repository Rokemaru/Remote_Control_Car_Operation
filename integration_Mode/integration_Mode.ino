#include <MeOrion.h>
#include <SoftwareSerial.h>

// モーター定義（移動用）
MeDCMotor motorR(M1);
MeDCMotor motorL(M2);

// IRリモコン
MeInfraredReceiver irReceiver(PORT_4);

// 補助モーター（ポート3に接続）
#define PIN_1 12
#define PIN_2 13

const uint8_t minSpeed = 0;
const uint8_t maxSpeed = 255;
const uint8_t accelStep = 5;
const uint16_t accelDelay = 1;

int targetSpeed = 0;
int currentSpeed = 0;
int moveDirection = 0;

// --- 補助モーター動作時間制御用 ---
unsigned long auxMotorStartTime = 0;
const unsigned long auxMotorDuration = 50;  // 500msだけONにする
bool auxMotorActive = false;

void setup() {
  pinMode(PIN_1, OUTPUT);
  pinMode(PIN_2, OUTPUT);

  irReceiver.begin();
  Serial.begin(9600);
  Serial.println("Start Full Direction Control");

  stopMotors();
  stopAuxMotor();
}

void loop() {
  if (irReceiver.available() || irReceiver.buttonState()) {
    uint8_t cmd = irReceiver.read();

    Serial.print("IR Code Received: ");
    Serial.println(cmd);  // IRコードの数値を確認できるようにする

    // ========== 補助モーター制御 ==========
    if (cmd == IR_BUTTON_1) {
      aux_L_H();
      auxMotorActive = true;
      auxMotorStartTime = millis();
    } else if (cmd == IR_BUTTON_2) {
      aux_H_L();
      auxMotorActive = true;
      auxMotorStartTime = millis();
    }

    // ========== 移動用モーター制御 ==========
    switch (cmd) {
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
      case IR_BUTTON_E:
        targetSpeed = maxSpeed;
        moveDirection = 3;
        break;
      case IR_BUTTON_D:
        targetSpeed = maxSpeed;
        moveDirection = -3;
        break;
      case IR_BUTTON_F:
        targetSpeed = maxSpeed;
        moveDirection = 4;
        break;
      case IR_BUTTON_0:
        targetSpeed = maxSpeed;
        moveDirection = -4;
        break;
      default:
        stopMotors();
        break;
    }
  } else {
    targetSpeed = 0;
  }

  // 補助モーターを一定時間後に自動停止
  if (auxMotorActive && (millis() - auxMotorStartTime > auxMotorDuration)) {
    stopAuxMotor();
    auxMotorActive = false;
  }

  updateSpeed();
  driveMotors();
  delay(accelDelay);
}

void updateSpeed() {
  if (currentSpeed < targetSpeed) {
    currentSpeed += accelStep;
    if (currentSpeed > targetSpeed) currentSpeed = targetSpeed;
  } else if (currentSpeed > targetSpeed) {
    currentSpeed -= accelStep;
    if (currentSpeed < targetSpeed) currentSpeed = targetSpeed;
  }
}

void driveMotors() {
  switch (moveDirection) {
    case 1:
      motorL.run(currentSpeed);
      motorR.run(currentSpeed);
      break;
    case -1:
      motorL.run(-currentSpeed);
      motorR.run(-currentSpeed);
      break;
    case 2:
      motorL.run(currentSpeed);
      motorR.run(-currentSpeed);
      break;
    case -2:
      motorL.run(-currentSpeed);
      motorR.run(currentSpeed);
      break;
    case 3:
      motorL.run(currentSpeed);
      motorR.run((int)(currentSpeed * 0.5));
      break;
    case -3:
      motorL.run((int)(currentSpeed * 0.5));
      motorR.run(currentSpeed);
      break;
    case 4:
      motorL.run(-currentSpeed);
      motorR.run((int)(-currentSpeed * 0.5));
      break;
    case -4:
      motorL.run((int)(-currentSpeed * 0.5));
      motorR.run(-currentSpeed);
      break;
    default:
      motorL.stop();
      motorR.stop();
      break;
  }
}

void stopMotors() {
  motorL.stop();
  motorR.stop();
}

// === 補助モーター制御関数 ===
void aux_H_L() {
  digitalWrite(PIN_1, HIGH);
  digitalWrite(PIN_2, LOW);
}

void aux_L_H() {
  digitalWrite(PIN_1, LOW);
  digitalWrite(PIN_2, HIGH);
}

void stopAuxMotor() {
  digitalWrite(PIN_1, HIGH);
  digitalWrite(PIN_2, HIGH);
}
