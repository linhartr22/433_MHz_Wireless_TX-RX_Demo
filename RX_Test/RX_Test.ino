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

#define DEBUG

#include <VirtualWire.h>
// Init VWMsgBuf.
uint8_t VWMsgBuf[VW_MAX_MESSAGE_LEN];
// Init VWMsgStr.
String VWMsgStr;

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
  uint8_t VWBufLen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(VWMsgBuf, &VWBufLen)) {
    // Flash a light to show received good message.
    digitalWrite(13, true); 
    // Message with a good checksum received, dump it.
    VWMsgStr = "";
    for (int i = 0; i < VWBufLen; i++) {
      VWMsgStr += char(VWMsgBuf[i]);
      //Serial.print(char(buf[i]));
    }
    #ifdef DEBUG
      Serial.println(VWMsgStr);
    #endif
    digitalWrite(13, false);
  }
}

