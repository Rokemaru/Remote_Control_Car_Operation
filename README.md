# このレポジトリの説明

- ### 使用環境
	- Arduino_IDE
	- 使用マイコン
		- MakeBlockのローバーのマイコンボード(Orion)
	- 赤外線コントローラー
	- 

---

# コードの解説
### - Radio-controlled square movement  
　- このコードは車が、スタート地点を基準に四角形の起動で移動するコード<br>
　- 書き込みが完了したら指定した分だけ移動して停止して、また動きます<br>
　- 机から落っこちないように気をつけてください<br>
### - Radio control_Remote control operation 
　 - このコードはリモコンで操作して四方八方に移動できるようになっている<br>
	　- 前・後・右・左・右斜め・左斜め・右後ろ・左後ろを十字ボタンを基準に移動する<br>
　- 加速と減速が段階的に行われるようになっている<br>
### - Motor_driver_control  
- モータードライバーを使用したモーター制御のコードです<br>
	- 0ボタン＝動く
	- 1ボタン＝動く
	- 2ボタン＝止まる
	- 3ボタン＝止まる
### - integration_Mode 
　- モータードライバー制御と、ラジコンカーの制御を行えるコード<br>
　- Radio control_Remote control operationとMotor_driver_controlが合わさったコードだと思ってOK<br>
　- 現在開発中なのでバグがありましたら申し訳ございません
