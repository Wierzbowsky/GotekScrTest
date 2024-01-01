//  Auther: Derek Cooper
//  Based on the work of and credit given to:
//  Fred.Chu
//  Detlef Giessmann Germany
//
//  Date:20 September 2020
//
//  Applicable Module:
//                     Gotek 3-digit LED display driver
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
//Modified record:
//Autor:        Detlef Giessmann Germany
//Mail:		derek_cooper@hotmail.com
//Comment:	Library to drive LEDC68 Gotek 3 digit LED display
//IDE:          Arduino-1.8.12
//
// 03/02/21 Changed the value for the digit 6 from 0x7c to 0x7d, ie added a top bar in the display
//          to distinguish it from a letter b. Credit to solarmon 
//
// Modified record:
// Autor:	Alex Wierzbowsky
// Mail:	wierzbowsky@rbsc.su
// Comment:	Added several functions to display characters and special characters with or without zeroes
//		Added full alphabet and special characters
// IDE:  	Arduino-1.8.19
//
/***************************************************************/
//

#include "TM1651.h"
#include <Arduino.h>


// 8-segment LED display assignment
//
//    a
//  f   b
//    g 
//  e   c
//    d
//
// Data byte: 0gfedcba

// Character table for 0-F
//
static int8_t NumTab[] = 
{ 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f,	// 0 - 9
  0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x00			// A-F + space
};

// Character table for A-Z
//
static uint8_t CharTab[] = 
{
  B01110111, B01111100, B00111001, B01011110, B01111001, B01110001, B00111101, B01110100, B00110000, B00011110,
  B01110101, B00111000, B00010101, B00110111, B00111111, B01110011, B01100111, B00110001, B01101101, B01111000,
  B00111110, B00101110, B00101010, B01110110, B01101110, B01001011
};


//******************************************
//
TM1651::TM1651(uint8_t Clk, uint8_t Data)
{
  Clkpin = Clk;
  Datapin = Data;
  pinMode(Clkpin,OUTPUT);
  pinMode(Datapin,OUTPUT);
}


//******************************************
// Write byte to chip
//
int TM1651::writeByte(int8_t wr_data)
{
  uint8_t i,count1;       
  for(i=0;i<8;i++)        //sent 8bit data
  { 
    digitalWrite(Clkpin,LOW);
    if(wr_data & 0x01)digitalWrite(Datapin,HIGH);//LSB first
    else digitalWrite(Datapin,LOW);
    wr_data >>= 1;
    digitalWrite(Clkpin,HIGH);
  
  }
  digitalWrite(Clkpin,LOW); //wait for the ACK
  digitalWrite(Datapin,HIGH);
  digitalWrite(Clkpin,HIGH);
  pinMode(Datapin,INPUT);
  
  bitDelay(); 
  uint8_t ack = digitalRead(Datapin);
  if (ack == 0)
  {  
     pinMode(Datapin,OUTPUT);
     digitalWrite(Datapin,LOW);
  }
  bitDelay();
  pinMode(Datapin,OUTPUT);
  bitDelay();
  
  return ack;
}

//******************************************
//send start signal to TM1651
//
void TM1651::start(void)
{
  digitalWrite(Clkpin,HIGH);//send start signal to TM1637
  digitalWrite(Datapin,HIGH);
  digitalWrite(Datapin,LOW);
  digitalWrite(Clkpin,LOW);
} 

//******************************************
//End signal
//
void TM1651::stop(void)
{
  digitalWrite(Clkpin,LOW);
  digitalWrite(Datapin,LOW);
  digitalWrite(Clkpin,HIGH);
  digitalWrite(Datapin,HIGH);
}

//******************************************
// Display hexadecimal number 0-F at selected position
//
void TM1651::displayNum(uint8_t dig, uint8_t number)
{
  start();          //start signal sent to TM1651 from MCU
  writeByte(ADDR_FIXED);//
  stop();           //
  start();          //
  writeByte(STARTADDR + dig);// digit pos 0-2
  writeByte(NumTab[number]);//
  stop();            //
  start();          //
  writeByte(Cmd_DispCtrl);// 88+0 to 7 brightness, 88=display on
  stop();           //
}


