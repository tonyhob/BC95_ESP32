#include <Arduino.h>
#include <Wire.h>
#include "esp_sleep.h"

#include <main.h>

// #define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
// #define TIME_TO_SLEEP 20       /* Time ESP32 will go to sleep (in seconds) */
// RTC_DATA_ATTR int bootCount = 0;

#define RXD2 16
#define TXD2 17

void setup()
{
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); //BC95
  delay(500);

  bc95_reset();
}

void loop()
{
  delay(20000);
  sendpackage();
}