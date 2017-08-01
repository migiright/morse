//#include "stm32f30x.h"
//#include "stm32f30x_rcc.h"
#include "lcd.h"
#include "button.h"
#include "morse.h"

volatile int mainEnabled = 0; //���ꂪ1�ɂȂ��main�̃��[�v��1�񏈗������
//main�ł̏������J�n�����O��morse_update���Ă΂�
//morse_signal�̒l���ς��\��������̂őޔ����Ă���
volatile char signal;

void main_systickHandler()
{
	morse_update();
	signal = morse_signal();
	if(signal != SPACE){
		mainEnabled = 1;
	}
}

int main(void)
{
	//1�~���b���Ƃ�SysTick_Hawndler���Ă�
	SysTick_Config(SystemCoreClock/1000);

	lcd_init();
	button_init();

	lcd_position(0, 0);

	while(1){
		while(!mainEnabled){} //main_systickHandler��mainEnabled��1�ɂȂ�̂�҂�
		mainEnabled = 0;
	}
}


