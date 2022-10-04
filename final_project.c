#include <stdio.h>
#include <math.h>
#include "stdint.h"
#include "stdbool.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include"math.h"
#include "inc/tm4c123gh6pm.h"
#define RED 0x02
#define BLUE 0x04
#define GREEN 0x08


void UART_Init(void)
    {
     SYSCTL_RCGCUART_R |= 0X0020 ;                            //enable clock of UART5
     SYSCTL_RCGCGPIO_R |= 0x0010;                             //enable PORTE
     while((SYSCTL_PRGPIO_R&0x0010)==0){};                    //delay



      /* UART5 Setting */
     UART5_CTL_R &= ~0x0001 ;                                 //disable UART 
     UART5_IBRD_R = 0x68 ;                                    //IBRD = system clock / (16 * clock div) 
     UART5_FBRD_R = 0x0B ;                                    // FBDR = round ( 0.16667 * 64)+0.5
     UART5_LCRH_R = 0x0070;                                   // set 8 bits for length , FIFO enable , one stop , no parity 
     UART5_CTL_R = 0x0301 ;                                   //enable UARTEN , TXE and RXE */



     /* UART0 TX0 and RX0 use PE5 and PE4*/
     GPIO_PORTE_DEN_R |= 0x30;                                                               //use pin 4,5 of PORTE as a digital
     GPIO_PORTE_AFSEL_R |= 0x30 ;                                                            //use pin 4,5 of PORTE as an Alternative function
     GPIO_PORTE_PCTL_R =(GPIO_PORTE_PCTL_R & 0xFF00FFFF)+0x00110000 ;                        //configure PE4 and PE5 for UART
     GPIO_PORTE_AMSEL_R &= ~0x30;                                                            //no analog for PE4 , PE5 

}

void init(){
SYSCTL_RCGCGPIO_R |=0x20 ;
while((SYSCTL_PRGPIO_R & 0x20)==0){};

GPIO_PORTF_LOCK_R=0x4C4F434B;
GPIO_PORTF_CR_R=0x1F;
GPIO_PORTF_AFSEL_R=0;
GPIO_PORTF_PCTL_R=0;
GPIO_PORTF_AMSEL_R=0;
GPIO_PORTF_DIR_R=0x0E;
GPIO_PORTF_DEN_R=0x1F;
GPIO_PORTF_PUR_R=0x11;
//GPIO_PORTF_DATA_R&=0x0E;
}
double GPS_cordinates(double *lat_do, double *long_do)
{


    char frst_ch = '0', dumm_var = '0';   //initi.


     //variables we will need to store the latit. and long as a strings
    char lat[10] = "0", longt[11] = "0";
    // getting the first char from gps
 while(1){
    while ((UART5_FR_R & 0x10) != 0)
    {}
    frst_ch = UART5_DR_R&0xFF;

    // check if it is the beging of nmea or not
    if(frst_ch == '$')
    {
        // check if the GPS is sending &GPGLL
        char sec_ch = '0', thr_ch = '0', frth_ch = '0', ffth_ch = '0', sx_ch = '0';
        while ((UART5_FR_R & 0x10) != 0)
        {}
        sec_ch = UART5_DR_R&0xFF;

        while ((UART5_FR_R & 0x10) != 0)
        {}
        thr_ch = UART5_DR_R&0xFF;

        while ((UART5_FR_R & 0x10) != 0)
        {}
        frth_ch = UART5_DR_R&0xFF;

        while ((UART5_FR_R & 0x10) != 0)
        {}
        ffth_ch = UART5_DR_R&0xFF;

        while ((UART5_FR_R & 0x10) != 0)
        {}
        sx_ch = UART5_DR_R&0xFF;
        // condition to check if we in the $GPGLL
        if(sec_ch == 'G' && thr_ch == 'P' && frth_ch == 'G' && ffth_ch == 'L' && sx_ch == 'L')
        {

            // know we know that we are in the line of $GPGLL



            // we want to extract latitude and longitude.

            int comm = 0;                       //counter for ','
            bool end = true ;               // variable to check if we've ended the nmea cordinates
            // itrative loop to get the values that we need from the cordinats
            while(end == true)
                {
                    while ((UART5_FR_R & 0x10) != 0)
                    {}
                    dumm_var = UART5_DR_R&0xFF;
                    if(comm == 1)       //getting latitude
                    {
                        if(dumm_var != ',') //condition to prevent ',' because at the end of latitude ',' will enter the outer condition
                        {
                            strncat(lat, &dumm_var, 1);
                            *lat_do = atof(lat);
                            int  degrees1= *lat_do/100;
                            double minutes1=*lat_do-(double)(degrees1*100);
                            double seconds1=minutes1/60.00;
                            *lat_do=degrees1+seconds1;
                        }
                    }
                    if(comm == 3)           //getting longitude
                    {
                        if(dumm_var != ',')  //condition to prevent ',' because at the end of longitude ',' will enter the outer condition
                        {
                            strncat(longt, &dumm_var, 1);
                            *long_do =atof( longt);
                            int  degrees2=*long_do/100;
                            double minutes2=*long_do-(double)(degrees2*100);
                            double seconds2=minutes2/60.00;
                            *long_do=degrees2+seconds2;
                        }
                    }
                    // we will take actions depending on dumm_var
                    if(dumm_var == ',')
                    {
                        comm++ ;
                    }
                    if(dumm_var == '*')
                    {
                        end = false ;
                        goto main;
                    }
            }

        }
    }}
 main:
    return 0;
}
        
