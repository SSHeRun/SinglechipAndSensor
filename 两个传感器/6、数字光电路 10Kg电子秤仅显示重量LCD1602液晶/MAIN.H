#ifndef __MAIN_H__
#define __MAIN_H__


#include <reg52.h>

//IO����
sbit Buzzer = P1^0;


//�������߱�������
extern void Delay_ms(unsigned int n);
extern void Get_Maopi();
extern void Get_Weight();


#endif