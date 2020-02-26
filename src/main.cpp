#include <Arduino.h>
#include <Wire.h>
#include "esp_sleep.h"


#include <main.h>

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  20       /* Time ESP32 will go to sleep (in seconds) */
RTC_DATA_ATTR int bootCount = 0;

#define RXD2 16
#define TXD2 17


void setup()
{


  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); //BC95
  delay(500);

  // if(bootCount == 0) //Run this only the first time
  // {
      reset();
  //     sendpackage();
  //     flightmodeoff();
  //     bootCount = bootCount+1;
  // }else
  // {
  //     flightmodeon();
  //     sendpackage();
  //     flightmodeoff();
  // }


  // delay(5000);
  // esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  // esp_sleep_pd_config(ESP_PD_DOMAIN_MAX, ESP_PD_OPTION_OFF);
  // esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  // esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
  // esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
  // Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");
  // Serial.println("Going to sleep now");
  // Serial.flush();
  // esp_deep_sleep_start();

}

void loop()
{
  delay(20000);
  sendpackage();
}