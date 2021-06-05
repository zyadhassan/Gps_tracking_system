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
GPIO_PORTF_CR_R=0x1F; //on for all pins
GPIO_PORTF_AFSEL_R=0; //no AF
GPIO_PORTF_PCTL_R=0;  //all pins GPIO
GPIO_PORTF_AMSEL_R=0; // all digital
GPIO_PORTF_DIR_R=0x0E; //f1 & f2 & f3 output 
GPIO_PORTF_DEN_R=0X1F; //all pins digital
GPIO_PORTF_PUR_R=0x11; // on for first and last pin
//we now will comment
}
float radians(float n) {
    return n * 3.141592654 / 180; // to be used in calculation of distance 
}
// GPS_distance_between (30.02350, 31.178291, 30.003019, 31.177931) ;

float GPS_distance_between (float lat1,float long1,float lat2,float long2) {
  // Courtesy of Maarten Lamers

 // float lat1=30.002350;
  //float lat2 =30.003019;
  //float long1=31.178291;
  //float long2=31.177931;



  float delta = radians(long1-long2);
  float sdlong = sin(delta);
  float cdlong = cos(delta);
  lat1 = radians(lat1);
  lat2 = radians(lat2);
  float slat1 = sin(lat1);
  float clat1 = cos(lat1);
  float slat2 = sin(lat2);
  float clat2 = cos(lat2);
  delta = (clat1 * slat2) - (slat1 * clat2 * cdlong);
  delta = pow(delta,2);
  delta += pow(clat2 * sdlong,2);
  delta = sqrt(delta);
  float denom = (slat1 * slat2) + (clat1 * clat2 * cdlong);
  delta = atan2(delta, denom);
  return ( delta * 6372795);



}