//#include "stm32f30x.h"
//#include "stm32f30x_rcc.h"
#include "lcd.h"
#include "button.h"
#include <stdio.h>

//ボタンが押されるか離されるごとにカウントする

int main(void)
{
	//1ミリ秒ごとにSysTick_Hawndlerを呼ぶ
	SysTick_Config(SystemCoreClock/1000);

	lcd_init();
	button_init();

	int p = 0;
	int cnt = 0;
	char buf[17];
	snprintf(buf, 17, "%d", cnt);
	lcd_write(0, 0, buf);

	while(1){
		int cp = button_pushed();
		if(p != cp){
			p = cp;
			++cnt;
			snprintf(buf, 17, "%d", cnt);
			lcd_write(0, 0, buf);
		}
	}
}


