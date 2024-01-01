/***********************************************************/
//Test for TM1651 when used on Gotek board LEDC68
//
//Hardware: Re-Used Gotek 3 digit LED
//Board:    Arduino Nano
//IDE:      Arduino
//Function: Testing removed display unit
//Date:     01.01.2024
//Author:   Wierzbowsky [RBSC]
/***************************************************************/

#include "TM1651.h"

#define CLK 3 //pins definitions for TM1651 and can be changed to other ports       
#define DIO 2

TM1651 Display(CLK,DIO);

void setup()
{
  Display.displayOn();
}
void loop()
{
 Display.displayClear();
 /*
 delay(2000);

 for(uint8_t cnt = 0; cnt < 15-2; cnt++)
 {
  Display.displaySpChar(0, SpCharTab[cnt]);
  Display.displaySpChar(1, SpCharTab[cnt+1]);
  Display.displaySpChar(2, SpCharTab[cnt+2]);
  delay(500);
 }
 
 delay(2000);
 Display.displayClear();

 for(char cnt = 0x41; cnt < 0x41+26-2; cnt++)
 {
  Display.displayChar(0,cnt);
  Display.displayChar(1,cnt+1);
  Display.displayChar(2,cnt+2);
  delay(500);
 } 

 delay(2000);
 Display.displayClear();

 for(char cnt = 0; cnt < 16-2; cnt++)
 {
  Display.displayNum(0,cnt);
  Display.displayNum(1,cnt+1);
  Display.displayNum(2,cnt+2);
  delay(500);
 } 
*/
 Display.displaySet(7);
 Display.displayNum(0,8);
 Display.displayNum(1,8);
 Display.displayNum(2,8);
 delay(1000);

 for (uint8_t cnt=0; cnt<6; cnt++)
 {
  for(uint8_t brt = 7; brt > 0; brt--)
  {
    Display.displaySet(brt);
    Display.displayNum(0,8);
    Display.displayNum(1,8);
    Display.displayNum(2,8);
    delay(30);
  }
  for(uint8_t brt = 1; brt < 8; brt++)
  {
    Display.displaySet(brt);
    Display.displayNum(0,8);
    Display.displayNum(1,8);
    Display.displayNum(2,8);
    delay(30);
  }
 }

 delay(1000);
 Display.displayClear();
 Display.displaySet(0);
 delay(1000);

 countdown(999);
 
 while(1);
}

void countdown(uint16_t num)
{
    for(num ; num > 0; num=num-111)
    {
     uint32_t timer = millis();

     if(num < 666)
     {
      for(uint8_t brt = 1; brt < 8; brt++)
      {
        Display.displaySet(brt);
//      Display.displayIntegerNZ(num);
        Display.displayInteger(num);
//      Display.displayNum(2, num);
        Display.displayDP(0);
        delay(30);
      }

      for(uint8_t brt = 7; brt > 0; brt--)
      {
        Display.displaySet(brt);
//      Display.displayIntegerNZ(num);
        Display.displayInteger(num);
//      Display.displayNum(2, num);
        Display.displayDP(0);
        delay(30);
      }
      if(millis() - timer < 1000)
        {
          delay(millis() - timer);
        }
    }
     else
     {
        Display.displaySet(7);
        Display.displayInteger(num);
        delay(1000);
     }
    }

    for(uint8_t del = 0; del < 5; del++)
    {
      Display.displaySet(7);
      Display.displayInteger(0);
      delay(100);
      Display.displaySet(0);
      Display.displayInteger(0);      
      delay(100);
    }

    Display.displaySet(7);
    Display.displayNum(0,8);
    Display.displayNum(1,12);
    Display.displayNum(2,14);
    delay(1500);
    Display.displayNum(0,16);
    delay(200);
    Display.displayNum(2,16);
    delay(200);
    Display.displayNum(1,16);
    delay(200);
    Display.displayOff();
}
