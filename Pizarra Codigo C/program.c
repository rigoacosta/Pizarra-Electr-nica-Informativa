#include <stdio.h>
#include <stdlib.h>
#include "i2c_lib.h"
#include "tools.h"
#include "b_spi.h"
#include <reg52.h>
#define DATA_SIZE 40
#define LF 0x0A
#define CR 0x0D
//****************************
//*Declaracion de direcciones*
//****************************
#define DIR_READ  0xD1 //Lectura para I2C
#define DIR_WRITE 0xD0 //Escritura para I2C

sbit timbre = P0^7;
//*******************************************
//*Declaracion de funciones y procedimientos*
//*******************************************

//Puerto Serie por Hardware
void serieInitHardware();
void execute();

//Utiliza bus serie I2C
void getRTC();
void setRTC();
void fixGetRTC();
void fixSetRTC();

//Utiliza bus serie ISP
word read_mcp3208(byte channel);
void ctrlBrillo();

//Funciones para mostrar info en la pizarra
word rotar(word valor, byte offset, byte dir);
void clearUp(void);
void clearDown(void);
void set_oe_la();
void set_up_letter();
bit get_bit(word number, word offset);
void setPuntos();
void setFechaArriba(byte day, byte decDate, byte uniDate);
void setFechaAbajo();
void setAMPM(byte valor);
void setHora(byte decHora, byte uniHora, byte decMin, byte uniMin);
void setTemp(byte decTemp, byte uniTemp);
byte checkChar(byte valor);
void rotarCartel();
void ctrlRotar();
void setTimbre(byte hora, byte mint, byte AMPM, byte sec);

//**************************
//*Declaracion de variables*
//**************************
byte entrada[DATA_SIZE] = {':','H',1,1,1,1,1,1,1,1,1,3,'F','I','E'};
word temperatura = 29;
byte ctrlTemp = 0;
bit mostrarTemp = 0;
word promedio = 0;
byte centinela  = 0;

// PMW
byte PWM = 0;
word temp  = 0; 

//Variables para Rotar
byte resto0 = 0;
byte resto1 = 0;
byte valorChar = 0;
bit ctrlChar = 0;
byte nextChar = 11;
byte typeChar = 0;
byte columnChar = 0;
byte l = 0;
byte finalChar = 0;
word rotChar = 0;
word idata pizarra[10][6] ;
word auxiliar = 0;
word readMCP = 0;

word code cartelChar[3][27][2][3] = {
    
    { //Letras

        { 
            {4080,16368,26416,},    //A
            {65280,65331,14182,},   
        },

        {
            {16368,28656,26480,},  //B
            {61235,40806,14182,},   
        },

        {
            {16352,4080,26416,},  //C
            {26419,0,14182,}, 
        },


        {
            {16368,4080,26480,}, //D
            {52787,65280,4966,},   
        },

        {
            {16368,28656,26480,},  //E
            {13107,1638,26214,},   
        },

        {
            {4080,28656,26480,},  //F
            {0,1638,26214,}, 
        },

        {
            {16352,4080,26416,}, //G
            {65331,4912,14182,}, 
        },

        {
            {4080,32752,1904,},  //H
            {65280,65399,30464,}, 
        },

        {
            {65520,65520,30576,}, //I
            {0,0,0,}, 
        },

        {
            {16352,0,26112,}, //J
            {3839,4095,26487,}, 
        },

        {
            {65504,65520,30576,}, //K
            {32712,2303,30480,}, 
        },

        { 
            {65520,65520,30576,},    //L
            {13107,0,0,},   
        },

        {
            {4080,4080,14192,},  //M
            {65280,65288,30513,},   
        },

        {
            {4080,61424,6000,},  //N
            {65480,65343,30464,}, 
        },


        {
            {16064,4080,25360,}, //O
            {52787,65280,4966,},   
        },

        {
            {4080,28656,26480,},  //P
            {0,52838,14182,},   
        },

        {
            {16096,4080,25360,},  //Q
            {56955,65280,4966,}, 
        },

        {
            {36848,32752,26480,}, //R
            {14316,52839,14182,}, 
        },

        {
            {13088,28352,26416,},  //S
            {61235,14182,9830,}, 
        },

        {
            {61440,61440,30304,}, //T
            {255,255,26231,}, 
        },

        {
            {16352,4080,1904,}, //U
            {61235,65280,30464,}, 
        },

        {
            {27776,4080,1904,}, //V
            {35939,65280,30464,}, 
        },

        {
            {28656,4080,1904,}, //W
            {65388,65280,30464,}, 
        },

        {
            {4080,32000,1904,},  //X
            {32512,3442,30464,}, 
        },

        {
            {61440,32448,1904,}, //Y
            {255,52855,30464,}, 
        },

        {
            {49136,12544,26208,}, //Z
            {13107,36583,30566,}, 
        },   
    },
    { //Numeros

        { 
            {16352,4080,26416,},    //0
            {61235,65280,14182,},   
        },

        {
            {62256,61440,29456,},  //1
            {13311,255,119,},   
        },

        {
            {65392,4224,26416,},  //2
            {13115,52851,14182,}, 
        },


        {
            {13088,13056,26144,}, //3
            {61235,53043,14182,},   
        },

        {
            {0,14320,12544,},  //4
            {65520,65523,30582,},   
        },

        {
            {13088,16352,26416,},  //5
            {61235,4915,9830,}, 
        },

        {
            {16352,28656,26416,}, //6
            {61235,14182,9830,}, 
        },

        {
            {0,13056,26144,},  //7
            {65280,65331,14182,}, 
        },

        {
            {16352,16320,26416,}, //8
            {44851,53043,14182,}, 
        },

        {
            {13088,16352,26416,}, //9
            {61235,65331,14182,}, 
        },
    },
    { //Simbolos
        {
            {0,30576,0,}, //-
            {0,30583,0,}, 
        },

        {
            {4080,61424,17744,}, //Ñ
            {65480,65303,21828,}, 
        },

        {
            {0,0,0,}, 
            {0,0,0,}, 
        },  

        {
            {0,0,0,}, 
            {0,0,0,}, 
        },     
    },
};

