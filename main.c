//#include "stm32f30x.h"
//#include "stm32f30x_rcc.h"
#include "lcd.h"
#include "button.h"
#include "morse.h"
#include "ringBuffer.h"

typedef enum {
	ResultOk,
	ResultTextEnd,
} Result;

volatile int mainEnabled = 0; //���ꂪ1�ɂȂ��main�̃��[�v��1�񏈗������

//main�ł̏������J�n�����O��morse_update���Ă΂�
//morse_signal�̒l���ς��\��������̂őޔ����Ă���
volatile char signal;

volatile int time; //�Q�[���J�n����̎���

const char * const Text = "We must respect the will of the individual.";

const char *currentTextPosition;

void draw() {
	//�e�L�X�g��1�s�ڂɕ\��
	{
		char buf[17];
		int i = 0;
		while (i < 16 && currentTextPosition[i] != '\0') {
			buf[i] = currentTextPosition[i];
			++i;
		}
		while (i < 16) {
			buf[i++] = ' '; //�󔒂Ŗ��߂�
		}
		buf[i] = '\0';
		lcd_write(0, 0, buf);
	}

	//���[���X�M����2�s�ڂɕ\��
	{
		char buf[17];
		int i = 0;
		while (i < 16 && i < ringBuffer_size()) {
			buf[i] = ringBuffer_read(i)->morse;
			++i;
		}
		while (i < 16) {
			buf[i++] = ' '; //�󔒂Ŗ��߂�
		}
		buf[i] = '\0';
		lcd_write(0, 1, buf);
	}
}

int writeMorse(const char *p) {
	const char *m;
	while (1) {
		if (*p == '\0') return ResultTextEnd;
		m = morseFromChar(*p);
		if (m != 0) break;
		++p;
	}
	while (*m != '\0') {
		MorseTuple mt = { *m, p };
		ringBuffer_add(mt);
		++m;
	}
	MorseTuple mt = { ' ', p };
	ringBuffer_add(mt);
	return ResultOk;
}

Result fillWithMorse() {
	while (ringBuffer_size() < 16) {
		Result r
			= writeMorse(ringBuffer_read(ringBuffer_size() - 1)->textPos + 1);
		if (r != ResultOk) return r;
	}
	return ResultOk;
}

void moveNext() {
	do {
		ringBuffer_removeFirst();
		fillWithMorse();
	} while (ringBuffer_read(0)->morse == SPACE);
	currentTextPosition = ringBuffer_read(0)->textPos;
}

void init(void) {
	currentTextPosition = Text;
	writeMorse(currentTextPosition);
	fillWithMorse();
	time = 0;
}

void main_systickHandler() {
	morse_update();
	signal = morse_signal();
	if (signal != SPACE) {
		mainEnabled = 1;
	}
	++time;
}

void clear(){
	lcd_write(0, 0, "Clear!          ");
	char buf[17];
	snprintf(buf, 17, "Time: %02d:%02d:%02d  ",
		time / 3600000, time / 60000 % 60, time / 1000 % 60);
	lcd_write(0, 1, buf);
	while(1){}
}

int main(void) {
	//1�~���b���Ƃ�SysTick_Hawndler���Ă�
	SysTick_Config(SystemCoreClock / 1000);

	lcd_init();
	button_init();

	init();
	draw();

	while (1) {
		while (!mainEnabled); //main_systickHandler��mainEnabled��1�ɂȂ�̂�҂�
		mainEnabled = 0;

		if (signal == ringBuffer_read(0)->morse) {
			moveNext();
			if(ringBuffer_size() == 0){
				clear();
			}
		}
		draw();
	}
}
