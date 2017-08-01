#pragma once

#define SPACE ' '
#define DIT '･' //トン
#define DAH '-' //ツー

void morse_update(); //Systick1回ごとに呼ぶ

//トンかツーが確定したときのみDITかDAHが返ってくる
//それ以外はSPACEが返ってくる
char morse_signal();

//文字に対応するモールス信号を返す
const char *morseFromChar(char c);

//モールス信号に変換する
//len: dstに何文字書き込むか('\0'含む)
void convertStrToMorse(const char *str, char *dst, int len);
