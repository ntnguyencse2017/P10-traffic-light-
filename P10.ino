#include <SPI.h>        //SPI.h must be included as DMD is written by SPI (the IDE complains otherwise)
#include <DMD.h>        //
#include <TimerOne.h>   //
#include <avr/pgmspace.h>
#include "sand_box.h"
#include "SystemFont5x7.h"
#include "Arial_black_16.h"
//Fire up the DMD library as dmd
#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1
#define SELECT_RED        4
#define SELECT_GREEN      5
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);
// SELECT_RED        4
// SELECT_GREEN      5
// PIN_DMD_nOE       9    // D9 active low Output Enable, setting this low lights all the LEDs in the selected rows. Can pwm it at very high frequency for brightness control.
// PIN_DMD_A         6    // D6
// PIN_DMD_B         7    // D7
// PIN_DMD_CLK       13   // D13_SCK  is SPI Clock if SPI is used
// PIN_DMD_SCLK      8    // D8
// PIN_DMD_R_DATA    11   // D11_MOSI is SPI Master Out if SPI is used
 unsigned long count=0;
int second=0;
int timer=0;
int t_red=60;
int animation_red=sizeof(animation)/sizeof(animation[0]);
int count_animation=0;
unsigned long time_pre_red=0;
unsigned long time_pre_yellow=0;
double _space;
enum _STATE{INIT_RED=0,RED,INIT_GREEN,GREEN,INIT_YELLOW,YELLOW,DEF};
_STATE STATE=INIT_RED;


 void draw3(){
  dmd.drawLine(22,8,29,8,GRAPHICS_NORMAL);
  dmd.drawLine(27,6,27,7,GRAPHICS_NORMAL);
  dmd.drawLine(24,5,22,5,GRAPHICS_NORMAL);
  dmd.drawLine(22,9,29,9,GRAPHICS_NORMAL);
  dmd.drawLine(27,7,27,8,GRAPHICS_NORMAL);
  dmd.drawLine(29,5,29,10,GRAPHICS_INVERSE);
  dmd.drawLine(25,7,25,9,GRAPHICS_INVERSE);
  dmd.drawLine(21,5,21,10,GRAPHICS_INVERSE);
  dmd.drawLine(28,10,22,10,GRAPHICS_INVERSE);

 };
void draw2(){
  dmd.drawLine(24,10,22,10,GRAPHICS_NORMAL);
  dmd.drawLine(25,7,25,9,GRAPHICS_NORMAL);
  dmd.drawLine(29,5,29,10,GRAPHICS_INVERSE);
  dmd.drawLine(25,5,25,10,GRAPHICS_INVERSE);
  dmd.drawLine(21,5,21,10,GRAPHICS_INVERSE);
  dmd.drawLine(28,10,26,10,GRAPHICS_INVERSE);
  dmd.drawLine(24,5,22,5,GRAPHICS_INVERSE);

 }
void draw1(){
  dmd.drawLine(25,5,25,10,GRAPHICS_NORMAL);
  dmd.drawLine(28,10,22,10,GRAPHICS_NORMAL);
  dmd.drawLine(24,5,22,5,GRAPHICS_NORMAL);
  dmd.drawLine(29,5,29,10,GRAPHICS_NORMAL);
  dmd.drawLine(25,7,25,9,GRAPHICS_NORMAL);
  dmd.drawLine(21,5,21,10,GRAPHICS_INVERSE);

   dmd.drawLine(22,8,29,8,GRAPHICS_INVERSE);
  dmd.drawLine(27,6,27,7,GRAPHICS_INVERSE);
}
void ScanDMD()
{ 
  /*--------------------------------------------------------------------------------------
  Interrupt handler for Timer1 (TimerOne) driven DMD refresh scanning, this gets
  called at the period set in Timer1.initialize();
--------------------------------------------------------------------------------------*/
  dmd.scanDisplayBySPI();
  count++;
  if(count%100==0){
    second++;
    if(second>4*t_red){
      second=0;
      count=0;
    }
    
    }
  }
