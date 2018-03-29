/*********************************************************************
 *  RPi-NodeMCU Weather Station - Version Local
 *     UV Sensor
 *     DHT SEnsor (Air Temperature & Humidity)
 *  NodeMCU ESP-12E
 *  128x64 OLED Display
 *  Libraries needed:
 *   SSD1306Wire.h: https://github.com/squix78/esp8266-oled-ssd1306
 *  
 *  UV Sensor Module connected to NodeMCU pin A0
 *  DHT connected to NodeMCU (GPIO 4 ==>D2)
 *  OLED pinout:
 *   - GND goes to ground
 *   - Vin goes to 3.3V
 *   - Data to I2C SDA (GPIO 0 ==>D3)
 *   - Clk to I2C SCL  (GPIO 2 ==>D4)
 *   
 *  Developed by MJRovai V.1 26 March 2018 
 *********************************************************************/
#define SW_VERSION "UV_T&H_V.LC" 

/* UV Sensor */
#define sensorUVPin A0
int dataSensorUV = 0;
int indexUV = 0;

/* DHT22*/
#include "DHT.h"
#define DHTPIN D2  
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);
float hum = 0;
float temp = 0;

/* OLED */
#include "SSD1306Wire.h"
#include "Wire.h"
const int I2C_DISPLAY_ADDRESS = 0x3c;
const int SDA_PIN = 0;
const int SCL_PIN = 2;
SSD1306Wire display(I2C_DISPLAY_ADDRESS, SDA_PIN, SCL_PIN);

/* Timer */
#include <SimpleTimer.h>
SimpleTimer timer;

void setup()
{
  Serial.begin(115200);
  displaySetup();
  timer.setInterval(20000L, getDhtData);
  timer.setInterval(30000L, readSensorUV);
}

void loop()
{                    
  displayLocalData();
  timer.run(); // Initiates SimpleTimer
}

/* Initiate and display setup data on OLED */
void displaySetup()
{
  display.init();                 // initialize display
  display.clear();                // Clear display
  display.flipScreenVertically(); // Turn the display upside down
  display.display();              // Put data on display

  Serial.println("Initiating UV Sensor Test");
  display.setFont(ArialMT_Plain_24);
  display.drawString(10, 0, "MJRoBot");  
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 29, "UV Temp Hum Test");
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 52, "SW Ver.:");
  display.drawString(45, 52, SW_VERSION);
  display.display();
  delay (3000);
}

/* Read UV Sensor in mV and call UV index calculation */
void readSensorUV()
{                    
  byte numOfReadings = 5;
  dataSensorUV = 0;
  for (int i=0; i< numOfReadings; i++) 
  {
    dataSensorUV += analogRead(sensorUVPin);
    delay (200);
  }
  dataSensorUV /= numOfReadings;
  dataSensorUV = (dataSensorUV * (3.3 / 1023.0))*1000;
  Serial.println(dataSensorUV);
  indexCalculate();
}

/*  UV Index calculation */
void indexCalculate()
{
    if (dataSensorUV < 227) indexUV = 0;
    else if (227 <= dataSensorUV && dataSensorUV < 318) indexUV = 1;
    else if (318 <= dataSensorUV && dataSensorUV < 408) indexUV = 2;
    else if (408 <= dataSensorUV && dataSensorUV < 503) indexUV = 3;
    else if (503 <= dataSensorUV && dataSensorUV < 606) indexUV = 4;    
    else if (606 <= dataSensorUV && dataSensorUV < 696) indexUV = 5;
    else if (696 <= dataSensorUV && dataSensorUV < 795) indexUV = 6;
    else if (795 <= dataSensorUV && dataSensorUV < 881) indexUV = 7; 
    else if (881 <= dataSensorUV && dataSensorUV < 976) indexUV = 8;
    else if (976 <= dataSensorUV && dataSensorUV < 1079) indexUV = 9;  
    else if (1079 <= dataSensorUV && dataSensorUV < 1170) indexUV = 10;
    else indexUV = 11;       
}

/* Get DHT data */
void getDhtData(void)
{
  float tempIni = temp;
  float humIni = hum;
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  if (isnan(hum) || isnan(temp))   // Check if any reads failed and exit early (to try again).
  {
    Serial.println("Failed to read from DHT sensor!");
    temp = tempIni;
    hum = humIni;
    return;
  }
}

/*  Display UV Values on local OLED*/
void displayLocalData()
{
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "Temp [oC]");
  display.drawString(80, 0, String(temp));
  display.drawString(0, 15, "Hum [%]:");
  display.drawString(80, 15, String(hum));
  display.drawString(0, 30, "UV (mV):" );
  display.drawString(80, 30, String(dataSensorUV));
  display.drawString(0, 45, "UV Index:" );
  display.drawString(80, 45, String(indexUV));
  display.display();
}

