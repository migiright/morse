//#include "stm32f30x.h"
//#include "stm32f30x_rcc.h"
#include "lcd.h"
#include "button.h"
#include "morse.h"

void main_systickHandler()
{
	morse_update();
	char buf[] = " ";
	buf[0] = morse_signal();
	if(buf[0] == DAH || buf[0] == DIT){
		lcd_str(buf);
	}
}

int main(void)
{
	//1ƒ~ƒŠ•b‚²‚Æ‚ÉSysTick_Hawndler‚ðŒÄ‚Ô
	SysTick_Config(SystemCoreClock/1000);

	lcd_init();
	button_init();

	lcd_position(0, 0);

	while(1){
	}
}


