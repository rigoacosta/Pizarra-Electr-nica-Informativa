#ifndef __TOOLS_H
#define __TOOLS_H

#include <reg52.h>

typedef unsigned char byte;
typedef unsigned int word;
typedef unsigned long dword;

//Frecuencia para PWM
#define PWM_Freq_Num 1
/* Columnas de la pizarra */
sbit f1 = P1^0;
sbit f2 = P1^1;
sbit f3 = P2^6;
sbit f4 = P2^7;
sbit f5 = P2^4;
sbit f6 = P2^5;
sbit f7 = P2^2;
sbit f8 = P2^3;
sbit f9 = P2^0;
sbit f10 = P2^1;

//Pin para Tx/Rx
sbit RS485 = P3^2;
/* CLK & LA */
sbit clkP = P3^4;
sbit la = P3^5;
sbit oe = P3^6;

/* LEDs */
sbit LED = P3^7;

//*******************************************
//*Declaracion de funciones y procedimientos*
//*******************************************
byte bcdToDec(byte val);
byte decToBcd(byte val);
void DELAY_ms(word ms_count);
void DELAY_us(word us_count);
byte decToAscii(byte val);


//**********************************************
//*Implementacion de funciones y procedimientos*
//**********************************************
byte bcdToDec(byte val)
{
	return ((val/16*10) + (val%16)); 
}

byte decToBcd(byte val)
{
	return ((val/10*16) + (val%10));
}

void DELAY_us(word us_count)
 {    
    while(us_count!=0)
      {
         us_count--;
       }
 }

 void DELAY_ms(word ms_count)
 {
     while(ms_count!=0)
      {
         DELAY_us(112u);     
          ms_count--;
      }

 }

byte decToAscii(byte val)
{
	 return val + '0';
}

#endif