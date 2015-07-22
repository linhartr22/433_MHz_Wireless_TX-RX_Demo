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
#define DEBUG
#include <VirtualWire.h>
// Init msgBuf.
char msgBuf[VW_MAX_MESSAGE_LEN];
// Init msgStr.
String msgStr;
// Init hello world count.
unsigned int cnt = 0;
// setup() runs once after reset.
void setup() {
  #ifdef DEBUG
    // Init serial for debuggin.
    Serial.begin(9600);
  #endif
  // Init BPS.
  vw_setup(2000);
  // Send setup() message.
  msgStr = "REMOTE1: setup()!";
  #ifdef DEBUG
    Serial.println(msgStr);
  #endif
  TXmsgStr(msgStr);
}
// loop() runs continuously after setup()
void loop() {
  // Init msg buffer
  msgStr = "REMOTE1: Hello World!  Count=" + String(cnt++);
  #ifdef DEBUG
    Serial.println(msgStr);
  #endif
  TXmsgStr(msgStr);
}
// Transmit msgBuf
void TXmsgStr(String msgStr) {
  msgStr.toCharArray(msgBuf, VW_MAX_MESSAGE_LEN);
  uint8_t msgBufLen = strlen(msgBuf);
  digitalWrite(13, true); // Flash a light to show transmitting
  vw_send((uint8_t *)msgBuf, strlen(msgBuf));
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(13, false);
  delay(200);
}
