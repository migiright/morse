#include <stdio.h>
#include <string.h>
#include "utilities.h"
#include "lcd.h"

static volatile uint32_t TimingDelay = 0; //Delayで使う

void assertImpl(const char *file, unsigned line, const char *message)
{
	//semihostingにエラー内容を表示
	printf("Error! file: %s, line: %d\nmessage: %s\n", file, line, message);

	// +++lcdにエラー内容を表示+++
	lcd_clear();

	//Error!と行数を1行目に表示
	{
		char buf[17];
		snprintf(buf, 17, "Error! %d", line);
		lcd_write(0, 0, buf);
	}

	//ファイル名を2行目にスクロールして表示
	{
		int p = 0; //今表示している左端の文字の位置
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
	// ---lcdにエラー内容を表示---
}

//Systick_Handlerで呼ばれる
void utilities_systickHandler()
{
	if (TimingDelay != 0x00) TimingDelay--; //Delay用のカウント
}

//nTime回SysTick_Handlerが呼ばれるまで待つ
void delay(uint32_t time)
{
	TimingDelay = time;
	while(TimingDelay != 0);
}
