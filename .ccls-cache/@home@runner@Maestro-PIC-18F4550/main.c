/*
 * File:   MainI2CIMU.c
 * Author: mario
 *
 * Created on 20 de diciembre de 2021, 02:36 PM
 */

//Envió el cpodigo de I2C:

#include<xc.h>
#define _XTAL_FREQ 8000000
#pragma config FOSC=INTOSC_EC
#pragma config WDT=OFF
#pragma config PBADEN=OFF



void Start(void);
void Stop(void);
void Rstart(void);
void EnvioDato(unsigned char,unsigned char,unsigned char);
unsigned char LecturaDato(unsigned char,unsigned char);
unsigned char LecturaDatoA(unsigned char);
void EnvioDatoA(unsigned char Direccion, unsigned char Dato);
char Contador =0;

/*------------------------------ Funciones para DS ---------------------*/

void Enviar_Dato_I2C(unsigned char);
unsigned char Recibir_Dato_I2C(unsigned char);

void main(void){
  unsigned char aux;
  int ax,ay,az,gx,gy,gz;
  OSCCON=0b01110000;
  __delay_ms(1);
  TRISD=0x00;
  LATD=0;
  TRISB=0xFF;
  TRISE=0;
  LATE=0;
  SSPCON1=0b00101000;
  SSPCON2=0b00000000;
  SSPSTAT=0b11000000;
  SSPADD=19;
  PIR1=0;  
  //__delay_ms(100);
  while(1){
    __delay_ms(10);
      /*EnvioDato(0x0B,Contador,Contador);*/
      LATD=LecturaDatoA(0x10);
      //LATD = LecturaDato(0x0A,2);
    //EnvioDatoA(0x10,1);
    //Contador++;
    LATE2=1;
    __delay_ms(1000);
    LATE2=0;
    __delay_ms(1000);
  }  
}

void Enviar_Dato_I2C(unsigned char Dato){
    while(RB2 == 1);
    __delay_ms(250);
    SSPBUF= Dato; //| 0b00000001; //& 0b11111110;
    SSPIF=0;
    while(SSPIF==0);
}
unsigned char Recibir_Dato_I2C(unsigned char Dato_Adicional){
    while(RB2 == 1);
    __delay_ms(500);
    RCEN=1;
    SSPIF=0;
    while(SSPIF==0);
    //LATD=SSPBUF;
    //LATE2=1;

    /** Envio de ACK **/
    SSPIF=0;
    if(Dato_Adicional==1) ACKDT=0;
    else ACKDT=1;
    ACKEN=1;
    while(SSPIF==0);
    //LATE2=0;
    return SSPBUF;
}

void Start(void){    
  SEN=1;
  while(SEN==1);  
}
void Stop(void){
  PEN=1;
  while(PEN==1);  
}
void Rstart(void){
  RSEN=1;
  while(RSEN==1);  
}
void EnvioDato(unsigned char Direccion, unsigned char Registro, unsigned char Dato){
  Start();
  SSPBUF=Direccion; //& 0b11111110;
  SSPIF=0;
  while(SSPIF==0);
  
  SSPBUF=Registro;
  SSPIF=0;
  while(SSPIF==0);
  
  SSPBUF=Dato;
  SSPIF=0;
  while(SSPIF==0);
  
  SSPIF=0;
  Stop();  
}
unsigned char LecturaDato(unsigned char Direccion, unsigned char Registro){
  Start();
  SSPBUF=Direccion & 0b11111110;
  SSPIF=0;
  while(SSPIF==0);
  SSPBUF=Registro;
  SSPIF=0;
  while(SSPIF==0);
  Rstart();
  SSPBUF=Direccion | 0b00000001;
  SSPIF=0;
  while(SSPIF==0);
  RCEN=1;
  SSPIF=0;
  while(SSPIF==0);
  
  SSPIF = 0;
  ACKDT = 1;
  ACKEN = 1;
  while(SSPIF==0);
  
  
  Stop();
  return SSPBUF;  
}
unsigned char LecturaDatoA(unsigned char Direccion){
  /** Envio de Start **/
  while(RB2 == 1);
  __delay_ms(500);
  Start();
  LATE2=1;
  LATE1=1;
  
  /** Envio de Dirección **/
  Enviar_Dato_I2C(Direccion & 0b11111110);
  /*while(RB2 == 1);
    __delay_ms(500);
  SSPBUF=Direccion& 0b11111110; //| 0b00000001; //& 0b11111110;
  SSPIF=0;
  while(SSPIF==0);/**/
  LATE2=0;
  
  /** Envio de registro a trabajar **/
  Enviar_Dato_I2C(0x1);
  /*while(RB2 == 1);
  __delay_ms(500);
  SSPBUF=0x1; 
  SSPIF=0;
  while(SSPIF==0);/**/
  LATE2=1;
  
  /** Envio de Dirección modo Lectura **/
  /*while(RB2 == 1);
  __delay_ms(500);/**/
  Rstart();
  Enviar_Dato_I2C(Direccion | 0b00000001);
  /*SSPBUF=Direccion | 0b00000001;
  SSPIF=0;
  while(SSPIF==0);/**/
  LATE2=0;
  
  /** Resepción de Dato **/
  
  /*while(RB2 == 1);
    __delay_ms(500);
  RCEN=1;
  SSPIF=0;
  while(SSPIF==0);
  LATD=SSPBUF;/**/
  //LATE2=1;
  LATD=Recibir_Dato_I2C(1);
  /** Envio de ACK **/
  /*SSPIF=0;
  ACKDT=0;
  ACKEN=1;
  while(SSPIF==0);/**/
  LATE2=1;
  
  /** Resepción de Dato **/
  
  /*while(RB2 == 1);
    __delay_ms(500);
  RCEN=1;
  SSPIF=0;
  while(SSPIF==0);
  LATD=SSPBUF;
  LATE2=1;/**/
  LATD=Recibir_Dato_I2C(0);
  /** Envio de NACK **/
  /*SSPIF=0;
  ACKDT=1;
  ACKEN=1;
  while(SSPIF==0);/**/
  LATE2=0;
  
  /** Envio de Stop **/
  while(RB2 == 1);
    __delay_ms(500);
  Stop();
  LATE2=1;
  LATE1=0;
  return SSPBUF;  
}
void EnvioDatoA(unsigned char Direccion, unsigned char Dato){
    while(RB2 == 1);
    __delay_ms(500);
    Start();
    LATE2=1;
    LATE1=1;
    
    while(RB2 == 1);
    __delay_ms(500);
    SSPBUF=Direccion; //& 0b11111110;
    SSPIF=0;
    while(SSPIF==0);
    LATE2=0;
    
    while(RB2 == 1);
    __delay_ms(500);
    SSPBUF=Dato;
    SSPIF=0;
    while(SSPIF==0);
    LATE2=1;
    
    while(RB2 == 1);
    __delay_ms(500);
    SSPIF=0;
    Stop(); 
    LATE1=0;
    LATE2=0;
}