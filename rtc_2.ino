// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
 
#include <Wire.h> //offcial arduino library
#include <EEPROM.h>  //offcial arduino library
#include "RTClib.h"  //jeelabs librery intalled via arduino library tools 
// #include <TimerOne.h>  // not need for this part
#include <LLAPSerial.h> //https://github.com/CisecoPlc/LLAPSerial 

#define VERSION "1.0"
#define DEVICETYPE "RTCV1" // Real Time Clock V1
#define DEVICEID1 '-'  // default deviceid part 0
#define DEVICEID2 '-'  // default deviceid part 1
#define EEPROM_DEVICEID1 0  //EEPROM location for deviceid part 0
#define EEPROM_DEVICEID2 1  //EEPROM location for deviceid part 1

String msg;        // storage for incoming message
String reply;    // storage for reply

RTC_DS1307 RTC;

String zeroPad(int number) // pad out left of int with ZERO for units less than 10 , 
{
  if (number<10) 
  {
    return "0" + String(number);
    }
    else
    {
      return "" + String(number);
      }
  }
 
void setup () {
  
  Serial.begin(57600);	//using 57600 with ciseco erf to support over the air programming, 
							// though i have yet ot make this 100% reliable
							// note you will need to change ERF from default 9600 !!!
  Wire.begin();
  RTC.begin();
 
  String permittedChars = "-#@?\\*ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char deviceID[2];
  deviceID[0] = EEPROM.read(EEPROM_DEVICEID1);
  deviceID[1] = EEPROM.read(EEPROM_DEVICEID2);
  

  if (permittedChars.indexOf(deviceID[0]) == -1 || permittedChars.indexOf(deviceID[1]) == -1)
  {
    deviceID[0] = DEVICEID1;
    deviceID[1] = DEVICEID2;
  }
  delay(1000);  // only usein testing to allow time to sort out com port, may notbeneed in prod use
                // delay fnction used 190 byte, + 12 for any calls is 202 for first use

  LLAP.init(deviceID);
  LLAP.sendMessage(String("STARTED"));

  if (! RTC.isrunning()) {
      LLAP.sendMessage(String("RTCERR"));
    // following line sets the RTC to the date & time this sketch was compiled
    //RTC.adjust(DateTime(__DATE__, __TIME__));
  }
 
}
 
void loop () {
    DateTime RTCnow = RTC.now();
    if (LLAP.bMsgReceived) // got a message?
    {  
        msg = LLAP.sMessage;
        LLAP.bMsgReceived = false;
        reply = msg;
        if (msg.compareTo("HELLO----") == 0)
        {
            ;    // just echo the message back
        }
        else if (msg.compareTo("FVER-----") == 0)
        {
          reply = reply.substring(0,4) + VERSION;
        }
        else if (msg.compareTo("DEVTYPE--") == 0)
        {
          reply = DEVICETYPE;
        }
        else if (msg.compareTo("SAVE-----") == 0)
        {
            EEPROM.write(EEPROM_DEVICEID1, LLAP.deviceId[0]);    // save the device ID
            EEPROM.write(EEPROM_DEVICEID2, LLAP.deviceId[1]);    // save the device ID
        }
        else if (msg.startsWith("TI")) //Get time
		{
			if (msg.substring(2, 8) == "------")
			{
				// read time and return
				RTCnow = RTC.now();
				reply =  zeroPad(RTCnow.hour());
				reply += zeroPad(RTCnow.minute());
				reply += zeroPad(RTCnow.second());
			}
			else
			{ 
				//set time WIP untested
				int hours = msg.substring(2, 4).toInt();
				int mins = msg.substring(4, 6).toInt();
				int seconds = msg.substring(6, 8).toInt();
				if (hours > 0 && hours < 24 && mins >= 0 && mins < 60 && seconds >= 0 && seconds < 60)
				{
					//set time onRTC
				}
				else
				{
					reply = "TIERROR";
				}
			}
        }
        else if (msg.startsWith("DA")) //Get Date
        {  // neeed to add set date code here 
          RTCnow = RTC.now();
          reply = "" + RTCnow.year();
          reply += zeroPad(RTCnow.month());
          reply += zeroPad(RTCnow.day());
        }
        else    // it is an action message
        {
          reply = "ERROR";
        }
        LLAP.sendMessage(reply);
    }
    else
    {
		// any not message stuff
    }
}
