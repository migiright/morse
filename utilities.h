#pragma once

//cが0のときエラーになる
#define ASSERT(c) if(!c) assertImpl(__FILE__, __LINE__, #c);

//ASSERTの実装　呼ばなくていい
void assertImpl(const char *file, unsigned line, const char *message);
void utilities_systickHandler();

void delay(uint32_t time); //Systick_Handlerがtime回呼ばれるまで待つ
