//done sending with serial pin

#include <Arduino.h>
#include <Wire.h>
#include "esp_sleep.h"

#include <Arduino.h>
#include "WiFi.h" 
#include "driver/adc.h"
#include <esp_wifi.h>
#include <esp_bt.h>

int count = 0;

// #define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
// #define TIME_TO_SLEEP 20       /* Time ESP32 will go to sleep (in seconds) */
// RTC_DATA_ATTR int bootCount = 0;

// #define RXD2 16
// #define TXD2 17

String UDP_IP = "147.8.183.95";
String UDP_port = "6001";

void bc95write(String command)
{
    command = command + "\n";
    for (unsigned int i = 0; i < command.length(); i++)
    {
        Serial.write(byte(command.charAt(i)));    //serial2
    }
}

String getresponse()
{
    while (Serial.available() == 0)    //serial2
    {
        delay(1);
    }
    String response = "";

    while (Serial.available())    //serial2
    {
        delay(2);
        response += char(Serial.read());    //serial2
    }
    // Serial.print(response);    //serial
    return response;
}

void bc95_reset()
{
    // Serial.println("Setting up...(7s)");     //serial
    delay(200);
    bc95write("AT+NRB");
    // Serial.println("*****");    //serial
    delay(7000);
    getresponse();
    bc95write("AT+CFUN=1");
    getresponse();
    bc95write("AT+CGATT=1");
    getresponse();
    bc95write("AT");
    getresponse();
    bc95write("AT+NSOCR=DGRAM,17,3000,1");
    getresponse();
    delay(2000);
}

int counter()
{
    count++;
    return count;
}

int getdBm(){
    bc95write("AT+CSQ");
    String temp = getresponse();
    int dBm = 113-temp.substring(7,9).toInt()*2;
    // Serial.println(dBm);     //serial
    return dBm;
}

String packagebuilder(String temp)
{
    String package = "";
    for (unsigned int i = 0; i < temp.length(); i++)
    {
        package += String(byte(temp.charAt(i)), HEX);
    }
    return package;
}


void flightmodeon()
{
    bc95write("AT+CFUN=0");
    delay(500);
    getresponse();
}


void flightmodeoff()
{
    bc95write("AT+CFUN=1");
    // Serial.println("waking");    //serial
    delay(5000);
    getresponse();
}

void sendpackage()
{
    // flightmodeoff();
    String content = packagebuilder(String(counter()) + ",dBm:-" + String(getdBm()));
    String msg = "AT+NSOST=1," + UDP_IP + "," + UDP_port + "," + String(content.length()/2) + "," + content;
    // Serial.println(msg);   //serial
    bc95write(msg);
    getresponse();
    // flightmodeon();
}



void setup()
{
  Serial.begin(9600);
//   Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); //BC95
  delay(500);

  bc95_reset();
}

void loop()
{
  delay(10000);
  sendpackage();
}