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
  float distance = radians(long1-long2);         //difference between longs with radians
  float sdlong = sin(distance);                 //we will prepare the parameters of the law 
  float cdlong = cos(distance);
  lat1 = radians(lat1);              //lat1 in radians
  lat2 = radians(lat2);                    //lat2 in radians 
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
  return ( distance * 6372795);                   //finally we have the result of distance between two points 
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
    delay_milli(500);
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
void delay_milli(int n){
int i,j;
for(i=0;i<n;i++){
    for(j=0;j<3180;j++)
{}
}
}
void delay_micro(int n){
int i,j;
for(i=0;i<n;i++){
for(j=0;j<3;j++)
{}
}
}
    void LCD_COMM(unsigned char command)
{
GPIO_PORTA_DATA_R = 0x00; //RS =0(command)
GPIO_PORTB_DATA_R =command; //data wich contain the command no.
GPIO_PORTA_DATA_R =0x80; //E=1 for high
delay_micro(0);
GPIO_PORTA_DATA_R =0x00; //E=0 to law
if(command <4) delay_milli(2); else delay_micro(37);  //from data sheet
    }
    
void LCD_DATA(unsigned char data) { //the data function of the lcd
    GPIO_PORTA_DATA_R = 0x20; //RS=1(data)
    GPIO_PORTB_DATA_R = data; //data wich appear on the screen
    GPIO_PORTA_DATA_R |= 0x80; // E high
    GPIO_PORTA_DATA_R = 0x00; // E law
    delay_micro(0);
}
// A7 for Enable , A5 for RS , A6 for RW(=0)
// portB for D0toD7
    void LCD_INITIALIZATION(void){
SYSCTL_RCGCGPIO_R |= 0x00; 
SYSCTL_RCGCGPIO_R |= 0x03;
GPIO_PORTA_DIR_R |=0xE0; //last 3 pins outputs
GPIO_PORTA_DEN_R |=0xE0; //last 3 pins digital
GPIO_PORTB_DIR_R |=0xFF; // all pins output
GPIO_PORTB_DEN_R |=0xFF; //all pins digital
LCD_COMM(0x38); //using 8_bit
LCD_COMM(0x06); //shifting from right to lift automatically
LCD_COMM(0x0F); //wake up display
LCD_COMM(0x01); //clear the screen
}   
// to print the number of distance on LCD screen , get them in three digits 
    int hund(int distance) {
        int xx = distance / 100;  
        int y = distance / 10; 
        int z = xx * 10; 
        int q = y - z;   
        int v = y * 10;  
        int n = distance - v; 
        return (xx + 48);      // returning the first digit (the hundreds digit ) ....... 48 is the offest of number to print on LCD screen 
    }
    int tens(int distance){
        int xx =distance/100; 
        int y =distance/10;  
        int z = xx *10;  
        int q =y-z;   
        int v=y*10; 
        int n= distance - v; 
        return (q+48); // returning the second digit (the tens digit ) ....... 48 is the offest of number to print on LCD screen 
}
int ones(int distance){
    int xx =distance/100;  
    int y =distance/10;
    int z = xx *10;
    int q =y-z;   
    int v=y*10;  
    int n= distance - v;
    return (n+48);   // returning the third digit (the ones digit ) ....... 48 is the offest of number to print on LCD screen 
}
void LCD_PRINT_Distance(int hundreds, int tens, int ones) {        //Taking number of three digits reprsenting distance 
    LCD_COMM(0x01);
    LCD_COMM(0x80);
    delay_milli(500);
    LCD_DATA('D');
    delay_milli(1);
    LCD_DATA('i');
    delay_milli(1);
    LCD_DATA('s');
    delay_milli(1);
    LCD_DATA('t');
    delay_milli(1);
    LCD_DATA('a');
    delay_milli(1);
    LCD_DATA('n');
    delay_milli(1);
    LCD_DATA('c');
    delay_milli(1);
    LCD_DATA('e');
    delay_milli(1);
    LCD_DATA(' ');
    delay_milli(1);
    LCD_DATA(49+9);
    delay_milli(1);
    LCD_DATA(' ');
    delay_milli(1);
    LCD_DATA(hundreds);
    delay_milli(1);
    LCD_DATA(tens);
     delay_milli(1);
     LCD_DATA(ones);
     delay_milli(1);
     LCD_DATA('m');
     delay_milli(1);
     delay_milli(500);
}


//function to turn on the green LED when the distance exceeds 100 meters

void led_100(int dis) {

    if (dis >= 100) {
        GPIO_PORTF_DATA_R = GREEN;


    }
}








