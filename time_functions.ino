/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets
byte alarm_nr = 0;

void setupTime()
{
  Udp.begin(localPort);
  //  setSyncProvider(getNtpTime);
  setSyncProvider(getDstCorrectedTime);
  setSyncInterval(300);
  
  Alarm.delay(0);

  if (WiFi.status() != WL_CONNECTED)
  {
    setupSDCard();
    readFile(SD_MMC, "/time.txt");
    digitalClockDisplay();
  }
  else
  {
    saveTime();
  }
}

void saveTime()
{
  char time_now_array[20];
  int time_now = now();
  sprintf(time_now_array, "%d", time_now);
  setupSDCard();
  writeFile(SD_MMC, "/time.txt", time_now_array);
  endSDCard();
  digitalClockDisplay();
}

void resetAlarms()
{
  for (int i = 0; i < alarm_nr; i++)
  {
    Alarm.free(ids[i]);
    ids[i] = dtINVALID_ALARM_ID;
  }
  alarm_nr = 0;
}


void add_recurring_alarm(byte _day, byte _hour, byte _minute)
{
  Serial.print("day: ");
  Serial.println(_day);
  Serial.print("current day: ");
  Serial.println(day());
  switch (_day) {
    case 0:
      ids[alarm_nr] = Alarm.alarmRepeat(_hour, _minute, 0, feed);
      break;
    case 1:
      ids[alarm_nr] = Alarm.alarmRepeat(dowSunday, _hour, _minute, 0, feed);
      break;
    case 2:
      ids[alarm_nr] = Alarm.alarmRepeat( dowMonday, _hour, _minute, 0, feed);
      break;
    case 3:
      ids[alarm_nr] = Alarm.alarmRepeat(dowTuesday, _hour, _minute, 0, feed);
      break;
    case 4:
      ids[alarm_nr] = Alarm.alarmRepeat( dowWednesday, _hour, _minute, 0, feed);
      break;
    case 5:
      ids[alarm_nr] = Alarm.alarmRepeat(dowThursday, _hour, _minute, 0, feed);
      break;
    case 6:
      ids[alarm_nr] = Alarm.alarmRepeat(dowFriday, _hour, _minute, 0, feed);
      break;
    case 7:
      ids[alarm_nr] = Alarm.alarmRepeat(dowSaturday, _hour, _minute, 0, feed);
      break;
    default:
      ids[alarm_nr] = Alarm.alarmRepeat(_hour, _minute, 0, feed);
      break;
  }
  Serial.print("ids repeat ");
  Serial.println(ids[alarm_nr]);
  Serial.println(_hour);
  Serial.println(_minute);
  alarm_nr += 1;
}

//void add_alarm_once(byte _hour, byte _minute)
//{
//  Alarm.alarmOnce(_hour, _minute, 0, feed);
//  Serial.println("alarm once ");
//  Serial.println(_hour);
//  Serial.println(_minute);
//}

void add_alarm_once(unsigned int timestamp)
{
  Alarm.triggerOnce(timestamp, feed);
  Serial.print("alarm once: ");
  Serial.println(timestamp);
  Alarm.delay(0);
}

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL;
    }
  }
  Serial.println("No NTP Response :-(");

  return 0; // return current time
}


/* Alternative SyncProvider that automatically handles Daylight Saving Time (DST) periods,
   at least in Europe, see below.
*/
time_t getDstCorrectedTime (void) {
  time_t t = getNtpTime ();

  if (t > 0) {
    TimeElements tm;
    breakTime (t, tm);
    t += (utcOffset + dstOffset (tm.Day, tm.Month, tm.Year + 1970, tm.Hour)) * SECS_PER_HOUR;
  }

  return t;
}

/* This function returns the DST offset for the current UTC time.
   This is valid for the EU, for other places see
   http://www.webexhibits.org/daylightsaving/i.html

   Results have been checked for 2012-2030 (but should work since
   1996 to 2099) against the following references:
   - http://www.uniquevisitor.it/magazine/ora-legale-italia.php
   - http://www.calendario-365.it/ora-legale-orario-invernale.html
*/
byte dstOffset (byte d, byte m, unsigned int y, byte h) {
  // Day in March that DST starts on, at 1 am
  byte dstOn = (31 - (5 * y / 4 + 4) % 7);

  // Day in October that DST ends  on, at 2 am
  byte dstOff = (31 - (5 * y / 4 + 1) % 7);

  if ((m > 3 && m < 10) ||
      (m == 3 && (d > dstOn || (d == dstOn && h >= 1))) ||
      (m == 10 && (d < dstOff || (d == dstOff && h <= 1))))
    return 1;
  else
    return 0;
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(".");
  Serial.print(month());
  Serial.print(".");
  Serial.print(year());
  Serial.println();
}

void printDigits(int digits)
{
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
