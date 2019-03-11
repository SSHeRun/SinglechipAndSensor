#ifndef   _KEYBOARD_H
#define   _KEYBOARD_H

#include <reg52.h>
sbit ROW1=P3^0;
sbit ROW2=P3^1;
sbit ROW3=P3^2;
sbit ROW4=P3^3;
sbit COL4=P3^4;
sbit COL3=P3^5;
sbit COL2=P3^6;
sbit COL1=P3^7;

unsigned char Getkeyboard(void);

#endif