word code numeros[10][2][3] = {

    { 
        {13310,255,52471,}, //0
        {239,255,127,},   
    },

    {
        {65331,65280,65378,}, //1
        {51,0,0,},   
    },

    {
        {49143,12544,52455,}, //2
        {51,239,127,}, 
    },


    {
        {13106,26208,52420,}, //3
        {239,159,127,},   
    },

    {
        {0,26350,50737,}, //4
        {255,255,255,},   
    },

    {
        {13106,26348,52471,}, //5
        {239,55,76,}, 
    },

    {
        {13310,26367,52471,}, //6
        {239,55,76,}, 
    },

    {
        {0,26208,52420,}, //7
        {255,255,127,}, 
    },

    {
        {13310,26361,52471,}, //8
        {239,159,127,}, 
    },

    {
        {13106,26348,52471,}, //9
        {239,255,127,}, 
    },
};

byte datosReloj[8];

void main(void)
{   
    timbre = 0;
    ctrlChar = 1;	
    Soft_SPI_Init();			//Bus SPI iniciado
    serieInitHardware();		//Inicializar puerto serie por hardware
    clearDown();
    clearUp();

    while(1)
    {	
        getRTC();
        fixGetRTC();
        
        setTimbre(datosReloj[2], datosReloj[1], datosReloj[7], datosReloj[0]);
        if(datosReloj[0] == 10)
            timbre = 0;

        if(((datosReloj[0]>15) && (datosReloj[0]<25)) || ((datosReloj[0] > 40) &&  (datosReloj[0] < 50)))
        {        
            clearUp();
            ctrlTemp = 0;    
            mostrarTemp = 0;
            setTemp(temperatura/10, temperatura%10);
        } 
        else
        {   
            clearUp();
            setAMPM(datosReloj[7]);
            setHora(datosReloj[2]/10, datosReloj[2]%10, datosReloj[1]/10, datosReloj[1]%10);
            setPuntos();

            if(ctrlTemp < 5)
            {
                ES = 0;
                promedio += read_mcp3208(0);
                ES = 1;
                ctrlTemp++;
            }
            else
            {
                if(mostrarTemp == 0)
                {
                    temperatura = promedio / 5;
                    promedio = 0;
                    mostrarTemp = 1;
                }
            }

        }

    	ctrlBrillo();
    	ctrlRotar();
    	rotarCartel();
    	set_up_letter();
    }    
}

