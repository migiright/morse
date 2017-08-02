#include "morse.h"
#include "button.h"
#include <ctype.h>

#define DAH_TIME 200

typedef enum {
	StatePulled, //�{�^����������Ă���
	StatePushed, //�{�^����������ăg�����c�[�����ɂ߂Ă���
	StateDah, //�{�^�����\���������ԉ�����ăc�[�Ɗm�肵�Ă���
} State;

State state = StatePulled;
int count = 0; //�c�[�Ɣ��肳���܂ł̎c�莞��
char signal = SPACE; //morse_signal�ŕԂ������肳�ꂽ�M��

void morse_update() {
	signal = SPACE;
	switch (state) {
	case StatePulled:
		if (button_pushed()) {
			state = StatePushed;
			count = DAH_TIME; //�J�E���g�_�E���J�n
		}
		break;
	case StatePushed:
		if (!button_pushed()) { //�K�莞�ԑO�ɗ����ꂽ��g���Ɣ���
			signal = DIT;
			state = StatePulled;
		} else {
			--count; //�J�E���g�_�E��
			if (count < 0) { //�K�莞�Ԃ�������c�[�Ɣ���
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

//�g�����c�[���m�肵���Ƃ��̂�DIT��DAH���Ԃ��Ă���
//����ȊO��SPACE���Ԃ��Ă���
char morse_signal(){
	return signal;
}

const char * const MorseAlphabets[] = {
	"�-", "-���", "-�-�", "-��", "�", "��-�", "--�", //ABCDEFG
	"����", "��", "�---", "-�-", "�-��", "--", "-�", //HIJKLMN
	"---", "�--�", "--�-", "�-�", "���", "-", "��-", //OPQRSTU
	"���-", "�--", "-��-", "-�--", "--��", //VWXYZ
};

const char * const MorseNumerals[] = {
	"-----", "�----", "��---", "���--", "����-", //01234
	"�����", "-����", "--���", "---��", "----�", //56789
};

const char * const MorsePeriod = "�-�-�-";
const char * const MorseComma = "--��--";
const char * const MorseQuestion = "��--��";
const char * const MorseExclamation = "-�-�--";
const char * const MorseColon = "---���";
const char * const MorseSemicolon = "-�-�-�";
const char * const MorseHyphen = "-���-";
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
	const char * const end = dst + len - 1; //'\0'���������ޏꏊ
	for(; *str != '\0'; ++str){
		if(*str == ' '){ //�X�y�[�X�̓X�y�[�X1������
			if(dst == end) goto full;
			*dst++ = ' ';
		} else if(morseFromChar(*str) == 0){ //�Ή����Ă��Ȃ������͔�΂�
			continue;
		} else {
			//*str�̕����ɑΉ����郂�[���X�M������������
			for(const char *pChar = morseFromChar(*str); *pChar != '\0'; ++pChar){
				if(dst == end) goto full;
				*dst++ = *pChar;
			}
			//�X�y�[�X��J����
			if(dst == end) goto full;
			*dst++ = ' ';
		}
	}
full:
	*dst = '\0';
}