double radians(double n){
      return n*3.141592654/180;
}
double GPS_distance_between (double lat1,double long1,double lat2,double long2) {
  double delta = radians(long1-long2);
  double sdlong = sin(delta);
  double cdlong = cos(delta);
  lat1 = radians(lat1);
  lat2 = radians(lat2);
  double slat1 = sin(lat1);
  double clat1 = cos(lat1);
  double slat2 = sin(lat2);
  double clat2 = cos(lat2);
  delta = (clat1 * slat2) - (slat1 * clat2 * cdlong);
  delta = pow(delta,2);
  delta += pow(clat2 * sdlong,2);
  delta = sqrt(delta);
  double denom = (slat1 * slat2) + (clat1 * clat2 * cdlong);
  delta = atan2(delta, denom);
  return ( delta * 6372795);
}

int hund(double distance){
    int xx;
    int y;
    int z;
    int q;
    int v;
    int n;

    xx =(int) (distance/100);  //hundreds
    y =(int) (distance/10);
    z = (int) xx *10;
    q = (int) (y-z);   //tens
    v=(int) (y*10);  //450
    n= (int) (distance - v);
    return (xx+48);
}



int tens(double distance){
    int xx;
    int y;
    int z;
    int q;
    int v;
    int n;

    xx =(int) (distance/100);  //hundreds
    y =(int) (distance/10);
    z = (int) xx *10;
    q = (int) (y-z);   //tens
    v=(int) (y*10);  //450
    n= (int) (distance - v);
    return (q+48);
}



int ones(double distance){
    int xx;
    int y;
    int z;
    int q;
    int v;
    int n;

    xx =(int) (distance/100);  //hundreds
    y =(int) (distance/10);
    z = (int) xx *10;
    q = (int) (y-z);   //tens
    v=(int) (y*10);  //450
    n= (int) (distance - v);
    return (n+48);

}


