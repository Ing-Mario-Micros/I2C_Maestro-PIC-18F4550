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



/*------------------------------ Funciones I2C ---------------------*/
void Start(void);
void Stop(void);
void Rstart(void);
unsigned char LecturaDato(unsigned char,unsigned char);
/*------------------------------ Funciones para DS ---------------------*/
unsigned char Recibir_Dato_DSPIC(unsigned char,unsigned char);
void Enviar_Dato_DSPIC(unsigned char,unsigned char,unsigned char);
void Enviar_Dato_I2C(unsigned char);
unsigned char Recibir_Dato_I2C(unsigned char);

/*------------------------------ variables del sistema -----------------*/
char Vector_Datos[12] ;

void main(void){
  OSCCON=0b01110000;
  __delay_ms(1);
  TRISD=0x00;
  LATD=0;
  TRISB=0xFF;
  TRISC=0;
  LATC=0;
  SSPCON1=0b00101000;
  SSPCON2=0b00000000;
  SSPSTAT=0b11000000;
  SSPADD=19;
  PIR1=0;  
  //__delay_ms(100);
  while(1){
    __delay_ms(10);
      /*EnvioDato(0x0B,Contador,Contador);*/
      //LATD=LecturaDatoA(0x10);
      if(1== Recibir_Dato_DSPIC(0x10,0x1)){
          LATC0=1;
      }
      else{
          LATC0=0;
      }
      LATD=Vector_Datos[0x1];
      if(1== Recibir_Dato_DSPIC(0x10,0x0)){
          LATC0=1;
      }
      else{
          LATC0=0;
      }
      LATD=Vector_Datos[0x0];
      Enviar_Dato_DSPIC(0x10,0x11,90);
      //LATD = LecturaDato(0x0A,2);
    //EnvioDatoA(0x10,1);
    //Contador++;
    LATC2=1;
    __delay_ms(1000);
    LATC2=0;
    __delay_ms(1000);
  }  
}
void Enviar_Dato_DSPIC(unsigned char Direccion,unsigned char Registro, unsigned char Valor){
  char Verificador_Temp;
  /** Envio de Start **/
  while(RB2 == 1);
  __delay_ms(500);
  Start();
  LATC2=1;
  LATC1=1;
  /** Envio de Dirección **/
  Enviar_Dato_I2C(Direccion & 0b11111110);
  LATC2=0;
  /** Envio de registro a trabajar **/
  Enviar_Dato_I2C(Registro);
  LATC2=1;
  /** Envio de valor al Registro **/
  Enviar_Dato_I2C(Valor);
  LATC2=0;
  /** Envio de Dirección modo Lectura **/
  Rstart();
  Enviar_Dato_I2C(Direccion | 0b00000001);
  LATC2=1;
  /** Resepción de Dato **/
  Verificador_Temp=Recibir_Dato_I2C(0);
  LATC2=0;
  /** Envio de Dirección modo Escritura **/
  Rstart();
  Enviar_Dato_I2C(Direccion & 0b11111110);
  LATC2=1;
  /** Envio de valor al Registro **/
  while(RB2 == 1);
  __delay_ms(500);
  if(Registro==Verificador_Temp){
      Enviar_Dato_I2C(2);
      LATC0=1;
  }
  else{
      Enviar_Dato_I2C(1);
      LATC0=0;
  }
  LATC2=0;
  /** Envio de Stop **/
  while(RB2 == 1);
  __delay_ms(500);
  Stop();
  LATC2=1;
  LATC1=0;
}
unsigned char Recibir_Dato_DSPIC(unsigned char Direccion , unsigned char Registro){
    char Dato_Temp , Verificador_Temp;
    /** Envio de Start **/
  while(RB2 == 1);
  __delay_ms(500);
  Start();
  LATC2=1;
  LATC1=1;
  
  /** Envio de Dirección **/
  Enviar_Dato_I2C(Direccion & 0b11111110);
  LATC2=0;
  
  /** Envio de registro a trabajar **/
  Enviar_Dato_I2C(Registro);
  LATC2=1;
  
  /** Envio de Dirección modo Lectura **/
  Rstart();
  Enviar_Dato_I2C(Direccion | 0b00000001);
  LATC2=0;
  
  /** Resepción de Dato **/
  Dato_Temp=Recibir_Dato_I2C(1);
  LATC2=1;
  
  /** Resepción de Dato **/
  Verificador_Temp=Recibir_Dato_I2C(0);
  LATC2=0;
  
  /** Envio de Stop **/
  while(RB2 == 1);
    __delay_ms(500);
  Stop();
  if(Registro==Verificador_Temp){
      Vector_Datos[Registro]=Dato_Temp;
      
      LATC2=1;
      LATC1=0;
      return 1; 
  }
  else{
      
      LATC2=1;
      LATC1=0;
      return 0;
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