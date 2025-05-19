#include "MeOrion.h"
#include <SoftwareSerial.h>

MeDCMotor motorR(M1);
MeDCMotor motorL(M2);
MeInfraredReceiver irReceiver(PORT_4);

const uint8_t minSpeed = 0;
const uint8_t maxSpeed = 255;
const uint8_t accelStep = 5;
const uint16_t accelDelay = 20;

int targetSpeed = 0;
int currentSpeed = 0;
int moveDirection = 0;  // 進行方向

void setup() {
  irReceiver.begin();
  Serial.begin(9600);
  Serial.println("Start Full Direction Control");
  stopMotors();
}

void loop() {
  if (irReceiver.available() || irReceiver.buttonState()) {
    uint8_t cmd = irReceiver.read();

    switch (cmd) {
      case IR_BUTTON_UP:
        targetSpeed = maxSpeed;
        moveDirection = 1;  // 前進
        break;
      case IR_BUTTON_DOWN:
        targetSpeed = maxSpeed;
        moveDirection = -1;  // 後退
        break;
      case IR_BUTTON_RIGHT:
        targetSpeed = maxSpeed;
        moveDirection = 2;  // 右折
        break;
      case IR_BUTTON_LEFT:
        targetSpeed = maxSpeed;
        moveDirection = -2;  // 左折
        break;

      case IR_BUTTON_E:  // 右前
        targetSpeed = maxSpeed;
        moveDirection = 3;
        break;
      case IR_BUTTON_D:  // 左前
        targetSpeed = maxSpeed;
        moveDirection = -3;
        break;
      case IR_BUTTON_F:  // 右後ろ
        targetSpeed = maxSpeed;
        moveDirection = 4;
        break;
      case IR_BUTTON_0:  // 左後ろ
        targetSpeed = maxSpeed;
        moveDirection = -4;
        break;

      default:
        break;
    }
  } else {
    targetSpeed = 0;  // ボタンを離したら停止
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
    case 1:  // 前進
      motorL.run(currentSpeed);
      motorR.run(currentSpeed);
      break;
    case -1:  // 後退
      motorL.run(-currentSpeed);
      motorR.run(-currentSpeed);
      break;
    case 2:  // 右折
      motorL.run(currentSpeed);
      motorR.run(-currentSpeed);
      break;
    case -2:  // 左折
      motorL.run(-currentSpeed);
      motorR.run(currentSpeed);
      break;
    case 3:  // 右前
      motorL.run(currentSpeed);
      motorR.run((int)(currentSpeed * 0.5));
      break;
    case -3:  // 左前
      motorL.run((int)(currentSpeed * 0.5));
      motorR.run(currentSpeed);
      break;
    case 4:  // 右後ろ
      motorL.run(-currentSpeed);
      motorR.run((int)(-currentSpeed * 0.5));
      break;
    case -4:  // 左後ろ
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
