#include "morse.h"
#include "button.h"

#define DAH_TIME 200

typedef enum {
	StatePulled, //ボタンが離されている
	StatePushed, //ボタンが押されてトンかツーか見極めている
	StateDah, //ボタンが十分長い時間押されてツーと確定している
} State;

State state = StatePulled;
int count = 0; //ツーと判定されるまでの残り時間
char signal = SPACE; //morse_signalで返す今判定された信号

void morse_update() {
	signal = SPACE;
	switch (state) {
	case StatePulled:
		if (button_pushed()) {
			state = StatePushed;
			count = DAH_TIME; //カウントダウン開始
		}
		break;
	case StatePushed:
		if (!button_pushed()) { //規定時間前に離されたらトンと判定
			signal = DIT;
			state = StatePulled;
		} else {
			--count; //カウントダウン
			if (count < 0) { //規定時間たったらツーと判定
				signal = DAH;
				state = StateDah;
			}
		}
		break;
	case StateDah:
		if (!button_pushed()) {
			state = StatePulled;
		}
		break;
	}
}

//トンかツーが確定したときのみDITかDAHが返ってくる
//それ以外はSPACEが返ってくる
char morse_signal(){
	return signal;
}