//**********************************************
//*Implementacion de funciones y procedimientos*
//**********************************************
void serieInitHardware()//Se inicializa a 9600 BAUDS
{
	TMOD = 0x21; //Temporizador 1 en Modo 2 autorecarga
	TH1 = 0xFD;  //Tiempo de Conteo
	SCON = 0x50; //Modo 1 con 1 byte de datos
	ES = 1;
	PS = 1;
	PT1 = 1;
	TR1 = 1;  //Se inicia el temporizador 

	// Timer 0
    TH0 = 0x00;
    TL0 = 0x00;
    ET0 = 1;
    TR0 = 1;
    PWM = 255; 
    RS485 = 0;
    EA = 1;
}

void getRTC()   //Obtener informacion del DS1307
{	
	datosReloj[0] = read_i2c(DIR_WRITE,0x00);
	datosReloj[1] = read_i2c(DIR_WRITE,0x01);
	datosReloj[2] = read_i2c(DIR_WRITE,0x02);
	datosReloj[3] = read_i2c(DIR_WRITE,0x03);
	datosReloj[4] = read_i2c(DIR_WRITE,0x04);
	datosReloj[5] = read_i2c(DIR_WRITE,0x05);
	datosReloj[6] = read_i2c(DIR_WRITE,0x06);
}

void setRTC()   //Poner informacion en el DS1307
{
	start();
    send_byte(DIR_WRITE);
	send_byte(0x00);
    send_byte(datosReloj[0]);
    send_byte(datosReloj[1]);
	send_byte(datosReloj[2]);
	send_byte(datosReloj[3]);
	send_byte(datosReloj[4]);
	send_byte(datosReloj[5]);
	send_byte(datosReloj[6]);
    stop();
}

void SerialPortInt() interrupt 4//Interrupcion del Puerto Serie del Hardware
{
	byte recepcion = 0;
    
    if(RI == 1)  //Se verifica que es para recepcion
	{
        if(RS485 == 0)
        {		
            recepcion = SBUF;
    		RI = 0;
            if(recepcion == 0) return;

    		if(centinela == 0)
    		{
    			if(recepcion == ':')
    			{
    				entrada[centinela++] = recepcion;
    			}
            }
    		else
    	    {             
                entrada[centinela++] = recepcion;
                if(centinela < DATA_SIZE)
                {
                    if(entrada[centinela-1] == LF || entrada[centinela-1] == CR)
                    {   
                       execute();
                    }
                }
    	    }
        }
        else
        {
            RI == 0;
        }
    }
}

void execute()  //Realiza las operaciones enviadas
{	
    byte i;
    // Centinala-1 posee la posicion de CR/LF
    // Centinela-2 posicion de la ultima letra 
	if(entrada[1] == 'H')
	{
        datosReloj[0] = entrada[2];  
		datosReloj[1] = entrada[3];
        datosReloj[2] = entrada[4];
		datosReloj[3] = entrada[5];
		datosReloj[4] = entrada[6];
		datosReloj[5] = entrada[7];
		datosReloj[6] = entrada[8];
        if(entrada[9] == 'P')
            datosReloj[7] = 1;
        if(entrada[9] == 'A')
            datosReloj[7] = 0;
        fixSetRTC();
        setRTC();
        centinela = 0;
	}
    if( entrada[1] == 'C')
    {
        byte total = entrada[2]; //Cantidad de letras existentes
        byte  startCartel = 0;  
        //Cartel colocar a partir de entrada[11] == cantidad de caracteres 
        for(i = 0; i <= total; i++)
        {   
            startCartel = DATA_SIZE-i; //puntero del cartel
            entrada[startCartel] = entrada[(centinela-2) - i]; //Copia los datos entrantes de atras hacia delante 
        }

        for(i = 0; i <= total; i++)
            entrada[11+i] = entrada[startCartel+i];   //Luego se trae hasta la posicion 11

        resto0 = 0;
        resto1 = 0;
        valorChar = 0;
        ctrlChar = 0;
        nextChar = 11;
        typeChar = 0;
        columnChar = 0;
        l = 0;
        finalChar = 0;
        rotChar = 0;
        auxiliar = 0;
        clearDown();
        ctrlChar = 1;
        centinela = 0;
    }
}

word read_mcp3208(byte channel) //Leer desde el A/D
{
	word lectura;
	byte dataHigh, dataLow, datos;

	datos = 0x06;

	if(channel > 3)
		datos |= 0x01;

	CS_LOW();

	Soft_SPI_Write(datos);
	datos = channel<<6;
	dataHigh = Soft_SPI_Transfer(datos);
	dataHigh &= 0x0F;
	dataLow = Soft_SPI_Read();
	
	CS_HIGH();
	lectura = ((dataHigh << 8)|dataLow) << 1;
	if(channel == 0)
	lectura  = ((lectura*5.0)/4096)*100;
	if(channel == 1)
	lectura  = ((lectura*5.0)/4096)*10;
	return lectura;
}

