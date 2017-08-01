#include "button.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_gpio.h"

void button_init()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitTypeDef gpioInitStrcture;
	GPIO_StructInit(&gpioInitStrcture);
	gpioInitStrcture.GPIO_Pin = GPIO_Pin_0;
	gpioInitStrcture.GPIO_Mode = GPIO_Mode_IN;
	gpioInitStrcture.GPIO_Speed = GPIO_Speed_50MHz;
	gpioInitStrcture.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &gpioInitStrcture);
}

int button_pushed()
{
	return button_pushedLaw();
}

int button_pushedLaw()
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1;
}
