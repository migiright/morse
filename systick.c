#include "systick.h"
#include "stm32f30x.h"
#include "utilities.h"
#include "main.h"

//��莞�Ԃ��ƂɌĂ΂�� ���Ԃ�SysTick_Config�Őݒ肷��
void SysTick_Handler()
{
	utilities_systickHandler();
	main_systickHandler();
}

