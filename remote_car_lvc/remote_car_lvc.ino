// demo of rfbee send and recv
#include <Arduino.h>
#include <EEPROM.h>
#include <RFBeeSendRev.h>
#include <RFBeeCore.h>
#include <Wire.h>

#define FRAMESTART1                 0x53                // data frame start1
#define FRAMESTART2                 0x01                // data frame start2
#define FRAMEEND1                   0x2f                // data frame end1
#define FRAMEEND2                   0x45                // data frame end2

void sendToI2C(unsigned char ilen, unsigned char *idata)
{
    Wire.beginTransmission(4);                           // transmit to device #4
    for(int i = 0; i<ilen; i++) {Wire.write(idata[i]);}  // sends one byte
    Wire.endTransmission();                              // stop transmitting
    
}

/*********************************************************************************************************
** Function name:           setup
** Descriptions:            setup
*********************************************************************************************************/
void setup(){

    pinMode(10, OUTPUT);
    RFBEE.init();
    Wire.begin();
    Serial.begin(38400);
    Serial.println("ok");
}

unsigned char rxData1[200];               // data len
unsigned char len1;                       // len
unsigned char srcAddress1;
unsigned char destAddress1;
char rssi1;
unsigned char lqi1;
int result1;

unsigned char cntGetDta = 5;

/*********************************************************************************************************
** Function name:           loop
** Descriptions:            loop
*********************************************************************************************************/
void loop()
{
    if(RFBEE.isDta())
    {
        result1 = receiveData(rxData1, &len1, &srcAddress1, &destAddress1, (unsigned char *)&rssi1 , &lqi1);
        Serial.println(len1);
        for(int i = 0; i< len1; i++)
        {
            Serial.print(rxData1[i]);Serial.print("\t");
        }
        
        Serial.println();
        
        sendToI2C(6, rxData1);
    }
}


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
