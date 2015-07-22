// receiver.pde
//
// Simple example of how to use VirtualWire to receive messages
// Implements a simplex (one-way) receiver with an Rx-B1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: receiver.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $
// RX data to Digital I/O pin 11.
// Receive TX data.
// Get local DHT11 data.

#define DEBUG

#include <VirtualWire.h>
// Init VWMsgBuf.
uint8_t VWMsgBuf[VW_MAX_MESSAGE_LEN];
// Init VWMsgStr.
String VWMsgStr;

#include <dht11.h>
// Init DHT11.
dht11 DHT11;
// DHT11 OUT to digital I/O pin.
#define DHT11_PIN 2
// DHT11 Sample Time
const unsigned long DHT11SampMillisMax = 1000;
unsigned long DHT11SampMillis = millis() + DHT11SampMillisMax;

// setup() runs once after reset.
void setup() {
  #ifdef DEBUG
    // Init serial for debugging.
    Serial.begin(9600);
  #endif
  // Init BPS.
  vw_setup(2000);
  // Start the receiver.
  vw_rx_start();       
  // Send setup() message.
  VWMsgStr = "LOCAL: setup()!";
  #ifdef DEBUG
    Serial.println(VWMsgStr);
  #endif
}

// loop() runs continuously after setup().
void loop() {
  // Init Buffer Length
  uint8_t VWBufLen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(VWMsgBuf, &VWBufLen)) {
    // Flash a light to show received good message.
    digitalWrite(13, true); 
    // Message with a good checksum received, dump it.
    VWMsgStr = "";
    for (int i = 0; i < VWBufLen; i++) {
      VWMsgStr += char(VWMsgBuf[i]);
    }
    #ifdef DEBUG
      Serial.println(VWMsgStr);
    #endif
    digitalWrite(13, false);
  }

  // Get Local DHT11 message?
  if (millis() >= DHT11SampMillis) {
    //Yes, update DHT11SampMillis
    DHT11SampMillis = millis() + DHT11SampMillisMax;
    int rc = DHT11.read(DHT11_PIN);
    switch (rc) {
      case DHTLIB_OK:
        VWMsgStr = "LOCAL: OK. Humi%: " + String(DHT11.humidity);
        VWMsgStr += "  TempC: " + String(DHT11.temperature);
        VWMsgStr += "  TempF: " + String(fahrenheit(DHT11.temperature));
        break;
      case DHTLIB_ERROR_CHECKSUM:
        VWMsgStr = "LOCAL: Checksum!";
        break;
      case DHTLIB_ERROR_TIMEOUT:
        VWMsgStr = "LOCAL: Time Out!";
        break;
      default:
        VWMsgStr = "LOCAL: Unknown!";
        break;
    }
    #ifdef DEBUG
      Serial.println(VWMsgStr);
    #endif
  }
}

// Returns int fahrenheit from int celsius.
int fahrenheit(int celsius) {
  return (celsius * 18 + 5) / 10 + 32;
}

/*
// Returns double fahrenheit from double celsius.
double fahrenheit(double celsius) {
  return 1.8 * celsius + 32;
}

// Returns double kelvin from double celsius.
double kelvin(double celsius) {
  return celsius + 273.15;
}

// dewPoint function NOAA
// reference (1) : http://wahiduddin.net/calc/density_algorithms.htm
// reference (2) : http://www.colorado.edu/geography/weather_station/Geog_site/about.htm
double dewPoint(double celsius, double humidity) {
  // (1) Saturation Vapor Pressure = ESGG(T)
  double RATIO = 373.15 / (273.15 + celsius);
  double RHS = -7.90298 * (RATIO - 1);
  RHS += 5.02808 * log10(RATIO);
  RHS += -1.3816e-7 * (pow(10, (11.344 * (1 - 1/RATIO ))) - 1) ;
  RHS += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1) ;
  RHS += log10(1013.246);
  // factor -3 is to adjust units - Vapor Pressure SVP * humidity
  double VP = pow(10, RHS - 3) * humidity;
  // (2) DEWPOINT = F(Vapor Pressure)
  double T = log(VP/0.61078);   // temp var
  return (241.88 * T) / (17.558 - T);
}

// delta max = 0.6544 wrt dewPoint()
// 6.9 x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity) {
  double a = 17.271;
  double b = 237.7;
  double temp = (a * celsius) / (b + celsius) + log(humidity*0.01);
  double Td = (b * temp) / (a - temp);
  return Td;
}
*/