void setup(void)
{
/*--------------------------------------------------------------------------------------
  setup
  Called by the Arduino architecture before the main loop begins
--------------------------------------------------------------------------------------*/

    //initialize TimerOne's interrupt/CPU usage used to scan and refresh the display
   Timer1.initialize( 5000 );           //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
   Timer1.attachInterrupt( ScanDMD );   //attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()

    //clear/init the DMD  pixels held in RAM
   dmd.clearScreen( false );   //true is normal (all pixels off), false is negative (all pixels on)
   Serial.begin(9600);
   pinMode(SELECT_RED,OUTPUT);
   pinMode(SELECT_GREEN,OUTPUT);  
}

 /*--------------------------------------------------------------------------------------
  loop
  Arduino architecture main loop
--------------------------------------------------------------------------------------*/

 void loop(void)
{
  switch(STATE){
    case INIT_RED:
    second=0;
    digitalWrite(SELECT_RED,HIGH);
    digitalWrite(SELECT_GREEN,LOW);  
    dmd.clearScreen(false);  
    for(int j=0;j<16;j++){
      for(int i=0;i<32;i++){   
        dmd.writePixel(i,j,GRAPHICS_INVERSE,RG[j*32+i]);
      }};
    STATE=RED;

    break;

    ///////////////////////////////
    //
    /////////////////////////////
    case RED:
    
    dmd.writePixel(animation[second][1],animation[second][0],GRAPHICS_INVERSE,1);
    dmd.writePixel(30-animation[second][1],animation[second][0],GRAPHICS_INVERSE,0);
    
     if(second>=2*t_red){
      STATE=INIT_GREEN;
      
     };
     break;

     ////////////////////
     //
     ///////////////////
     
     case INIT_YELLOW:
     
     
     digitalWrite(SELECT_RED,HIGH);
    digitalWrite(SELECT_GREEN,HIGH);  
    dmd.clearScreen(false);  
    for(int j=0;j<16;j++){
      for(int i=0;i<32;i++){   
        dmd.writePixel(i,j,GRAPHICS_INVERSE,test[j*32+i]);
      }};
      STATE=YELLOW;
      second=0;
     break;
     
     //////////////////////////////
     //
     /////////////////////////////
     
     case YELLOW:
     if((second>=1)&&(second<=3)){
      draw3();
     }
     if((second>3)&&(second<=5)){
      draw2();
     }
     if((second>5)&&(second<=7)){
      draw1();
     }
     if(second>8){
      STATE=INIT_RED;
     }
     break;

     ////////////////////////
     //
     //////////////////////
     
     case INIT_GREEN:
     second=0;
          digitalWrite(SELECT_RED,LOW);
    digitalWrite(SELECT_GREEN,HIGH);  
    dmd.clearScreen(false);  
    for(int j=0;j<16;j++){
      for(int i=0;i<32;i++){   
        dmd.writePixel(i,j,GRAPHICS_INVERSE,RG[j*32+i]);
      }};
     STATE=GREEN;

     break;
     case GREEN:
    dmd.writePixel(animation[second][1],animation[second][0],GRAPHICS_INVERSE,1);
    dmd.writePixel(30-animation[second][1],animation[second][0],GRAPHICS_INVERSE,0);
    if(second>=2*t_red){
      STATE=INIT_YELLOW;
     
    }
     break;
  }
}


































/*{
  switch(STATE){
    case RED:
     for(int j=0;j<16;j++){
  for(int i=0;i<32;i++){   
      dmd.writePixel(i,j,GRAPHICS_INVERSE,RG[j*32+i]);
    }};
  delay(10);
  for(int k = 0; k < sizeof(animation)/sizeof(animation[0]); k++)
  {
    dmd.writePixel(animation[k][1],animation[k][0],GRAPHICS_INVERSE,1);
    dmd.writePixel(30-animation[k][1],animation[k][0],GRAPHICS_INVERSE,0);
    delay(200);
  }
  
    case GREEN:
         for(int j=0;j<16;j++){
  for(int i=0;i<32;i++){   
      dmd.writePixel(i,j,GRAPHICS_INVERSE,RG[j*32+i]);
    }};
  delay(3000);
    case YELLOW:
    dmd.clearScreen( false );
    draw3();delay(1000);
    draw2();delay(1000);
    draw1();delay(1000);
    if(second==2*t_red-3){
      draw3();
    };
    if(second==2*t_red-2){
      draw2();
    };
    if(second==2*t_red-1){
      draw1();
    };
    if(second>=(2*t_red)){
      dmd.clearScreen( false );
      
    };
    case DEF:
    STATE=DEF;
    if(second%2==0){
      for(int j=0;j<16;j++){
        for(int i=0;i<32;i++){   
          dmd.writePixel(i,j,GRAPHICS_INVERSE,1);
        }};
    }
    else{
      for(int j=0;j<16;j++){
        for(int i=0;i<32;i++){   
          dmd.writePixel(i,j,GRAPHICS_NORMAL,1);
        }};
    break;
  }
  delay(2);
  }}
/*
   for(int j=0;j<16;j++){
  for(int i=0;i<32;i++){   
      dmd.writePixel(i,j,GRAPHICS_INVERSE,RG[j*32+i]);
    }};
  delay(500);
  for(int k = 0; k < sizeof(animation)/sizeof(animation[0]); k++)
  {
    dmd.writePixel(animation[k][1],animation[k][0],GRAPHICS_INVERSE,1);
    dmd.writePixel(30-animation[k][1],animation[k][0],GRAPHICS_INVERSE,0);
    delay(200);
  }
  delay(2000);
  *//*for(int j=0;j<16;j++){
  for(int i=0;i<32;i++){   
      dmd.writePixel(i,j,GRAPHICS_INVERSE,test[j*32+i]);
    }};
  delay(500);
  draw3();
  delay(1000);
  draw2();
  delay(1000);
  draw1();
  delay(1000);



 };*/
