/* 
 * File:   I2C.h
 * Author: mario
 *
 * Created on 1 de agosto de 2022, 08:59 AM
 */

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#include<xc.h>
/*------------------------------ Funciones I2C ---------------------*/
void Start(void);
void Stop(void);
void Rstart(void);
unsigned char LecturaDato(unsigned char,unsigned char);
/*------------------------------ Funciones para DS ---------------------*/
void Enviar_Dato_I2C(unsigned char);
unsigned char Recibir_Dato_I2C(unsigned char);
/*------------------------------ Funciones I2C ---------------------*/
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
/*------------------------------ Funciones para DS ---------------------*/
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
#endif	/* I2C_H */

