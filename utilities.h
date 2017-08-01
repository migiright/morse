#pragma once

//c‚ª0‚Ì‚Æ‚«ƒGƒ‰[‚É‚È‚é
#define ASSERT(c) if(!c) assertImpl(__FILE__, __LINE__, #c);

//ASSERT‚ÌÀ‘•@ŒÄ‚Î‚È‚­‚Ä‚¢‚¢
void assertImpl(const char *file, unsigned line, const char *message);
void utilities_systickHandler();

void delay(uint32_t time); //Systick_Handler‚ªtime‰ñŒÄ‚Î‚ê‚é‚Ü‚Å‘Ò‚Â
