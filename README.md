# Arduinio_IoT_RTC
Arduinio code for RTC using .wirelessthings.net ERF Radio and LLAP protocol

as part of a Nixie clock project, i integrated a wireless things ERF module https://www.wirelessthings.net/radio-comparison-chart
and a DS1703 RTC module

the LLAP base protocl code is available here https://github.com/CisecoPlc/LLAPSerial
it also requires RTC from here https://github.com/adafruit/RTClib, or this can be install using the IDE library tools

the code is a blend of existing opensource code and some new LLAP commands

a--TI-------- will read time ie 12:56:12 wil reply a--TI125612--

a--DA-------- will read data in most significant number first to 1st april 2016 will be a--DA20160401

a--TI125612-- will set the time

a--DA20160401 will set the date

future updates may include ability to chose 24/12 hr mode, enable / disable SQ update of clock chip

The hardware is a Arduino mini pro with a wireless things ERF connected to the FDTI pins , configure for at 57600 baud remote programming , and a DS7103 module connected to the i2c bus with appropriate pull up resistors

