#pragma once

//c��0�̂Ƃ��G���[�ɂȂ�
#define ASSERT(c) if(!c) assertImpl(__FILE__, __LINE__, #c);

//ASSERT�̎����@�Ă΂Ȃ��Ă���
void assertImpl(const char *file, unsigned line, const char *message);
void utilities_systickHandler();

void delay(uint32_t time); //Systick_Handler��time��Ă΂��܂ő҂�
