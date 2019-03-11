#ifndef   _KEYBOARD_H
#define   _KEYBOARD_H

#include <reg52.h>
sbit ROW1=P0^0;
sbit ROW2=P0^1;
sbit ROW3=P0^2;
sbit ROW4=P0^3;
sbit COL4=P0^4;
sbit COL3=P0^5;
sbit COL2=P0^6;
sbit COL1=P0^7;

unsigned char Getkeyboard(void);

#endif