#include "morse.h"
#include "button.h"
#include <ctype.h>

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

const char * const MorseAlphabets[] = {
	"･-", "-･･･", "-･-･", "-･･", "･", "･･-･", "--･", //ABCDEFG
	"････", "･･", "･---", "-･-", "･-･･", "--", "-･", //HIJKLMN
	"---", "･--･", "--･-", "･-･", "･･･", "-", "･･-", //OPQRSTU
	"･･･-", "･--", "-･･-", "-･--", "--･･", //VWXYZ
};

const char * const MorseNumerals[] = {
	"-----", "･----", "･･---", "･･･--", "････-", //01234
	"･････", "-････", "--･･･", "---･･", "----･", //56789
};

const char * const MorsePeriod = "･-･-･-";
const char * const MorseComma = "--･･--";
const char * const MorseQuestion = "･･--･･";
const char * const MorseExclamation = "-･-･--";
const char * const MorseColon = "---･･･";
const char * const MorseSemicolon = "-･-･-･";
const char * const MorseHyphen = "-･･･-";
const char * const MorseSpace = "";
const char * const MorseNone = 0;

const char *morseFromChar(char c){
	if(isalpha(c)){
		return MorseAlphabets[toupper(c) - 'A'];
	} else if(isdigit(c)){
		return MorseNumerals[c - '0'];
	} else {
		switch(c){
		case '.': return MorsePeriod;
		case ',': return MorseComma;
		case '?': return MorseQuestion;
		case '!': return MorseExclamation;
		case ':': return MorseColon;
		case ';': return MorseSemicolon;
		case '-': return MorseHyphen;
		case ' ': return MorseSpace;
		default: return MorseNone;
		}
	}
}

void convertStrToMorse(const char *str, char *dst, int len) {
	const char * const end = dst + len - 1; //'\0'を書き込む場所
	for(; *str != '\0'; ++str){
		if(*str == ' '){ //スペースはスペース1個あける
			if(dst == end) goto full;
			*dst++ = ' ';
		} else if(morseFromChar(*str) == 0){ //対応していない文字は飛ばす
			continue;
		} else {
			//*strの文字に対応するモールス信号を書き込む
			for(const char *pChar = morseFromChar(*str); *pChar != '\0'; ++pChar){
				if(dst == end) goto full;
				*dst++ = *pChar;
			}
			//スペース一個開ける
			if(dst == end) goto full;
			*dst++ = ' ';
		}
	}
full:
	*dst = '\0';
}

