/*
 * File:   MainI2C_Controlador.c
 * Author: mario
 *
 * Created on 1 de Agosto de 2022, 09:24 AM
 */

//Envió el cpodigo de I2C:

#include<xc.h>
#define _XTAL_FREQ 8000000
#include "I2C.h"
#pragma config FOSC=INTOSC_EC
#pragma config WDT=OFF
#pragma config PBADEN=OFF

/*------------------------------ variables del sistema -----------------*/
extern char Vector_Datos[12] ;

void main(void){
  OSCCON=0b01110000;
  __delay_ms(1);
  TRISD=0x00;
  LATD=0;
  TRISB=0xFF;
  TRISC=0;
  LATC=0;
  Activar_I2C_Maestro();
  while(1){
    __delay_ms(10);
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
      Enviar_Dato_DSPIC(0x10,0x11,0);
    LATC2=1;
    __delay_ms(1000);
    LATC2=0;
    __delay_ms(1000);
  }  
}













