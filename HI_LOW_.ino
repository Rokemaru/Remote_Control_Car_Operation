#include <MeOrion.h>

#define PIN_1 12  // PORT_3のピン12
#define PIN_2 13  // PORT_3のピン13

void setup() {
  pinMode(PIN_1, OUTPUT);  // ピン12を出力モードに設定
  pinMode(PIN_2, OUTPUT);  // ピン13を出力モードに設定
  Serial.begin(9600);      // シリアル通信を開始
}

void loop() {
  digitalWrite(PIN_1, HIGH);  // ピン12をHIGH（5V）
  digitalWrite(PIN_2, LOW);   // ピン13をLOW（0V）
  Serial.println("H_L");
  delay(3000);

  digitalWrite(PIN_1, HIGH);  // ピン12をHIGH（5V）
  digitalWrite(PIN_2, HIGH);  // ピン13をHIGH（5V）
  Serial.println("Stop");      // "Stop"をシリアルモニタに表示
  delay(3000);

  digitalWrite(PIN_1, LOW);   // ピン12をLOW（0V）
  digitalWrite(PIN_2, HIGH);  // ピン13をHIGH（5V）
  Serial.println("L_H");
  delay(3000);

  digitalWrite(PIN_1, LOW);   // ピン12をLOW（0V）
  digitalWrite(PIN_2, LOW);   // ピン13をLOW（0V）
  Serial.println("Break");     // "Break"をシリアルモニタに表示
  delay(3000);
}