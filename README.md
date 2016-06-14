# Arduinio_IoT_RTC
Arduinio code for RTC using .wirelessthings.net ERF Radio and LLAP protocol

as part of a Nixie clock project, i integrated a wireless things ERF module https://www.wirelessthings.net/radio-comparison-chart
and a DS1703 RTC module

the code is a blend of existing opensource code and seom new LLAP commands
a--TI-------- will read time ie 12:56:12 wil replay a--TI125612
a--DA-------- will read data in most sig number first to 1st april 2016 will be a--DA20160401
a--TI125612-- will set the time
a--DA20160401 will set the date
