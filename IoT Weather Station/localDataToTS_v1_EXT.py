"""
MJRoBot IoT Local Weather Station

Sensor Data - Station:
	Temperature: External (not physicaly at station)
	Temperature: Station
	Humidity: Station
	Pressure: (Sea Level) 
	Altitude (calculated due absolut atmosferic pression)

Real Altitude: Station  ==> 950m (Lo Barnechea, Chile)

All local data uploaded to ThingSpeak.com
Channel Id: 483033

Client library for the thingspeak.com API developed by Mikolaj Chwaliz and Keith Ellis.
The library can be download from: https://github.com/mchwalisz/thingspeak

Install using: pip install thingspeak

Code developed by Marcelo Rovai - MJRoBot.org @ 26April18

"""

import time
import datetime

# Client library for the thingspeak.com API 
import thingspeak  

# ThingSpeak channel credentials 
chId = 483033
tsKey='ENTER WITH YOUR CHANNEL WRITE KEY'
tsUrl='https://api.thingspeak.com/update'
ts = thingspeak.Channel(chId, tsUrl ,tsKey)

# DS18B20 1-Wire library
from w1thermsensor import W1ThermSensor 
ds18b20Sensor = W1ThermSensor() # By default GPIO 4 is used by library

# DHT22 Library 
import Adafruit_DHT
DHT22Sensor = Adafruit_DHT.DHT22
DHTpin = 16

# BMP180 library
import Adafruit_BMP.BMP085 as BMP085 
bmp180Sensor = BMP085.BMP085()

# Global Variables
global altReal
altReal = 950

# Get data (from local sensors)
def getLocalData():
	global timeString
	global humLab
	global tempExt
	global tempLab
	global presSL
	global altLab
	global presAbs
	
	# Get time of reading
	now = datetime.datetime.now()
	timeString = now.strftime("%Y-%m-%d %H:%M")
	
	# Read Outside Temperature (1 meter distance)
	tempExt = round(ds18b20Sensor.get_temperature(), 1)
	
	tempLab, presAbs, altLab, presSL = bmp180GetData(altReal) 
	
	humDHT, tempDHT = Adafruit_DHT.read_retry(DHT22Sensor, DHTpin)
	if humDHT is not None and tempDHT is not None:
		humLab = round (humDHT)

# Get BMP180 data		
def bmp180GetData(altitude):
	
	temp = bmp180Sensor.read_temperature()
	pres = bmp180Sensor.read_pressure()
	alt =  bmp180Sensor.read_altitude()
	presSeaLevel = pres / pow(1.0 - altitude/44330.0, 5.255) 
	
	temp = round (temp, 1)
	pres = round (pres/100, 2) # absolute pressure in hPa (or mbar)
	alt = round (alt)
	presSeaLevel = round (presSeaLevel/100, 2) # absolute pressure in hPa (or mbar)

	return temp, pres, alt, presSeaLevel

# Send data to ThingSpeak
def sendDataTs():
	data = {"field1": tempLab, 
			"field2": tempExt, 
			"field3": humLab, 
			"field4": presSL, 
			"field5": altLab
			}
	ts.update(data)
	print ("[INFO] Data sent for 5 fields: ", tempLab, tempExt, humLab, presSL, altLab)
  
# Main function
def main():	
	print ("[INFO] Iniciating")
	while True:
		getLocalData()
		try:
			sendDataTs()
			time.sleep(60)
		except (KeyboardInterrupt):
			print ("[INFO] Finishing")
			break

''''--------------------------------------------------------------'''
if __name__ == '__main__':
	main()
