// Date and time functions using a PCF8523 RTC connected via I2C and Wire lib
#include "RTClib.h"

RTC_PCF8523 rtc;

void setup() {
  Serial.begin(57600);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();

    while (1) {
      delay(10);
    }
  }

  if (!rtc.initialized() || rtc.lostPower()) {
    Serial.println("RTC is NOT initialized, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date this sketch was compiled at
    // 12:00am.
    rtc.adjust(DateTime(F(__DATE__), F("12:00:00")));

    // Note: allow 2 seconds after inserting battery or applying external power
    // without battery before calling adjust(). This gives the PCF8523's
    // crystal oscillator time to stabilize. If you call adjust() very quickly
    // after the RTC is powered, lostPower() may still return true.
  }

  // When the RTC was stopped and stays connected to the battery, it has
  // to be restarted by clearing the STOP bit. Let's do this to ensure
  // the RTC is running.
  rtc.start();
}

void loop() {
  DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(3000);
}
