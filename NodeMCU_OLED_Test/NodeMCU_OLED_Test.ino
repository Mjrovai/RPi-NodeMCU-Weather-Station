/***************************************************************** 
* 
* NodeMCU OLED Test
* Special Libraries needed:
*   SSD1306Wire.h: https://github.com/squix78/esp8266-oled-ssd1306
* 
* 128x64 OLED pinout:
*   - GND goes to ground
*   - Vin goes to 3.3V
*   - Data to I2C SDA (GPIO 0 ==>D3)
*   - Clk to I2C SCL  (GPIO 2 ==>D4)
*    
* Developed by Marcelo Rovai https://MJRoBot.org 
* Version 1.0 7May17
*****************************************************************/

/* OLED */
#include "SSD1306Wire.h"
#include "Wire.h"
const int I2C_DISPLAY_ADDRESS = 0x3c;
const int SDA_PIN = 0;
const int SCL_PIN = 2;
SSD1306Wire display(I2C_DISPLAY_ADDRESS, SDA_PIN, SCL_PIN);

void setup () 
{
  Serial.begin(115200); 
  displaySetup();
}

void loop() 
{ 
}

/* Initiate and display setup data on OLED */
void displaySetup()
{
  display.init();         // initialize display
  display.clear();        // Clear display
  display.flipScreenVertically(); // Turn the display upside down
  display.display();      // Put data on display
  
  Serial.println("Initiating Display Test");
  
  display.setFont(ArialMT_Plain_24);
  display.drawString(30, 0, "OLED");  // (xpos, ypos, "Text")
  display.setFont(ArialMT_Plain_16);
  display.drawString(18, 29, "Test initiated");
  display.setFont(ArialMT_Plain_10);
  display.drawString(10, 52, "Serial BaudRate:");
  display.drawString(90, 52, String(11500));
  display.display();  // Put data on display
  delay (3000);
}