void intPWM() interrupt 1
{
    TR0 = 0;

    if(oe)
    {
        oe = 0;     //Tiempo encedido
        temp = (255-PWM)*PWM_Freq_Num;
        TH0 = 0xFF - (temp>>8)&0xFF;
        TL0 = 0xFF - (temp&0xFF);
    }
    else
    {
        oe = 1;   //Tiempo apagado
        temp = PWM*PWM_Freq_Num;
        TH0 = 0xFF - (temp>>8)&0xFF;
        TL0 = 0xFF - (temp&0xFF); 
    }

    TF0 = 0;   
    TR0 = 1;
}

word rotar(word valor, byte offset, byte dir)
{
    if(dir == 1)
    {
        return valor << offset;
    }
    else
    {
        return valor >> offset;
    }
}

void clearDown(void)
{
    byte i, j;
    for(i = 0; i< 10; i++)
        for(j = 0; j < 3; j++)
            pizarra[i][j] = 0;
}

void clearUp(void)
{   
    byte i, j;
    for(i = 0; i< 10; i++)
        for(j = 3; j < 6; j++)
            pizarra[i][j] = 0;
}

void set_oe_la()
{    
    oe = 1;
    DELAY_ms(1);
    la= 1;
    DELAY_ms(1);
    la = 0;
    DELAY_ms(1);
    oe = 0; 
}

bit get_bit(word number, word offset)
{
    return ( (number & (1 << offset)) > 0 );
}

void set_up_letter() 
{
    byte k, j;
    DELAY_ms(1);
    clkP = 0;
    DELAY_ms(1);

    for ( k = 0; k < 6; k++ )           
    {                 
        for ( j = 0; j < 16; j++ )
        {
            f1 = get_bit(pizarra[0][k], j);
            f2 = get_bit(pizarra[1][k], j);
            f3 = get_bit(pizarra[2][k], j);
            f4 = get_bit(pizarra[3][k], j);
            f5 = get_bit(pizarra[4][k], j);
            f6 = get_bit(pizarra[5][k], j);
            f7 = get_bit(pizarra[6][k], j);
            f8 = get_bit(pizarra[7][k], j);
            f9 = get_bit(pizarra[8][k], j);
            f10 = get_bit(pizarra[9][k], j); 
            
            clkP = 1;
            DELAY_ms(1);
            clkP = 0;
            DELAY_ms(1);
        }             
    }
    
    set_oe_la();
}

void setPuntos()
{
    pizarra[4][3] = (204)|(pizarra[4][3]);
    pizarra[4][4] = (136)|(pizarra[4][4]);
    pizarra[4][5] = (17)|(pizarra[4][5]);
}

void setAMPM(byte valor)
{
    if(valor == 0) //AM
    {
        pizarra[8][3] = 9456; 
        pizarra[8][4] = 35040;
        pizarra[8][5] = 17456;

        pizarra[9][3] = 244;
        pizarra[9][4] = 232;
        pizarra[9][5] = 52;
    }

    if(valor == 1) //PM
    {
        pizarra[8][3] = 9456; 
        pizarra[8][4] = 35040;
        pizarra[8][5] = 17456;

        pizarra[9][3] = 244;
        pizarra[9][4] = 8;
        pizarra[9][5] = 52;
    }
}

