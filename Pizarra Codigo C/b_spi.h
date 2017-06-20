#ifndef __BSPI_H
#define __BSPI_H

#include <reg52.h>
#include <intrins.h>

typedef unsigned char byte;
typedef unsigned int word;

//****************************
//*Declaracion de los puertos*
//****************************
sbit clk=P0^2;   // Conectado al CLK del MCP3208  
sbit mosi=P0^4;  // Conectado al din del MCP3208
sbit miso=P0^3;  // Conectado al dout del MCP3208
sbit cs=P0^5;    // Conectado al CS/SHDN del MCP3208

//*******************************************
//*Declaracion de funciones y procedimientos*
//*******************************************
void Soft_SPI_Init();
void Soft_SPI_Write(byte val);
byte Soft_SPI_Read();
byte Soft_SPI_Transfer(byte val);
void CS_LOW();
void CS_HIGH();
void delay_us();

//**********************************************
//*Implementacion de funciones y procedimientos*
//**********************************************
void Soft_SPI_Init()
{
	cs = 1;
	clk = 1;
	mosi = 1;
	miso = 1;
}

void CS_LOW()
{
	cs = 0;
}

void CS_HIGH()
{
	cs = 1;
}

void Soft_SPI_Write(byte val)
{
	byte i;
	for (i = 0; i < 8; i++)
	{
		 if((val & 0x80) == 0)
                mosi = 0;
         else
                mosi = 1;
         clk = 0;
         delay_us();
         clk = 1;   
         val <<= 1;
	}
}


byte Soft_SPI_Read()
{
	byte i, value;
	value = 0x00;
	for(i=0;i<8;i++)
	{	
		value<<=1;
		
		if(miso==1)
		{
			value=value+1;

		}
 		else
		{
			value=value+0;
		} 
		clk=0;
		delay_us();
		clk=1;
				
	}

	return value;
}

byte Soft_SPI_Transfer(byte val)
{
	byte i, value;
	value = 0x00;
	for(i=0;i<8;i++)
	{	
		value<<=1;
		
		if(miso==1)
		{
			value=value+1;

		}
 		else
		{
			value=value+0;
		} 
        
        if((val & 0x80) == 0)
            mosi = 0;
        else
            mosi = 1;
		
		clk=0;
		delay_us();
		clk=1;
		val<<=1;
	}

	return value;
}


void delay_us()
 {
  _nop_ (); 
 }

#endif