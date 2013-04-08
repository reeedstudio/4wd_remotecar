/*
  moto_4wd.cpp
  2012 Copyright (c) Seeed Technology Inc.  All right reserved.
 
  Author: Loovee
  
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
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Arduino.h>
#include <pwm_lvc.h>
#include "moto_4wd.h"


void motor_4wd::init()
{
    motorSpeed1     = 0;
    motorSpeed2     = 0;
    
    motorDir1       = 0;
    motorDir2       = 0;
    
    motorState1     = STOP;
    motorState2     = STOP;
    
    // pwm set
    PWM.init();
    PWM.setPwm(9, 0, FREQPWM);
    PWM.setPwm(10, 0, FREQPWM);

    pinMode(PINCS, OUTPUT);
    pinMode(PINM1F, OUTPUT);
    pinMode(PINM1R, OUTPUT);
    pinMode(PINM2F, OUTPUT);
    pinMode(PINM2R, OUTPUT);
    
    digitalWrite(PINCS, LOW);
    digitalWrite(PINM1F, LOW);
    digitalWrite(PINM1R, LOW);
    digitalWrite(PINM2F, LOW);
    digitalWrite(PINM2R, LOW);
   
}

void motor_4wd::setRun1()
{
    PWM.setPwm(9, motorSpeed1, FREQPWM);
    ALLMOSON;
    if(motorDir1 == DIRF)
    {
        digitalWrite(PINM1F, HIGH);
        digitalWrite(PINM1R, LOW);
    }
    else
    {
        digitalWrite(PINM1F, LOW);
        digitalWrite(PINM1R, HIGH);  
    }
    motorState1 = RUN;
}

void motor_4wd::setRun2()
{
    PWM.setPwm(10, motorSpeed2, FREQPWM);
    ALLMOSON;
    if(motorDir2 == DIRF)
    {
        digitalWrite(PINM2F, HIGH);
        digitalWrite(PINM2R, LOW);
    }
    else
    {
        digitalWrite(PINM2F, LOW);
        digitalWrite(PINM2R, HIGH);
    }
    motorState2 = RUN;
}
    
void motor_4wd::setStop1()
{
    PWM.setPwm(9, 0, FREQPWM);
    digitalWrite(PINM1F, LOW);
    digitalWrite(PINM1R, LOW);
    motorState1 = STOP;
}

void motor_4wd::setStop2()
{
    PWM.setPwm(10, 0, FREQPWM);
    digitalWrite(PINM2F, LOW);
    digitalWrite(PINM2R, LOW);
    motorState2 = STOP;
}
    
void motor_4wd::setSpeed1(int ispeed)       // pwm, 0-100
{
    motorSpeed1 = ispeed < 100 ? ispeed : 100;
}

void motor_4wd::setSpeed2(int ispeed)       // pwm, 0-100
{
    motorSpeed2 = ispeed < 100 ? ispeed : 100;
}

void motor_4wd::setDir1(unsigned char dir)
{
    motorDir1 = dir;
}

void motor_4wd::setSpeedDir(int ispeed, unsigned char dir)
{
    
    setSpeedDir1(ispeed, dir);
    setSpeedDir2(ispeed, dir);

}

void motor_4wd::setSpeedDir1(int ispeed, unsigned char dir)
{
    setSpeed1(ispeed); 
    setDir1(dir);
    setRun1();
    
}

void motor_4wd::setSpeedDir2(int ispeed, unsigned char dir)
{
    setSpeed2(ispeed);
    setDir2(dir);
    setRun2();
}
    
    

void motor_4wd::setDir2(unsigned char dir)
{
    motorDir2 = dir;
}
    
int motor_4wd::getMotor1Speed()
{

}

int motor_4wd::getMotor2Speed()
{

}
    
    

motor_4wd MOTOR;

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/