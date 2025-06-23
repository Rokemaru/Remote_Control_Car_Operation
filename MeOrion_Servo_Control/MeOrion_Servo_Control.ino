#include "MeOrion.h" // Orionボード専用の設定を読み込む

// 資料のサンプル通り、赤外線受信機はポート4に接続 
MeInfraredReceiver infraredReceiverDecode(PORT_4);

void setup() {
  // PCとのデバッグ用、および受信機Arduinoとの通信を開始
  Serial.begin(9600);
  
  // 赤外線受信機能を開始 
  infraredReceiverDecode.begin();

  Serial.println("赤外線コマンド送信機、起動完了。");
  Serial.println("リモコンのボタンを押してください...");
}

void loop() {
  // 赤外線リモコンから何かのデータを受信したら 
  if (infraredReceiverDecode.available()) {
    // 受信したボタンのコードを読み取る 
    uint8_t ReceiverCode = infraredReceiverDecode.read();

    // どのボタンが押されたかによって処理を分ける 
    switch (ReceiverCode) {
      case IR_BUTTON_A:
        Serial.println("リモコン[A]を検出 -> コマンド 'A' を送信");
        Serial.write('A'); // 受信機に向けてコマンド 'A' を送信
        break;
        
      case IR_BUTTON_B:
        Serial.println("リモコン[B]を検出 -> コマンド 'B' を送信");
        Serial.write('B'); // 受信機に向けてコマンド 'B' を送信
        break;
        
      // 他のボタンは無視する
      default:
        break;
    }
  }
}