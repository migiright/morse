#include "systick.h"
#include "stm32f30x.h"
#include "utilities.h"

//一定時間ごとに呼ばれる 時間はSysTick_Configで設定する
void SysTick_Handler()
{
	utilities_systickHandler();
}

