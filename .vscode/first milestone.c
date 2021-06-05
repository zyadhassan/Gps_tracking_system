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
}
float radians(float n) {         // to be used in calculation of distance
    return n * 3.141592654 / 180; 
}

float GPS_distance_between (float lat1,float long1,float lat2,float long2) { // return distancce between two points with coordinates
  float distance = radians(long1-long2);     
  float sdlong = sin(distance);
  float cdlong = cos(distance);
  lat1 = radians(lat1);
  lat2 = radians(lat2);
  float slat1 = sin(lat1);
  float clat1 = cos(lat1);
  float slat2 = sin(lat2);
  float clat2 = cos(lat2);
  distance = (clat1 * slat2) - (slat1 * clat2 * cdlong);
  distance = pow(distance,2);
  distance += pow(clat2 * sdlong,2);
  distance = sqrt(distance);
  float denom = (slat1 * slat2) + (clat1 * clat2 * cdlong);
  distance = atan2(distance, denom);
  return ( distance * 6372795);
}
/*
For Testing 
GPS_distance_between (30.02350, 31.178291, 30.003019, 31.177931) ;

*/
int Total_Distance(){
int x;
float long2 ;
float lat2 ;
float lat1 ;               
float long1 ;       
float totalDistance = 0.0;
if ((GPIO_PORTF_DATA_R&0x11)==0x01) //switch of pin 0 to start calculate distance and Recieve coordinates
   {
x=1;
long2 = GPS_LONG(); // update data from gps
lat2 = GPS_LAT();   // update data from gps
lat1 = GPS_LAT();   // update data from gps
long1 = GPS_LONG(); // update data from gps
}
while (x){
float distance = GPS_distance_between(lat1,long1,lat2,long2);
totalDistance=totalDistance+distance;
long2=long1;
lat2=lat1;
long1 = GPS_LONG(); // update data from gps 
lat1 = GPS_LAT();   // update data from gps
if ((GPIO_PORTF_DATA_R&0x11)==0x10) { // switch of pin 4 to end calculation of the total distane taken 
           x=0;
           return totalDistance; // Return toatal distance 
}
}
}