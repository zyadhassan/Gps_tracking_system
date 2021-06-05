#include <stdio.h>
#include <math.h>

#include"stdint.h"
#include"math.h"

#include"C:/Keil/EE319Kware/inc/tm4c123gh6pm.h"

#define RED 0x02
#define BLUE 0x04
#define GREEN 0x08

void init(){
SYSCTL_RCGCGPIO_R |=0x20 ;
while((SYSCTL_PRGPIO_R & 0x20)==0){};

GPIO_PORTF_LOCK_R=0x4C4F434B; 
GPIO_PORTF_CR_R=; //on for all pins
GPIO_PORTF_AFSEL_R=; //no AF
GPIO_PORTF_PCTL_R=;  //all pins GPIO
GPIO_PORTF_AMSEL_R=; // all digital
GPIO_PORTF_DIR_R=; //f1 & f2 & f3 output 
GPIO_PORTF_DEN_R=; //all pins digital
GPIO_PORTF_PUR_R=; // on for first and last pin
//GPIO_PORTF_DATA_R&=0x0E;


}