void LCD_COMM(unsigned char command)
{
GPIO_PORTA_DATA_R = 0x00; //RS =0, E=0, RW=0
GPIO_PORTB_DATA_R =command;
GPIO_PORTA_DATA_R =0x80; //E=1 to secure command
delay_micro(0);
GPIO_PORTA_DATA_R =0x00;
if(command <4) delay_milli(2); else delay_micro(37);
    }
   void LCD_INITIALIZATION(void){

SYSCTL_RCGCGPIO_R |= 0x03;  //ACTIVATE CLOCK OF PORT A, B 
GPIO_PORTA_DIR_R |=0xE0;    //PORTA controls RS,E and R/W
GPIO_PORTA_DEN_R |=0xE0;
GPIO_PORTB_DIR_R |=0xFF;    //PORTB D0-D7
GPIO_PORTB_DEN_R |=0xFF;    //PORTB D0-D7
LCD_COMM(0x38);             //8-bits,2 display lines, 5x7 font
LCD_COMM(0x06);             //increments automatically
LCD_COMM(0x0F);             //Turn on display
LCD_COMM(0x01);             //clear display
}
void LCD_DATA(unsigned char data) { GPIO_PORTA_DATA_R =0x20; //RS=1, E=0,RW=0
    GPIO_PORTB_DATA_R =data;
    GPIO_PORTA_DATA_R |= 0x80;
    GPIO_PORTA_DATA_R =0x00;
    delay_micro(0);
    }
    // For Delay

void delay_milli(int n){
int i,j;
for(i=0;i<n;i++){
    for(j=0;j<3180;j++)
{}
}
}
   
//delay milli

void delay_micro(int n){
int i,j;
for(i=0;i<n;i++)
for(j=0;j<3;j++)
{}
}

void LCD_PRINT (double distance){
    int h;
    int t;
    int o;
    h = hund(distance);
    t = tens(distance);
    o = ones(distance);

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
    LCD_DATA(h);
    delay_milli(1);
    LCD_DATA(t);
     delay_milli(1);
     LCD_DATA(o);
     delay_milli(1);
     LCD_DATA('m');
     delay_milli(1);

     delay_milli(500);


}
void led_100(double dis) {  //function to turn on the green LED when the distance exceeds 100 meters
    if (dis >= 100) {
        GPIO_PORTF_DATA_R = GREEN;
    }
}

void led_Start_finish(int x) {  //function to turn on the blue LED when starting calculate distance and turn off at the End point
    if (x == 1) {
        GPIO_PORTF_DATA_R = BLUE;
    }
    else{
        GPIO_PORTF_DATA_R = 0x00;
    }
}

void main(){

     init();
     LCD_INITIALIZATION();
     UART_Init();
     double lg2 ;
     double lt2 ;
     double lt1 ;
     double lg1 ;
     double a;
     double b;

    double totalDistance = 0.0;

     //switch of pin 0 to start calculate distance and Recieve coordinates
     exit:
     while((GPIO_PORTF_DATA_R&0x11)!=0x01){}
    //  double totalDistance = 0.0;
    
     //switch of pin 0 to start calculate distance and Recieve coordinates
     exit:
     while((GPIO_PORTF_DATA_R&0x11)!=0x01){}
    //  double totalDistance = 0.0;

         led_Start_finish(1);
         GPS_cordinates(&a,&b);
     lg2 = b; // update data from gps
     lt2 = a;   // update data from gps
     lt1 = a;   // update data from gps
     lg1 = b; // update data from gps
     
     //while
     while (1){
     double distance = GPS_distance_between(lt1,lg1,lt2,lg2) ;
     totalDistance=totalDistance+distance;
     LCD_PRINT (totalDistance);
     lg2=lg1;
     lt2=lt1;
     // while((GPIO_PORTF_DATA_R&0x11)!=0x01){}
     delay_milli(5000);
     GPS_cordinates(&a,&b);
     lg1 = b; // update data from gps
     lt1 = a;   // update data from gps
   
    if ((GPIO_PORTF_DATA_R&0x11)==0x10) { // switch of pin 4 to end calculation of the total distance taken
         led_Start_finish(0);
         led_100(totalDistance);
         delay_milli(5000);
         goto exit;
     }
}
}
