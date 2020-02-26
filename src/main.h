#include <Arduino.h>
#include "WiFi.h" 
#include "driver/adc.h"
#include <esp_wifi.h>
#include <esp_bt.h>

int count = 0;

String UDP_IP = "147.8.183.95";
String UDP_port = "6001";

void bc95write(String command)
{
    command = command + "\n";
    for (unsigned int i = 0; i < command.length(); i++)
    {
        Serial2.write(byte(command.charAt(i)));
    }
}

String getresponse()
{
    while (Serial2.available() == 0)
    {
        delay(1);
    }
    String response = "";

    while (Serial2.available())
    {
        delay(2);
        response += char(Serial2.read());
    }
    Serial.print(response);
    return response;
}

void bc95_reset()
{
    Serial.println("Setting up...(7s)");
    delay(200);
    bc95write("AT+NRB");
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
    // Serial.println(dBm);
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
    Serial.println("waking");
    delay(5000);
    getresponse();
}

void sendpackage()
{
    // flightmodeoff();
    String cont = packagebuilder(String(counter()) + ",dBm:-" + String(getdBm()));
    String msg = "AT+NSOST=1," + UDP_IP + "," + UDP_port + "," + String(cont.length()/2) + "," + cont;
    Serial.println(msg);
    bc95write(msg);
    getresponse();
    // flightmodeon();
}