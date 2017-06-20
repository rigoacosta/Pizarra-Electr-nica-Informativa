#ifndef __I2CLIB_H
#define __I2CLIB_H

#include <reg51.h>
sbit SCL=P0^0; //PIN CLOCK
sbit SDA=P0^1; //PIN DATA 

typedef unsigned char byte;
typedef unsigned int word;
byte slave_ack;

#define delayus _nop_(); //demora de 1us

//*******************************************
//*Declaracion de funciones y procedimientos*
//*******************************************
void _nop_(void);
void start(void);
void stop(void);
void send_byte(byte c);
byte receive_byte(byte master_ack);
void write_i2c(byte device_id, byte location, byte c);
byte read_i2c(byte device_id, byte location);

//**********************************************
//*Implementacion de funciones y procedimientos*
//**********************************************
void start(void)
{
	SDA = 0; 
	delayus
	 
	SCL = 0;   
	delayus
}

void stop(void)
{
	SDA = 0; 
	SCL = 1; 
	delayus
	SDA = 1; 
	delayus
}

void send_byte(byte c)
{
	byte mask=0x80;
	do   
	{
		if(c&mask) 
			SDA = 1; 
		else
			SDA = 0;  
		 
		
		SCL = 1;  
		delayus
		 
		SCL = 0;  
		delayus
		 
		mask/=2;  
	}while(mask>0);
	 
	SDA = 1;  
	SCL = 1;  
	delayus
	slave_ack=SDA; 
	SCL = 0;  
	delayus
}

byte receive_byte(byte master_ack)
{
	byte c=0,mask=0x80;
	do   
	{
		SCL = 1; 
		delayus
 
		if(SDA==1) 
		c|=mask;    
		SCL = 0;   
		delayus
		mask/=2; 
	}while(mask>0);
 
	if(master_ack==1)
	SDA = 1; 
	else
	SDA = 0; 
 
    SCL = 1; 
    delayus
   
    SCL = 0; 
    delayus
 
	SDA = 1; 
 
	return c;
}

void write_i2c(byte device_id, byte location, byte c)
{
	do
	{
		start();       
		send_byte(device_id);   
		if(slave_ack==1)   
		stop();
	}while(slave_ack==1); 
	 
	send_byte(location); 
	send_byte(c); 
	stop(); 
}

byte read_i2c(byte device_id, byte location)
{
	byte c;
	do
	{
		start();   
		send_byte(device_id); 
		if(slave_ack==1) 
		stop();
	}while(slave_ack==1); 
	 
	send_byte(location);  
	stop(); 
	start(); 
	send_byte(device_id+1); 
	c=receive_byte(1); 
	stop(); 
	return c;
} 

#endif