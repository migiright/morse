#pragma once

#define SPACE ' '
#define DIT '�' //�g��
#define DAH '-' //�c�[

void morse_update(); //Systick1�񂲂ƂɌĂ�

//�g�����c�[���m�肵���Ƃ��̂�DIT��DAH���Ԃ��Ă���
//����ȊO��SPACE���Ԃ��Ă���
char morse_signal();
