#include <stdio.h>
#include <string.h>
#include "utilities.h"
#include "lcd.h"

static volatile uint32_t TimingDelay = 0; //Delay�Ŏg��

void assertImpl(const char *file, unsigned line, const char *message)
{
	//semihosting�ɃG���[���e��\��
	printf("Error! file: %s, line: %d\nmessage: %s\n", file, line, message);

	// +++lcd�ɃG���[���e��\��+++
	lcd_clear();

	//Error!�ƍs����1�s�ڂɕ\��
	{
		char buf[17];
		snprintf(buf, 17, "Error! %d", line);
		lcd_write(0, 0, buf);
	}

	//�t�@�C������2�s�ڂɃX�N���[�����ĕ\��
	{
		int p = 0; //���\�����Ă��鍶�[�̕����̈ʒu
		char buf[17];
		buf[16] = '\0';
		int l = strlen(file);
		while(1){
			for(int i = 0; i < 16; ++i){
				buf[i] = file[(p + i) % l];
			}
			lcd_write(0, 1, buf);
			++p;
			delay(300);
		}
	}
	// ---lcd�ɃG���[���e��\��---
}

//Systick_Handler�ŌĂ΂��
void utilities_systickHandler()
{
	if (TimingDelay != 0x00) TimingDelay--; //Delay�p�̃J�E���g
}

//nTime��SysTick_Handler���Ă΂��܂ő҂�
void delay(uint32_t time)
{
	TimingDelay = time;
	while(TimingDelay != 0);
}
