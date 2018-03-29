/***************************************************************************************************************
 *  UV Sensor using NodeMCU ESP-12E
 *  Including 128x64 OLED Display
 *  Libraries needed:
 *   SSD1306Wire.h: https://github.com/squix78/esp8266-oled-ssd1306
 *  
 *  UV Sensor Module connected to NodeMCU pin A0
 *  OLED pinout:
 *   - GND goes to ground
 *   - Vin goes to 3.3V
 *   - Data to I2C SDA (GPIO 0 ==>D3)
 *   - Clk to I2C SCL  (GPIO 2 ==>D4)
 *   
 *  Developed by MJRovai V.1 23 March 2018 
 ********************************************************************************************************************************/
#define SW_VERSION "UV_Sensor_V.1" 

/* UV Sensor */
#define sensorUVPin A0
int dataSensorUV = 0;
int indexUV = 0;

/* OLED */
#include "SSD1306Wire.h"
#include "Wire.h"
const int I2C_DISPLAY_ADDRESS = 0x3c;
const int SDA_PIN = 0;
const int SCL_PIN = 2;
SSD1306Wire display(I2C_DISPLAY_ADDRESS, SDA_PIN, SCL_PIN);

void setup()
{
  Serial.begin(115200);
  displaySetup();
}

void loop()
{                    
  readSensorUV();
  displayUV();
  delay (1000);
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
  display.drawString(0, 29, "UV Sensor Test");
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

/*  Display UV Values on local OLED*/
void displayUV()
{
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.drawString(20, 0, "UV Sensor");
  display.drawString(0, 23, "UV (mV):" );
  display.drawString(80, 23, String(dataSensorUV));
  display.drawString(0, 48, "UV Index:" );
  display.setFont(ArialMT_Plain_24);
  display.drawString(82, 42, String(indexUV));
  display.display();
}