void setHora(byte decHora, byte uniHora, byte decMin, byte uniMin)
{
    byte i;
    //Decenas de horas
    if(decHora == 0)
    {
        for(i=3 ; i<6 ; i++)
        {
            pizarra[0][i] = 0;
            pizarra[1][i] = 0;
        }
    }
    else
    {
        for(i= 3; i < 6; i++)
        {
            pizarra[0][i] = 0;
            pizarra[0][i] = (rotar(numeros[1][0][i-3],8,1))&65280;
            auxiliar = (rotar(numeros[1][0][i-3],8,0)&255)|(rotar(numeros[1][1][i-3],8,1)&65280);
            pizarra[1][i] = 0;
            pizarra[1][i] = auxiliar;

        }
    }
    
    
    for(i = 3; i < 6 ; i++)
    {
        //Unidades de horas
        pizarra[2][i] = 0;
        pizarra[2][i] = (rotar(numeros[uniHora][0][i-3],4,1)&65520);
        auxiliar = (rotar(numeros[uniHora][0][i-3],12,0)&15)|(rotar(numeros[uniHora][1][i-3],4,1)&4080);
        pizarra[3][i] = 0;
        pizarra[3][i] = auxiliar;   
       
        //Unidades de Minutos
        auxiliar = (rotar(numeros[uniMin][0][i-3],8,0)&255)|(rotar(numeros[uniMin][1][i-3],8,1)&65280);
        pizarra[7][i] = 0;
        pizarra[7][i] = auxiliar;

         //Decenas de Minutos
        pizarra[4][i] = 0;
        pizarra[4][i] |= (rotar(numeros[decMin][0][i-3],12,1)&61440);
        auxiliar = (rotar(numeros[decMin][0][i-3],4,0)&4095)|(rotar(numeros[decMin][1][i-3],12,1)&61440);
        pizarra[5][i] = 0;
        pizarra[5][i] = auxiliar;
        pizarra[6][i] = 0;
        pizarra[6][i] = (rotar(numeros[decMin][1][i-3],4,0)&15) | (rotar(numeros[uniMin][0][i-3],8,1)&65280);
    

    }
}

void setTemp(byte decTemp, byte uniTemp)
{
    byte i;
    for(i = 3; i<6 ; i++)
    {
        pizarra[1][i] = numeros[decTemp][0][i-3];
        pizarra[2][i] = numeros[decTemp][1][i-3];

        pizarra[3][i] = numeros[uniTemp][0][i-3];
        pizarra[4][i] = numeros[uniTemp][1][i-3];
    }
	//Simbolo de grado celsius
    pizarra[5][3] = 0;
    pizarra[5][4] = 51200;
    pizarra[5][5] = 63232;

    pizarra[6][3] = 0;
    pizarra[6][4] = 36044;
    pizarra[6][5] = 32716;

    pizarra[7][3] = 65024;
    pizarra[7][4] = 65280;
    pizarra[7][5] = 63232;

    pizarra[8][3] = 26419;
    pizarra[8][4] = 0;
    pizarra[8][5] = 28364;
}

//Verifica el tipo de caracter
byte checkChar(byte valor)
{
    if((valor >= '0') && (valor <= '9'))
        return 1;
    if((valor >= 'a') && (valor <= 'z'))
        return 0;
    if((valor >= 'A') && (valor <= 'Z'))
        return 3;
    if((valor== 0x10) || (valor== 0x11) || (valor== 0x12))
        return 2;
}
//Rota el cartel
void rotarCartel()
{
    byte i, j;
    for(i = 0; i < 3; i++) //Se rota una sola columna de LEDs
    {
        resto0 = rotar(cartelChar[typeChar][valorChar][columnChar][i],(l*4),0) & 0xF;   //Extrae la columna LED de la letra ENTRANTE
                                                                                       //Se extrae por fila de la pizarra 
        for(j = 0; j < 10; j++)
        {
             
             resto1 = (pizarra[9-j][i] & 0xF);                  //Obtine los 4 bits LSB
             auxiliar = rotar(pizarra[9-j][i],4,0) & 4095;    //Rotacion  y elimina los 4 bits LSB
             rotChar = rotar(resto0,12,1) & 61440;              //Los 4 bits entrantes los pongo como 4 bits MSB
             pizarra[9-j][i] = (auxiliar | rotChar);            //Creo la nueva cuadricula
             resto0 = resto1;                                  //Los 4 bits LSB los pasa a la proxima cudricula como MSB           
             
        }
        
    }

    //Cada vez q se llama la funcion de rotarCartel se rota una columna LED
    if(l == 3)   //l controla las columnas lEDs que van entrando
    {
        l = 0;
        
        if((typeChar == 0) && (valorChar == 8))   //Provoca q para la I rote solo 4 columnas LEDs
            columnChar = 1;

        if((typeChar == 2) && (valorChar == 2))   //Provoca q para espacios rote solo 4 columnas LEDs
            columnChar = 1;
        
        if(columnChar == 1) //Como las letras estan formadas por 2 columnas de 4 columnas LEDs
        {                   //Controlar esas columnas
            columnChar = 0;
            ctrlChar = 1;
            if((typeChar == 2) && (valorChar == 3))
            {
                finalChar++;
            }
        }
        else
        {
            columnChar++;
        }
    }
    else
    {
        l++;
    }
}

