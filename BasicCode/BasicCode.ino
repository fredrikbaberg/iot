// Basic code with WiFi, NTP and a simple webserver. To be extended with required functionalities.
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
  setupSerial(); // Setup serial port for debugging.
  setupWiFi(); // Setup WiFi for communication.
  setupNTP(); // Setup NTP for correct date/time.
  setupWebServer(); // Setup webserver for displaying data.
  Serial.println("Should now be setup...");
}

void setupSerial(){
  // Setup serial port for debugging.
  Serial.begin(115200);
  Serial.println();
}

void setupWiFi(){
  // Connect to WiFi, print IP on serial port when done.
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
  // Setup NTP with GMT+2 for Stockholm time.
  timeClient.begin();
  timeClient.setTimeOffset(7200);
  timeClient.update();
  Serial.println("Time: " + timeClient.getFormattedTime());
}

void setupWebServer(){
  // Setup webserver.
  server.begin();
  Serial.println("Started webserver.");
}

String prepareHtmlPage(){
  String htmlPage =
    String("HTTP/1.1 200 OK\r\n") + "Content-Type: text/html\r\n" +
           "Connection: close\r\n" +  // the connection will be closed after completion of the response
           "Refresh: 5\r\n" +  // refresh the page automatically every 5 sec
           "\r\n" + "<!DOCTYPE HTML>" + "<html>" +
           "Time: " + String(timeClient.getFormattedTime()) + + "<br>" +
           "Analog input:  " + String(analogRead(A0)) +
           "</html>" + "\r\n";
  return htmlPage;
}

void serverLoop(){
  WiFiClient client = server.available();
  // wait for a client (web browser) to connect
  if (client){
    Serial.println("\n[Client connected]");
    while (client.connected()){
      // read line by line what the client (web browser) is requesting
      if (client.available()){
        String line = client.readStringUntil('\r');
        Serial.print(line);
        // wait for end of client's request, that is marked with an empty line
        if (line.length() == 1 && line[0] == '\n'){
          client.println(prepareHtmlPage());
          break;
        }
      }
    }
    delay(1); // give the web browser time to receive the data
    client.stop(); // close the connection
    Serial.println("[Client disonnected]");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(WiFi.localIP());
  Serial.println(timeClient.getFormattedTime());
  serverLoop();
  delay(2000);
}
