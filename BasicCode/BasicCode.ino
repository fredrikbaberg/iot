// Basic code with WiFi, NTP and a simple webserver.
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

// For WiFi
WiFiManager wifiManager;
// For NTP.
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
// For Webserver
WiFiServer server(80);

void setup() {
  // put your setup code here, to run once:
  setupSerial();
  setupWiFi();
  setupNTP();
  setupWebServer();
  Serial.println("Should now be setup...");
}

void setupSerial(){
  // Setup serial port for debugging.
  Serial.begin(115200);
  Serial.println();
}

void setupWiFi(){
  // Connect to WiFi, and print IP on serial port when done.
  if(!wifiManager.autoConnect()){
    Serial.println("Entered config mode");
    Serial.println(WiFi.softAPIP());
    Serial.println(wifiManager.getConfigPortalSSID());
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void setupNTP(){
  timeClient.begin();
  timeClient.setTimeOffset(7200);
  timeClient.update();
  Serial.println("Time: " + timeClient.getFormattedTime());
}

void setupWebServer(){
  server.begin();
  Serial.println("Started webserver");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(WiFi.localIP());
  Serial.println(timeClient.getFormattedTime());
  delay(2000);
}