void ctrlRotar()
{
    //Control de la rotacion de la pizarra
    if(ctrlChar == 1) //ctrlChar 
    {
        if(nextChar < (entrada[11]+11))
        {
            ctrlChar = 0;   
            nextChar++;
            typeChar = checkChar(entrada[nextChar]);
            if(typeChar == 1)   //Numero
                valorChar = entrada[nextChar] - '0';
            if(typeChar == 0)   //Letra
                valorChar = entrada[nextChar] - 'a';
            if(typeChar == 3)
            {
                typeChar = 0;
                valorChar = entrada[nextChar] - 'A';
            }
            if(typeChar == 2)   //Simbolo creado
                valorChar = entrada[nextChar] - 0x10;
        }
        else   //Poner espacio despues q termina la info
        {
            typeChar = 2;
            valorChar = 3;
            
            if(finalChar == 4)  //Cantidad de espacios a poner
            {
                nextChar = 11;  // Empezar a mostrar la informacion de nuevo.
                finalChar = 0;
            }

        }
    }
}

void ctrlBrillo()
{
    readMCP = read_mcp3208(1);
    if(readMCP <= 8)
    {
        PWM = 0;
    }
    else if(readMCP <= 41 && readMCP > 8)
    {
        PWM = 125;
    }
    else
    {
        PWM = 255;
    }
}

//Lleva los datos a decimal
void fixGetRTC()
{
    if((datosReloj[2] & 0x20) == 0)
        datosReloj[7] = 0;   //AM
    else
        datosReloj[7] = 1;  //PM

    datosReloj[2] &= 0x1F; 
    datosReloj[0] = bcdToDec(datosReloj[0]);
    datosReloj[1] = bcdToDec(datosReloj[1]);
    datosReloj[2] = bcdToDec(datosReloj[2]);
    datosReloj[3] = bcdToDec(datosReloj[3]);
    datosReloj[4] = bcdToDec(datosReloj[4]);
    datosReloj[5] = bcdToDec(datosReloj[5]);
    datosReloj[6] = bcdToDec(datosReloj[6]);
}
//Lleva los datos a BCD
void fixSetRTC()
{
    datosReloj[0] = decToBcd(datosReloj[0]);
    datosReloj[1] = decToBcd(datosReloj[1]);
    datosReloj[2] = decToBcd(datosReloj[2]);
    datosReloj[3] = decToBcd(datosReloj[3]);
    datosReloj[4] = decToBcd(datosReloj[4]);
    datosReloj[5] = decToBcd(datosReloj[5]);
    datosReloj[6] = decToBcd(datosReloj[6]);

    datosReloj[2] += 64;
    if(datosReloj[7] == 1)
        datosReloj[2] += 32;
    if(datosReloj[7] == 0)
        datosReloj[2] &= 0x5F;     
}

void setTimbre(byte hora, byte mint, byte AMPM, byte sec)
{
    if((hora == 8) && (mint == 30) && (AMPM == 0) && (sec == 0))
    {
        timbre = 1;
    }

    if((hora == 9) && (mint == 55) && (AMPM == 0) && (sec == 0))
    {
        timbre = 1;
    }

    if((hora == 10) && (mint == 5) && (AMPM == 0) && (sec == 0))
    {
        timbre = 1;
    }

    if((hora == 11) && (mint == 30) && (AMPM == 0) && (sec == 0))
    {
        timbre = 1;
    }

    if((hora == 11) && (mint == 40) && (AMPM == 0) && (sec == 0))
    {
        timbre = 1;
    }

    if((hora == 1) && (mint == 5) && (AMPM == 1) && (sec == 0))
    {
        timbre = 1;
    }

    if((hora == 1) && (mint == 15) && (AMPM == 1) && (sec == 0))
    {
        timbre = 1;
    }

    if((hora == 2) && (mint == 40) && (AMPM == 1) && (sec == 0))
    {
        timbre = 1;
    }

    if((hora == 2) && (mint == 50) && (AMPM == 1) && (sec == 0))
    {
        timbre = 1;
    }

    if((hora == 4) && (mint == 15) && (AMPM == 1) && (sec == 0))
    {
        timbre = 1;
    }

    if((hora == 4) && (mint == 25) && (AMPM == 1) && (sec == 0))
    {
        timbre = 1;
    }

    if((hora == 5) && (mint == 50) && (AMPM == 1) && (sec == 0))
    {
        timbre = 1;
    }

}


