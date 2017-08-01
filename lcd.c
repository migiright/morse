#include "lcd.h"
#include <stdio.h>
#include "stm32f30x.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_i2c.h"
#include "utilities.h"

#define LCD_LONG_TIMEOUT 0x10000
//LCDの仕様で決まっている WriteしかできないのでRWフラグも0
#define LCD_I2C_ADDRESS 0xA0
const int lcd_adrbase[2] = { 0x80, 0xC0 };
volatile uint32_t LCD_Timeout;
static void lcd_i2cInit(void);

void lcd_init(void) {
	lcd_i2cInit(); //I2CをLCD用に初期化する
	lcd_reset();
}

//I2C通信をする
//controlMode: コマンドを送るか書き込むデータを送るか
//commandOrData: コマンドまたは書き込むデータ
uint16_t lcd_io(ControlMode controlMode, unsigned char commandOrData) {
	//stm32f3_discovery_lsm303dlhc.cのLSM303DLHC_Writeを参考にした

	//バスが空くのを待つ
	LCD_Timeout = LCD_LONG_TIMEOUT;
	while (I2C_GetFlagStatus(I2C2, I2C_ISR_BUSY) != RESET) {
		if ((LCD_Timeout--) == 0) ASSERT(0 && "LCD Timeout");
	}

	//送る準備をする？スレーブアドレスを送る。
	//3番目の引数で送るバイト数を決める。
	//I2C_AutoEnd_Modeでは決まったバイト数が送られたら自動的にストップシーケンスに移行する
	I2C_TransferHandling(I2C2, LCD_I2C_ADDRESS, 2, I2C_AutoEnd_Mode, I2C_Generate_Start_Write);

	//送信が終わって次のデータ待ちになるのを待つ
	LCD_Timeout = LCD_LONG_TIMEOUT;
	while (I2C_GetFlagStatus(I2C2, I2C_ISR_TXIS) == RESET) {
		if ((LCD_Timeout--) == 0) ASSERT(0 && "LCD Timeout");
	}

	//control byte を送る
	I2C_SendData(I2C2, controlMode == ControlModeWrite ? 0x80 : 0x00);

	//送信が終わって次のデータ待ちになるのを待つ
	LCD_Timeout = LCD_LONG_TIMEOUT;
	while (I2C_GetFlagStatus(I2C2, I2C_ISR_TXIS) == RESET) {
		if ((LCD_Timeout--) == 0) ASSERT(0 && "LCD Timeout");
	}

	I2C_SendData(I2C2, commandOrData); //command or data byte を送る

	//送信が完了するのを待つ
	LCD_Timeout = LCD_LONG_TIMEOUT;
	while (I2C_GetFlagStatus(I2C2, I2C_ISR_STOPF) == RESET) {
		if ((LCD_Timeout--) == 0) ASSERT(0 && "LCD Timeout");
	}

	I2C_ClearFlag(I2C2, I2C_ICR_STOPCF); //STOPFフラグをクリア

	return LCD_OK;
}

//I2C LCD ACM1602N1
void lcd_reset(void) {
	delay(100);
	lcd_io(ControlModeCommand, 0x38); //Function Set: 8bit, 2-line 5x8dots font
	delay(1);
	lcd_io(ControlModeCommand, 0x08); //Display ON/OFF Control: Display:off, Cursor:off, Blnk:off
	delay(1);
	lcd_io(ControlModeCommand, 0x01); //Clear Display
	delay(1);
	lcd_io(ControlModeCommand, 0x06); //Entry Mode Set: cursor右へ, 全体をシフトしない
	delay(1);
	lcd_io(ControlModeCommand, 0x0c); //Display ON/OFF Control: Display:on, Cursor:off, Blnk:off
	delay(1);
}

void lcd_clear(void) {
	lcd_io(ControlModeCommand, 0x01);
	delay(10);
}

void lcd_position(unsigned char xpos, unsigned char ypos) {
	lcd_io(ControlModeCommand, lcd_adrbase[ypos] + xpos);
}

void lcd_str(char* ptr) {
	while (*ptr != 0){
		lcd_io(ControlModeWrite, *ptr++);
	}
}

void lcd_write(unsigned char xpos, unsigned char ypos, char* ptr) {
	lcd_position(xpos, ypos);
	lcd_str(ptr);
}

static void lcd_i2cInit(void) {
	//I2C2用のAPB1クロックを有効にする
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

	//I2C2の入出力に使うGPIOAのクロックを有効にする
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	// +++GPIOAをI2C2用にする設定+++
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_Pin_9がSCL, GPIO_Pin_10がSDA
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_4);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_4);
	// ---GPIOAをI2C2用にする設定---

	// +++I2Cの初期化+++
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
	I2C_InitStructure.I2C_DigitalFilter = 0x00;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	//本来は100kHz I2C_Timing=0x00902025だが使用するLCDでは速すぎるとACKが返ってこないので遅くする
	I2C_InitStructure.I2C_Timing = 0x009FFFFF; //CLK = 62uS(16.8KHz)
	I2C_Init(I2C2, &I2C_InitStructure);
	// ---I2Cの初期化---

	//I2Cを有効にする
	I2C_Cmd(I2C2, ENABLE);
}
