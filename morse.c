#include "morse.h"
#include "button.h"

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
