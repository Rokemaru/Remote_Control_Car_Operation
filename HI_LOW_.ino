#include <MeOrion.h>
#include <SoftwareSerial.h>

#define PIN_1 12  // PORT_3のピン12
#define PIN_2 13  // PORT_3のピン13

MeInfraredReceiver irReceiver(PORT_4);  // IR受信機をPORT_6に接続（環境に応じて変更）

void setup() {
  pinMode(PIN_1, OUTPUT);
  pinMode(PIN_2, OUTPUT);
  Serial.begin(9600);
  irReceiver.begin();
  Serial.println("Start Full Direction Control");
  stopMotors();
}

void loop() {
  if (irReceiver.available()) {
    int irCode = irReceiver.read();
    Serial.print("IR Code: ");
    Serial.println(irCode);

    // リモコンのボタンに応じた動作を設定
    switch (irCode) {
      case IR_BUTTON_0:
        H_L();
        break;

      case IR_BUTTON_1:
        L_H();
        break;

      case IR_BUTTON_2:
        stopMotors();
        break;

      case IR_BUTTON_3:
        Break();
        break;

      default:
        Serial.println("Unknown Command");
        break;
    }
  }
}

// 各動作関数
void H_L() {
  digitalWrite(PIN_1, HIGH);
  digitalWrite(PIN_2, LOW);
  Serial.println("H_L");
}

void L_H() {
  digitalWrite(PIN_1, LOW);
  digitalWrite(PIN_2, HIGH);
  Serial.println("L_H");
}

void stopMotors() {
  digitalWrite(PIN_1, HIGH);
  digitalWrite(PIN_2, HIGH);
  Serial.println("Stop");
}

void Break() {
  digitalWrite(PIN_1, LOW);
  digitalWrite(PIN_2, LOW);
  Serial.println("Break");
}
