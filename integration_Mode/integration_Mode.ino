#include <MeOrion.h>
#include <SoftwareSerial.h>

// モーター定義（移動用）
MeDCMotor motorR(M1);
MeDCMotor motorL(M2);

// IRリモコン
MeInfraredReceiver irReceiver(PORT_4);

// 別のモーター用ピン。ポート3に指す。
#define PIN_1 12
#define PIN_2 13

// モーター速度制御用
const uint8_t minSpeed = 0;
const uint8_t maxSpeed = 250;
const uint8_t accelStep = 5;
const uint16_t accelDelay = 20;

int targetSpeed = 0;
int currentSpeed = 0;
int moveDirection = 0;

void setup() {
  pinMode(PIN_1, OUTPUT);
  pinMode(PIN_2, OUTPUT);

  irReceiver.begin();
  // シリアルポートがエラーを起こす原因の可能性がある？
  Serial.begin(9600);
  Serial.println("Start Full Direction Control");

  stopMotors();
  stopAuxMotor();
}

// ここで受け取ったボタンに対してのスイッチ文で分岐させて、実行している。
void loop() {
  if (irReceiver.available() || irReceiver.buttonState()) {
    uint8_t cmd = irReceiver.read();
    Serial.print("IR Code: ");
    Serial.println(cmd);


    // ========== モータードライバー制御 ==========
    if (cmd == IR_BUTTON_1) {
      aux_L_H();
    } else if (cmd == IR_BUTTON_2) {
      aux_H_L();
    }


    switch (cmd) {
      // ========== 移動用 ==========
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

      // この箇所で、何もしてない時の動作を書き込む。
      default:
        // stopAuxMotor();
        stopMotors();
        break;
    }
    // --- 補助モーターを止める条件：1と2以外のとき ---
    if (cmd != IR_BUTTON_1 && cmd != IR_BUTTON_2) {
      stopAuxMotor();
    }

  } else {
    targetSpeed = 0;
  }

  updateSpeed();
  driveMotors();
  delay(accelDelay);
}

// 加速、減速はこの箇所で処理。モータの回す値を決めた値、決めた時間で増加させたり減らしてる。
void updateSpeed() {
  if (currentSpeed < targetSpeed) {
    currentSpeed += accelStep;
    if (currentSpeed > targetSpeed) currentSpeed = targetSpeed;
  } else if (currentSpeed > targetSpeed) {
    currentSpeed -= accelStep;
    if (currentSpeed < targetSpeed) currentSpeed = targetSpeed;
  }
}

// ラジコンの動きのモータの挙動をスイッチ文で出てきた値をもとに実行している。
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

// 個々から下はモータドライバー制御の関数をここで定義。
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

void brakeAuxMotor() {
  digitalWrite(PIN_1, LOW);
  digitalWrite(PIN_2, LOW);
}
