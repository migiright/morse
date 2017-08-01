//#include "stm32f30x.h"
//#include "stm32f30x_rcc.h"
#include "lcd.h"
#include "button.h"
#include "morse.h"

volatile int mainEnabled = 0; //これが1になるとmainのループで1回処理される
//mainでの処理が開始される前にmorse_updateが呼ばれ
//morse_signalの値が変わる可能性があるので退避しておく
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
	//1ミリ秒ごとにSysTick_Hawndlerを呼ぶ
	SysTick_Config(SystemCoreClock/1000);

	lcd_init();
	button_init();

	lcd_position(0, 0);

	while(1){
		while(!mainEnabled){} //main_systickHandlerでmainEnabledが1になるのを待つ
		mainEnabled = 0;
	}
}


