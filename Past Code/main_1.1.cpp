//NB-IoT tester
//done OLED 

#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// #include "esp_sleep.h"
// #include "driver/adc.h"

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

int code;
int count = 0;
int dbm_sum;
int dbm_mean;

// #define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
// #define TIME_TO_SLEEP 20       /* Time ESP32 will go to sleep (in seconds) */
// RTC_DATA_ATTR int bootCount = 0;

// #define RXD2 16
// #define TXD2 17

String UDP_IP = "147.8.183.95";
String UDP_port = "6001";

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);
void oledsetup()
{
    //reset OLED display via software
    pinMode(OLED_RST, OUTPUT);
    digitalWrite(OLED_RST, LOW);
    delay(20);
    digitalWrite(OLED_RST, HIGH);

    //initialize OLED
    Wire.begin(OLED_SDA, OLED_SCL);
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false))
    { // Address 0x3C for 128x32
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("NB-IoT Tester: " + String(code));
    display.drawLine(0, 10, display.width() - 1, 10, WHITE);
    display.setCursor(0, 15);
    display.print("Connecting...in 10s");
    display.display();
}

void displaycon(int tempcount, int tempdbm, int tempdbm_mean)
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("NB-IoT Tester: " + String(code));
    display.drawLine(0, 10, display.width() - 1, 10, WHITE);
    display.setCursor(0, 15);
    display.print("Packet  dBm   Mean");
    display.setTextSize(2);
    display.setCursor(0, 27);
    display.print(String(tempcount) + " -" + String(tempdbm) + " -" + String(tempdbm_mean));
    display.drawLine(0, 52, display.width() - 1, 52, WHITE);
    display.setTextSize(1);
    display.setCursor(0, 54);
    display.print("Ref: Gd:-51 Bd:-113;");  
    display.display();
}

void bc95write(String command)
{
    command = command + "\n";
    for (unsigned int i = 0; i < command.length(); i++)
    {
        Serial.write(byte(command.charAt(i))); //serial2
    }
}

String getresponse()
{
    while (Serial.available() == 0) //serial2
    {
        delay(1);
    }
    String response = "";

    while (Serial.available()) //serial2
    {
        delay(2);
        response += char(Serial.read()); //serial2
    }
    // Serial.print(response);    //serial
    // display.clearDisplay();
    // display.setTextSize(1);
    // display.setCursor(0, 0);
    // display.print(response);
    // display.display();

    return response;
}

void bc95_reset()
{
    // Serial.println("Setting up...(7s)");     //serial
    delay(200);
    bc95write("AT+NRB");
    // Serial.println("*****");    //serial
    delay(7000);
    String tem = getresponse();
    bc95write("AT+CFUN=1");
    tem = getresponse();
    bc95write("AT+CGATT=1");
    tem = getresponse();
    bc95write("AT");
    tem = getresponse();
    bc95write("AT+NSOCR=DGRAM,17,3000,1");
    tem = getresponse();
    delay(2000);
}

int counter()
{
    count++;
    return count;
}

int getdBm()
{
    bc95write("AT+CSQ");
    String temp = getresponse();
    int dBm = 113 - temp.substring(7, 9).toInt() * 2;
    // Serial.println(dBm); //serial
    return dBm;
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

String packagebuilder(String temp)
{
    String package = "";
    for (unsigned int i = 0; i < temp.length(); i++)
    {
        package += String(byte(temp.charAt(i)), HEX);
    }
    return package;
}

void sendpackage()
{
    int dbm = getdBm();
    int count = counter();
    dbm_sum += dbm;
    dbm_mean = dbm_sum / count;
    displaycon(count, dbm, dbm_mean);
    // flightmodeoff();
    String content = packagebuilder(String(code) + " - packet:" + String(count) + ", dBm:-" + String(dbm));
    String msg = "AT+NSOST=1," + UDP_IP + "," + UDP_port + "," + String(content.length() / 2) + "," + content;
    // Serial.println(msg);   //serial
    bc95write(msg);
    getresponse();
    // flightmodeon();
}

void setup()
{
    code = random(1000, 10000);
    Serial.begin(9600);
    oledsetup();
    bc95_reset();
}

void loop()
{
    delay(10000);
    sendpackage();
}