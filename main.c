//#include "stm32f30x.h"
//#include "stm32f30x_rcc.h"
#include "lcd.h"

int main(void)
{
	//1ƒ~ƒŠ•b‚²‚Æ‚ÉSysTick_Hawndler‚ðŒÄ‚Ô
	SysTick_Config(SystemCoreClock/1000);

	lcd_init();

	while(1){
	}
}


