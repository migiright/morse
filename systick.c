#include "systick.h"
#include "stm32f30x.h"
#include "utilities.h"

//ˆê’èŠÔ‚²‚Æ‚ÉŒÄ‚Î‚ê‚é ŠÔ‚ÍSysTick_Config‚Åİ’è‚·‚é
void SysTick_Handler()
{
	utilities_systickHandler();
}

