void Alarms() {
  long unsigned gv_AlarmOn = 21240; // 8:00AM
  long unsigned gv_AlarmDuration = 60;   // 1 hour
  long unsigned gv_AlarmStart;
  long unsigned gv_AlarmEnd;

  gv_AlarmStart = EpochToday + gv_AlarmOn;
  gv_AlarmEnd = gv_AlarmStart + gv_AlarmDuration;
//  Serial.print("gv_AlarmStart = ");
//  Serial.println(gv_AlarmStart);
//  Serial.print("gv_AlarmEnd = ");
//  Serial.println(gv_AlarmEnd);
//  Serial.print("AESTEpoch = ");
//  Serial.println(AESTEpoch);
  if ((AESTEpoch > gv_AlarmStart) && (AESTEpoch < gv_AlarmEnd)) {
    Serial.println("Alarm on");
  } else {
//    Serial.println("Alarm off");
  }
}

void Initialization() {
  Serial.begin(9600);
  Serial1.begin(9600);
  while ( Serial.available() != 0) {
  }
  while ( Serial1.available() != 0) {
  }
  delay(2000);
  lcd.begin(15, 2);  // set up the LCD's number of columns and rows:
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome:");
  lcd.setCursor(0, 1);
  lcd.print("Carlou Torres");
  RecvData();
  ConvertRecDataToArray();
}

void DisplayTime() {
  // Get Epoch every second
  if ( gv_ElapseTime1000 >= del1000 ) {
    RecvData();
    ConvertRecDataToArray();
    PrintTime();
    NewData = 1;
    gv_PreviousMillis1000 = millis();
  }
}

void RecvData() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;
  while (Serial1.available() == 0) {
  }
  while (Serial1.available() > 0) {
    rc = Serial1.read();
    if (recvInProgress == true) {
      if (rc != endMarker) {
        ReceivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      } else {
        ReceivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
      }
    } else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

void  ConvertRecDataToArray() {
  // Convert ReceivedChars to array
  seg = 0;
  CharSerialData = ReceivedChars;
  StringCharSerialData = CharSerialData;
  int StringLength = StringCharSerialData.length() - 1;
  for (int i = 0; i <= StringLength; i++) {
    if (ReceivedChars[i] == '-') {
      seg = seg + 1;
      gv_num = ST_Seg.toInt();

      switch (seg) {
        case 1:  // Year
          gv_year = gv_num;
          break;
        case 2:  // Month
          gv_mon = gv_num;
          break;
        case 3:  // Date
          gv_date = gv_num;
          break;
        case 4:  // Hour
          gv_hour = gv_num;
          break;
        case 5:  // Min
          gv_min = gv_num;
          break;
        case 6:  // Sec
          gv_sec = gv_num;
          break;
        case 7:  // Epoch
          Epoch = gv_num;
          break;
      }
      gv_num = 0;
      ST_Seg = "";
    } else {
      ST_Seg = ST_Seg + ReceivedChars[i];
    }
  }
  rtc.adjust(DateTime(gv_year, gv_mon, gv_date, gv_hour, gv_min, gv_sec));
  //  Serial.print("Incoming date/time/Epoch = ");
  //  Serial.println(ReceivedChars);
  //  Serial.print(gv_year);
  //  Serial.print("/");
  //  Serial.print(gv_mon);
  //  Serial.print("/");
  //  Serial.print(gv_date);
  //  Serial.print("  ");
  //  Serial.print(gv_hour);
  //  Serial.print(":");
  //  Serial.print(gv_min);
  //  Serial.print(":");
  //  Serial.print(gv_sec);
  //  Serial.print("   Epoch = ");
  //  Serial.print(Epoch);
  //  Serial.println(" ");
  //  Serial.println("Printing clock on LCD");
  //  lcd.clear();
  //  lcd.setCursor(0, 0);
  //  lcd.print(gv_year);
  //  lcd.print("/");
  //  lcd.print(gv_mon);
  //  lcd.print("/");
  //  lcd.print(gv_date);
  //  lcd.setCursor(0, 1);
  //  lcd.print(gv_hour);
  //  lcd.print(":");
  //  lcd.print(gv_min);
  //  lcd.print(":");
  //  lcd.print(gv_sec);
  //  Serial.println("Clock adjust ............");

  //  datertc.adjust(DateTime(gv_year, gv_mon, gv_date, 0, 0, 0));
  //  datenow = datertc.now();
  //  Serial.print("Date now = ");
  //  Serial.print(datenow.year());
  //  Serial.print("/");
  //  Serial.print(datenow.month());
  //  Serial.print("/");
  //  Serial.println(datenow.day());
  //
  //  Serial.print("Time now = ");
  //  Serial.print(datenow.hour());
  //  Serial.print("/");
  //  Serial.print(datenow.minute());
  //  Serial.print("/");
  //  Serial.println(datenow.second());
}

void SoftwareDelays() {
  gv_Millis = millis();
  gv_ElapseTime1000 = gv_Millis - gv_PreviousMillis1000;
  gv_ElapseTimeUpdate = gv_Millis - gv_PreviousMillisUpdate;
}

void PrintTime() {
  now = rtc.now();                          // rtc = is the time in UTC
  AEST = (now.unixtime() + gv_Timezone);    // AEST = is adjusted to Australiam Easter Standard Time
  LocYear    = AEST.year();
  LocMonth   = AEST.month();
  LocDate    = AEST.day();
  LocHour    = AEST.hour();
  LocMin     = AEST.minute();
  LocSec     = AEST.second();

  const uint8_t  SPRINTF_BUFFER_SIZE = 32;  // Buffer size for sprintf()
  char Loc_Date[SPRINTF_BUFFER_SIZE];       // Buffer for sprintf()/sscanf()
  char Loc_Time[SPRINTF_BUFFER_SIZE];       // Buffer for sprintf()/sscanf()

  sprintf(Loc_Date, "%04d/%02d/%02d", LocYear, LocMonth, LocDate);
  // Convert military time to Am/Pm format
  if (LocHour > 12) {
    LocHour = LocHour - 12;
    LocAmPm = "PM";
  } else {
    LocAmPm = "AM";
  }
  sprintf(Loc_Time, "%02d:%02d:%02d %s", LocHour, LocMin, LocSec, LocAmPm);
//  Serial.print("Australian Eastern Standard Date / Time is: ");
  Serial.print(Loc_Date);
  Serial.print("  ");
  Serial.print(Loc_Time);
  Serial.print("  ");
  Serial.print("AEST Epoch = ");
  AESTEpoch = AEST.unixtime();
  Serial.print(AESTEpoch);
  Serial.print("  ");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(Loc_Date);
  lcd.setCursor(0, 1);
  lcd.print(Loc_Time);
  gv_PreviousMillis1000 = millis();

  datertc.adjust(DateTime(LocYear, LocMonth, LocDate, 0, 0, 0));
  datenow = datertc.now();
  EpochToday = datenow.unixtime();
  Serial.print("Epoch today = ");
  Serial.println(EpochToday);
}
