
#include "stm32f30x.h"

#define LCD_OK ((uint32_t) 0)

typedef enum {
	ControlModeCommand,
	ControlModeWrite,
} ControlMode;

void lcd_init(void);
uint16_t lcd_io(ControlMode controlMode, unsigned char commandOrData);
void lcd_reset(void);
void lcd_clear(void);
void lcd_position(unsigned char xpos, unsigned char ypos);
void lcd_str(char* ptr);
void lcd_write(unsigned char xpos, unsigned char ypos, char* ptr);
void lcd_clear(void);