//******************************************
// Display decimal number 0-999 with prepended zeroes
//
void TM1651::displayInteger(uint16_t number)
{
 uint8_t i;   

 if(number > 999)
 {
  displaySpChar(0, Minus);
  displaySpChar(1, Minus);
  displaySpChar(2, Minus);
 }
 else
 {
  start();          //start signal sent to TM1651 from MCU
  writeByte(ADDR_AUTO);// auto increment the address
  stop(); 
  start();
  writeByte(STARTADDR);// start at 0
   writeByte(NumTab[(number/100) % 10]);
   writeByte(NumTab[(number/10) % 10]);
   writeByte(NumTab[number % 10]);
  stop();
  start();
  writeByte(Cmd_DispCtrl);// 88+0 to 7 brightness, 88=display on
  stop();
 }
}

//******************************************
// Display dot (doesn't work on all displays)
//
void TM1651::displayDP(uint8_t dp)
{
  uint8_t SegData;

  //if (dp == 1) SegData = 0x08;
  //else SegData = 0x00;
  start();          //start signal sent to TM1651 from MCU
  writeByte(ADDR_FIXED);
  stop();  
  start();
  writeByte(STARTADDR + 3);// digit pos 3 controls decimal point
  //writeByte(SegData);
  writeByte(dp);
  stop();      
  start();  
  writeByte(Cmd_DispCtrl);// 88+0 to 7 brightness, 88=display on
  stop();  
}

//******************************************
// Clear display (no numbers lit)
//
void TM1651::displayClear(void)
{
 displayDP(0);
 displayNum(0,16);
 displayNum(1,16);
 displayNum(2,16);
}

//******************************************
// Show 3 zeroes on display
//
void TM1651::displayZero(void)
{
 displayDP(0);
 displayInteger(0);
}

//******************************************
// Set brightness
//
void TM1651::displaySet(uint8_t brightness)
{
  Cmd_DispCtrl = 0x88 + brightness;	//Set the brightness and turn on
}

//******************************************
// Switch off display
//
void TM1651::displayOff()
{
  Cmd_DispCtrl = 0x80;
  start();
  writeByte(Cmd_DispCtrl);// 80=display off
  stop();
}

//******************************************
// Init display
//
void TM1651::displayOn()
{
 displayClear();
 displayDP(0);
 displaySet(BRIGHT_TYPICAL);
}

//******************************************
// Small delay
//
void TM1651::bitDelay(void)
{
        delayMicroseconds(50);
}

//******************************************
// Display character A-Z in a defined position (uppercase!)
//
void TM1651::displayChar(uint8_t dig, uint8_t letter)
{
  start();          //start signal sent to TM1651 from MCU
  writeByte(ADDR_FIXED);//
  stop();           //
  start();          //
  writeByte(STARTADDR + dig);// digit pos 0-2
  writeByte(CharTab[letter - 0x41]);//
  stop();            //
  start();          //
  writeByte(Cmd_DispCtrl);// 88+0 to 7 brightness, 88=display on
  stop();           //
}

//******************************************
// Display special character (see definitions)
//
void TM1651::displaySpChar(uint8_t dig, uint8_t letter)
{
  start();          //start signal sent to TM1651 from MCU
  writeByte(ADDR_FIXED);//
  stop();           //
  start();          //
  writeByte(STARTADDR + dig);// digit pos 0-2
  writeByte(letter);//
  stop();            //
  start();          //
  writeByte(Cmd_DispCtrl);// 88+0 to 7 brightness, 88=display on
  stop();           //
}

//******************************************
// Display decimal number 0-999 with prepended zeroes
//
void TM1651::displayDec(uint16_t num)
{
  displayNum(0, (num/10/10) % 10);
  displayNum(1, (num/10) % 10);
  displayNum(2, num % 10);
}

//******************************************
// Display decimal number 0-999 without prepended zeroes
//
void TM1651::displayDecNZ(uint16_t num)
{
  if(num > 99) displayNum(0, (num/10/10) % 10);
  if(num > 9) displayNum(1, (num/10) % 10);
  displayNum(2, num % 10);
}

//******************************************
// Display hex number 0-FFF (4095)
//
void TM1651::displayHex(uint16_t num)
{
 if(num > 0xFFF)
 {
  displaySpChar(0, Minus);
  displaySpChar(1, Minus);
  displaySpChar(2, Minus);
 }
 else
 { 
  displayNum(0, (num/16/16) % 16);
  displayNum(1, (num/16) % 16);
  displayNum(2, num % 16);
 }
}

void TM1651::displayHexNZ(uint16_t num)
{
 if(num > 0xFFF)
 {
  displaySpChar(0, Minus);
  displaySpChar(1, Minus);
  displaySpChar(2, Minus);
 }
 else
 { 
  if(num > 0xFF) displayNum(0, (num/16/16) % 16);
  if(num > 0x0F) displayNum(1, (num/16) % 16);
  displayNum(2, num % 16);
 }
}
