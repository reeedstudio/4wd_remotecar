/*
  remote_lvc.ino
  2013 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:Loovee
  2013-3-18
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-
  1301  USA
*/
#include <RFBeeSendRev.h>
#include <EEPROM.h>

#define MAXSPEED        80
#define MAXSPEEDN       -80

#define VAL1MIN      -263
#define VAL1MAX       263
#define VAL2MIN      -257
#define VAL2MAX       257

#define START1      0x53
#define START2      0x01
#define END1        0x2f
#define END2        0x45
#define DTALEN      6

#define DIRFF       0
#define DEIRR       1

#define __DEBUG		1

#if __DEBUG
#define __print(X)		Serial.print(X)
#define __println(X)	Serial.println(X)
#else
#define __print(X)
#define __println(X)
#endif

int speedLeft  = 0;
int speedRight = 0;

unsigned char dtaSend[6] = {START1, START2, 0, 0, END1, END2};

int val1    = 0;
int valbuf1 = 0;
int val2    = 0;
int valbuf2 = 0;

int comVal1 = 0;
int comVal2 = 0;

/*********************************************************************************************************
** Function name:           getAnalog
** Descriptions:            read analog value
*********************************************************************************************************/
int getAnalog(int pin)
{
    unsigned int sum = 0;
    for(int i = 0; i<32; i++)
    {
        sum += analogRead(pin);
    }
    sum = ((sum >> 5) & 0x03ff);        // diff
    
    return sum;
}

/*********************************************************************************************************
** Function name:           cmpLvc
** Descriptions:            compare
*********************************************************************************************************/
bool cmpLvc(int a, int b)
{
    int tmp = a - b;
	return abs(tmp) > 1 ? 1 : 0;
}

/*********************************************************************************************************
** Function name:           sendToRfbee
** Descriptions:            send data to rfbee
*********************************************************************************************************/
void sendToRfbee()
{
    dtaSend[2] = speedLeft;
    dtaSend[3] = speedRight;
    RFBEE.sendDta(6, dtaSend);
}

/*********************************************************************************************************
** Function name:           makeDirSpeed
** Descriptions:            make direction and speed
*********************************************************************************************************/
void makeDirSpeed(int x, int y)
{

    if(y == 0)												// go ahead or go back
    {

		speedLeft  = abs(x);
        speedRight = speedLeft;
		if(x >= 0)
		{
			__println("forward");
		}
		else 
		{
			speedLeft = 0x80 | speedLeft;
			speedRight = speedLeft;
			__println("goback");
		}
		
    }
	else if(x == 0)
	{
		speedLeft  = map(abs(y), 0, 100, 0, 50);
		speedRight = speedLeft;
			
		if(y > 0)		// right-rev
		{
			speedRight  = speedRight | 0x80;
		}
		else			// left-rev
		{
			speedLeft  = speedLeft | 0x80;
		}
	}
    else if(x >= 0 && y<= 0 && abs(x) >= abs(y))			// ahead-left
    {
        __println("forward-left");
        speedLeft   = x+y;
        speedRight  = x;
    }
	else if(x >= 0 && y <= 0 && abs(x) < abs(y))			// left-reverse
	{
		__println("left-rev");
		speedLeft  = 100 - min(abs(x), abs(y));
		
		//speedLeft  = map(speedLeft, 0, MAXSPEED, 0, MAXSPEEDTURN);
		speedRight = speedLeft;
		speedLeft  = speedLeft | 0x80;
	}
    else if(x >=0 && y >= 0 && abs(x) >= abs(y))			// ahead-right
    {
        __println("forward-right");
        speedLeft   = x;
        speedRight  = x-y;

    }
	else if(x >= 0 && y >= 0 && abs(x) < abs(y))			// right-reverse
	{
		__println("right-rev");
		speedLeft   = 100 - min(abs(x), abs(y));
		//speedLeft  = map(speedLeft, 0, MAXSPEED, 0, MAXSPEEDTURN);
		speedRight  = speedLeft;
		speedRight  = speedRight | 0x80;
		
	}
	else if(x <= 0 && y <= 0 && abs(x) > abs(y))			// back-left
	{
		speedRight = 0x80 + abs(x);
		speedLeft  = 0x80 + abs(x) - abs(y);
		__println("back-left");
	}
	else if(x <= 0 && y >=0 && abs(x) > abs(y))
	{
		speedLeft  = 0x80 + abs(x);
		speedRight = 0x80 + abs(x) - abs(y);
		__println("back-right");
	}
	else return ;
	
	__print("right speed: ");
	__println(speedRight);
    __print("left  speed: ");
	__println(speedLeft);
	sendToRfbee();

}

/*********************************************************************************************************
** Function name:           setup
** Descriptions:            setup
*********************************************************************************************************/
void setup()
{
    pinMode(A3, OUTPUT);   		// turn on power regulation MOSFET
    digitalWrite(A3, LOW); 		//
    RFBEE.init();
    comVal1 = getAnalog(A4);
    comVal2 = getAnalog(A5);
    __println("init over");
}


/*********************************************************************************************************
** Function name:           loop
** Descriptions:            loop
*********************************************************************************************************/
void loop()
{

    val1 = getAnalog(A4) - comVal1;
    val2 = getAnalog(A5) - comVal2;
    
    val1 = constrain(val1, VAL1MIN, VAL1MAX);
    val2 = constrain(val2, VAL2MIN, VAL2MAX);
    
    val1 = map(val1, VAL1MIN, VAL1MAX, MAXSPEED, MAXSPEEDN);
    val2 = map(val2, VAL2MIN, VAL2MAX, MAXSPEED, MAXSPEEDN);

    //if((cmpLvc(val1, valbuf1) || cmpLvc(val2, valbuf2)))
	if(val1 != valbuf1 || val2 != valbuf2)
    {
        __print("A4: ");__println(val1);
        __print("A5: ");__println(val2);
        makeDirSpeed(val1, val2);
        __println();
    }
    
    valbuf1 = val1;
    valbuf2 = val2;
    
    delay(10);
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/