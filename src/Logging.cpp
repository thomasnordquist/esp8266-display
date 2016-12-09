#include "Logging.hpp"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <stdio.h>

#define IP "192.168.178.22"
#define PORT 1234
WiFiUDP Udp;

void Logging::setup(){
  Udp.beginPacket(IP, PORT);
  Udp.println("Successfully connected");
  Udp.print("My Address is: ");
  Udp.println(WiFi.localIP());
  Udp.endPacket();
}

void Logging::startStream() {
  Udp.beginPacket(IP, PORT);
}

void Logging::endStream() {
  Udp.endPacket();
  delay(10);
}

void Logging::stream(char* s) {
  Udp.write(s);
}

void Logging::log(char* message) {
  Logging::startStream();
  Logging::stream(message);
  Logging::endStream();
}

void Logging::log(String message) {
  char buffer[128];
  message.toCharArray(buffer, 128);
  Logging::log(buffer);
}
