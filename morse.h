#pragma once

#define SPACE ' '
#define DIT '�' //�g��
#define DAH '-' //�c�[

void morse_update(); //Systick1�񂲂ƂɌĂ�

//�g�����c�[���m�肵���Ƃ��̂�DIT��DAH���Ԃ��Ă���
//����ȊO��SPACE���Ԃ��Ă���
char morse_signal();

//�����ɑΉ����郂�[���X�M����Ԃ�
const char *morseFromChar(char c);

//���[���X�M���ɕϊ�����
//len: dst�ɉ������������ނ�('\0'�܂�)
void convertStrToMorse(const char *str, char *dst, int len);
