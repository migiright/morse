#pragma once

#define SPACE ' '
#define DIT '･' //トン
#define DAH '-' //ツー

void morse_update(); //Systick1回ごとに呼ぶ

//トンかツーが確定したときのみDITかDAHが返ってくる
//それ以外はSPACEが返ってくる
char morse_signal();
