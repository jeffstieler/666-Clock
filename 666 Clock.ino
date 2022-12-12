// Date and time functions using a PCF8523 RTC connected via I2C and Wire lib
#include "RTClib.h"

// Adafruit seven segment library
#include "Adafruit_LEDBackpack.h"

// madleech Button library
#include "Button.h"

// I2C address of the display.  Stick with the default address of 0x70
// unless you've changed the address jumpers on the back of the display.
#define DISPLAY_ADDRESS 0x70
#define COLON_DIGIT 2
#define CENTER_COLON 0x02
#define LEFT_COLON_LOWER 0x08
#define LEFT_COLON_UPPER 0x04
#define HOUR_BUTTON_PIN 2
#define MINUTE_BUTTON_PIN 3

RTC_PCF8523 rtc;
Adafruit_7segment display = Adafruit_7segment();
Button hourButton(HOUR_BUTTON_PIN);
Button minuteButton(MINUTE_BUTTON_PIN);

void setup() {
  Serial.begin(57600);

  hourButton.begin();
  minuteButton.begin();

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();

    while (1) {
      delay(10);
    }
  }

  if (!rtc.initialized() || rtc.lostPower()) {
    Serial.println("RTC is NOT initialized, let's set the time!");
    // Note: allow 2 seconds after inserting battery or applying external power
    // without battery before calling adjust(). This gives the PCF8523's
    // crystal oscillator time to stabilize. If you call adjust() very quickly
    // after the RTC is powered, lostPower() may still return true.
    delay(2000);

    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date this sketch was compiled at
    // 12:00am.
    rtc.adjust(DateTime(F(__DATE__), F("00:00:00")));
  }

  // When the RTC was stopped and stays connected to the battery, it has
  // to be restarted by clearing the STOP bit. Let's do this to ensure
  // the RTC is running.
  rtc.start();

  display.begin(0x70);
}

void displayTime(uint8_t hour, uint8_t minute, uint8_t second) {
  // Show the time on the display by turning it into a numeric
  // value, like 3:30 turns into 330, by multiplying the hour by
  // 100 and then adding the minutes.
  int displayValue = (hour * 100) + minute;

  // 24 hour to 12 hour format conversion.
  if (hour > 12) {
    displayValue -= 1200;
  }
  // Handle hour 0 (midnight) being shown as 12.
  else if (hour == 0) {
    displayValue += 1200;
  }

  // Print the time value to the display.
  display.print(displayValue, DEC);

  int colonValue = 0;

  // Use the lower left colon dot as PM indicator.
  if (hour >= 12) {
    colonValue += LEFT_COLON_UPPER;
  }

  // Blink the center colon every other second.
  if (second % 2 == 0) {
    colonValue += CENTER_COLON;
  }

  display.writeDigitRaw(COLON_DIGIT, colonValue);
  display.writeDisplay();
}

void loop() {
  DateTime now = rtc.now();
  int hour = now.hour();
  int minute = now.minute();
  int second = now.second();

  // Note: Button::pressed() also requires a change, so it doesn't
  // work for detecting when the button is held down.

  // Handle setting the hour, when *only* the hour button is pressed.
  if (hourButton.read() == Button::PRESSED &&
      minuteButton.read() == Button::RELEASED) {
    hour = (hour + 1) % 24;

    rtc.adjust(
        DateTime(now.year(), now.month(), now.day(), hour, minute, second));
  }

  // Handle setting the minute, when *only* the minute button is pressed.
  if (minuteButton.read() == Button::PRESSED &&
      hourButton.read() == Button::RELEASED) {
    minute = (minute + 1) % 60;

    rtc.adjust(
        DateTime(now.year(), now.month(), now.day(), hour, minute, second));
  }

  // The entire reason we made this clock.
  if ((hour == 7 || hour == 19) && minute == 6) {
    displayTime(hour - 1, 66, second);
  } else {
    // Boooooring.
    displayTime(hour, minute, second);
  }

  delay(100);
}
