// transmitter.pde
//
// Simple example of how to use VirtualWire to transmit messages.
// Implements a simplex (one-way) transmitter.
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: transmitter.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $
// TX data to Digital I/O pin 12.
// Sends DHT11 data.

#define DEBUG

#include <VirtualWire.h>
// Init VWMsgBuf.
char VWMsgBuf[VW_MAX_MESSAGE_LEN];
// Init VWMsgStr.
String VWMsgStr;

#include <dht11.h>
// Init DHT11.
dht11 DHT11;
// DHT11 OUT to digital I/O pin.
#define DHT11_PIN 2

// Init hello world count.
unsigned int cnt = 0;

// setup() runs once after reset.
void setup() {
  #ifdef DEBUG
    // Init serial for debugging.
    Serial.begin(9600);
  #endif
  // Init BPS.
  vw_setup(2000);
  // Send setup() message.
  VWMsgStr = "REMOTE1: setup()!";
  VWTX(VWMsgStr);
  #ifdef DEBUG
    Serial.println(VWMsgStr);
  #endif
}

// loop() runs continuously after setup().
void loop() {
  // Send hello world message.
  VWMsgStr = "REMOTE1: Hello World!  Count=" + String(cnt++);
  VWTX(VWMsgStr);
  #ifdef DEBUG
    Serial.println(VWMsgStr);
  #endif

  // Send DHT11 message.
  int rc = DHT11.read(DHT11_PIN);
  switch (rc) {
    case DHTLIB_OK:
      VWMsgStr = "REMOTE1: OK. Humi%: " + String(DHT11.humidity);
      VWMsgStr += "  TempC: " + String(DHT11.temperature);
      VWMsgStr += "  TempF: " + String(fahrenheit(DHT11.temperature));
      break;
    case DHTLIB_ERROR_CHECKSUM:
      VWMsgStr = "REMOTE1: Checksum!";
      break;
    case DHTLIB_ERROR_TIMEOUT:
      VWMsgStr = "REMOTE1: Time Out!";
      break;
    default:
      VWMsgStr = "REMOTE1: Unknown!";
      break;
  }
  VWTX(VWMsgStr);
  #ifdef DEBUG
    Serial.println(VWMsgStr);
  #endif
}

// Virtual Wire Transmit msgStr
void VWTX(String VWMsgStr) {
  VWMsgStr.toCharArray(VWMsgBuf, VW_MAX_MESSAGE_LEN);
  uint8_t VWMsgBufLen = strlen(VWMsgBuf);
  digitalWrite(13, true); // Flash a light to show transmitting
  vw_send((uint8_t *)VWMsgBuf, strlen(VWMsgBuf));
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(13, false);
  delay(1000);
}

/*
// Returns double fahrenheit from double celsius.
double fahrenheit(double celsius) {
  return 1.8 * celsius + 32;
}
*/

// Returns int fahrenheit from int celsius.
int fahrenheit(int celsius) {
  return (celsius * 18 + 5) / 10 + 32;
}

/*
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


