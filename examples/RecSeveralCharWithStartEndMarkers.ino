/*
  This program is to receive a line of characters from Serial1 and use it to unpdate the
  RTC clock.

  Format of the serial data is:
    <2020-07-11-09-24-19-1594459459->
     1    2  3  4  5  6  7

     Where:
       1 = Year
       2 = Month
       3 = Date
       4 = Hour
       5 = Min
       6 = Sec
       7 = UTC Epoch - an instant of time or a date selected as a point of reference

       Epoch is:
         - an instant of time or a date selected as a point of reference.
         - an event or a time that begins a new period.

  If you are not from UTC, convert it to your local time by adding/subtracting
  the number of seconds relative to UTC in your area as shown below. This can
  be done in the Print() funciton.
    long GMT - 11 = -39600L;
    long GMT - 10 = -36000L;
    long GMT - 9 = -32400L;
    long GMT - 8 = -28800L;
    long GMT - 7 = -25200L;
    long GMT - 6 = -21600L;
    long GMT - 5 = -18000L;
    long GMT - 4 = -14400L;
    long GMT - 3 = -10800L;
    long GMT - 2 = -7200L;
    long GMT - 1 = -3600L;
    long GMT - 0 = 0;       //-- > Greenwich Mean Time / Universal Coordinated Time
    long GMT + 1 = 3600L;
    long GMT + 2 = 7200L;
    long GMT + 3 = 10800L;
    long GMT + 4 = 14400L;
    long GMT + 5 = 18000L;
    long GMT + 6 = 21600L;
    long GMT + 7 = 25200L;
    long GMT + 8 = 28800L;
    long GMT + 9 = 32400L;
    long GMT + 10F = 36000L; //-- > AEST - Australia Eastern Standard Time
    long GMT + 11 = 39600L;
    long GMT + 12 = 43200L;
*/


#include "RTClib.h"
RTC_Millis rtc;
RTC_Millis datertc;
#include <LiquidCrystal.h>
#include <arduino_secrets.h>

long unsigned gv_Timezone = 36000L;    // Set the timezone here
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
long unsigned gv_Millis;
long unsigned gv_PreviousMillis1000;
long unsigned gv_ElapseTime1000;
long unsigned del1000 = 1000;
long unsigned gv_PreviousMillisUpdate;
long unsigned gv_ElapseTimeUpdate;
long unsigned delUpdte = 5000;         // Default = 600000 (10min), Test = 5000 (5 sec)
const byte numChars = 32;
char ReceivedChars[numChars];
long unsigned Epoch;
long unsigned EpochToday;
long unsigned AESTEpoch;
long unsigned PreviousEpoch = 0;
long unsigned EpochDiference;
char* CharSerialData;
String StringCharSerialData;
int NewData;
long unsigned gv_num;
int gv_year;
int gv_mon;
int gv_date;
int gv_hour;
int gv_min;
int gv_sec;
int seg;
String ST_Seg;
DateTime now;
DateTime datenow;
DateTime AEST;
int LocYear;
int LocMonth;
int LocDate;
int LocHour;
int LocMin;
int LocSec;
char* LocAmPm;

void setup() {
  Initialization();
  RecvData();
  ConvertRecDataToArray();
  PrintTime();
}

void loop() {
  SoftwareDelays();       
  DisplayTime();
  Alarms();           
}
