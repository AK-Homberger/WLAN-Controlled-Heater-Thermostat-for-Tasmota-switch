/*
  This code is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This code is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

// WLAN Controlled Heater Switch Thermostat (for Tasmota switch)
//   - Uses Wemos D1 mini (ESP12) to measure temperature with DS18B20 sensor
//   - Controls temperature by sending On/Off commands to Tasmota switch
//   - The DS18B20 is the only external element necessary
//   - Connect to 3,3 V, GND and D2. Internal ESP pullup resistor is used!
//   - Can act as WLAN Client or AP
//   - Stores status (Auto) and temperature level in EEPROM and reloads after boot

// Version 1.0, 27.09.2020, AK-Homberger

#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include "index_html.h"  // Web site information 


// Change these values only
// ------------------------------------------------------------------------
#define AP 1             // Set to 1 to act as Access Point, 0 as client 
#define Correction -7    // Defines corection value for temperature sensor
const char *TasmotaIP = "192.168.4.20";    // Defines address of Tasmota switch
const char *ssid      = "MyHeaterControl"; // Set WLAN name
const char *password  = "password";        // Set password
// -------------------------------------------------------------------------

#define On true
#define Off false

// Data wire is plugged into port D2 on the Arduino
#define ONE_WIRE_BUS D2
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

float TempLevel = 15;     // Initial temperature level. Current value is stored in EEPROM
float Temp = 0;           // Current temperature

bool OnOff = false;
bool Error = false;
bool Auto = false;
bool update = false;
bool LedState = LOW;

unsigned long t, t2;
boolean wifiConnected = false;

WiFiClient client;
ESP8266WebServer server(80);                // Web Server at port 80


void setup(void) {
  Serial.begin(115200);

  pinMode(D4, OUTPUT);
  digitalWrite(D4, LedState);               // Internal LED on

  // Start up the library
  pinMode(ONE_WIRE_BUS, INPUT_PULLUP);      // We use internal pullup for DS18B20. Works only for one sensor!
  sensors.begin();

  EEPROM.begin(16);                         // Initialise non volatile storage (16 byte is minimum)
  TempLevel = EEPROM.read(0) / 2.0;         // Read temp level from NVS (0.5 °C resulution)
  Auto = EEPROM.read(1);                    // Read state from NVS

  // Initialise wifi connection

  wifiConnected = connectWifi();            // Connect to WLAN or start AP

  if (!wifiConnected) {
    Serial.println("Cannot connect to WiFi. Please check data. Reset the ESP now.");
    delay(3000);
    ESP.restart();
  }

  // Define HTTP request events and handlers

  server.on("/", Event_Index);
  server.on("/temp", Event_Temperature);
  server.on("/up", Event_Up);
  server.on("/down", Event_Down);
  server.on("/auto", Event_Auto);
  server.on("/on", Event_On);
  server.on("/off", Event_Off);
  server.on("/slider", Event_Slider);
  
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP Server started");
}


void Event_Index() {                     // If "http://<ip address>/" requested
  server.send(200, "text/html", MAIN_page);  // Send Index Website (defined in index_html.h)
}


void Event_Up() {                       // Handle UP button
  Serial.println("Up");
  TempLevel += 0.5;
  if (TempLevel > 30 ) TempLevel = 30;
  update = true;
  server.send(200, "text/html");
}


void Event_Down() {                     // Handle Down button
  Serial.println("Down");
  TempLevel -= 0.5;
  if (TempLevel < 0) TempLevel = 0;
  update = true;
  server.send(200, "text/html");
}


void Event_Auto() {                      // Handle Auto button
  Serial.println("Auto");
  Auto = true;
  update = true;
  server.send(200, "text/html");
}


void Event_On() {                       // Handle On button
  Serial.println("On");
  SwitchTasmota(On);
  Auto = false;
  OnOff = On;
  update = true;
  server.send(200, "text/html");
}


void Event_Off() {                      // Handle Off button
  Serial.println("Off");
  SwitchTasmota(Off);
  Auto = false;
  OnOff = Off;
  update = true;
  server.send(200, "text/html");
}


void Event_Slider() {                   // Handle slider event
  Serial.print("Slider:");
  if (server.args() > 0) {
    TempLevel = server.arg(0).toFloat();
    Serial.println(TempLevel);
  }
  server.send(200, "text/html");
  update = true;
}


void Event_Temperature() {               // Send temperature, level and status as JSON string
  String Text, State;
  char buf[20];

  StaticJsonDocument<100> root;

  if (Auto) State = "Auto";
  else {
    if (OnOff) State = "On"; else State = "Off";
  }

  if (Error) State = "Tasmota Error";

  snprintf(buf, sizeof(buf), "%2.2f", Temp);
  root["temp"] = buf;

  snprintf(buf, sizeof(buf), "%2.2f", TempLevel);
  root["level"] = buf;

  root["status"] = State;
    
  serializeJsonPretty(root, Text);
  server.send(200, "text/plain", Text); //Send sensors values to client ajax request
}

void handleNotFound() {                                           // Unknown request. Send error 404
  server.send(404, "text/plain", "File Not Found\n\n");
}


// Switch Tasmota device On/Off

void  SwitchTasmota(bool OnOff) {      
  WiFiClient client;
  char command[50];

  client.setTimeout(1000);              // Set timeout to 1 second
  if (!client.connect(TasmotaIP, 80)) { // Check if device is available
    Error = true;
    return;                             // Not available -> return
  }
  client.stop();

  HTTPClient http;                      // Generate http client and send command

  if (OnOff) snprintf(command, sizeof(command), "http://%s/cm?cmnd=Power%%20On", TasmotaIP);
  else snprintf(command, sizeof(command), "http://%s/cm?cmnd=Power%%20Off", TasmotaIP);

  http.begin(command);
  http.addHeader("Content-Type", "application/json");
  http.GET();
  http.end();
  Error = false;
}


// Connect to wifi or start AP – returns true if successful or false if not

boolean connectWifi() {
  boolean state = true;
  int i = 0;

  if (AP == 0) {
    WiFi.mode(WIFI_STA);
    WiFi.hostname("HeaterControl");
    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi");
    // Wait for connection
    Serial.print("Connecting...");
    // Initialise wifi connection

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      if (i > 10) {
        state = false; break;
      }
      i++;
    }
    Serial.println("");
    if (state) {
      Serial.print("Connected to ");
      Serial.println(ssid);
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
    }
    else {
      Serial.println("Connection failed.");
    }
    return state;
  }  else {

    Serial.println("Start WLAN AP");         // WiFi Mode AP
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    delay(100);
    IPAddress IP = WiFi.softAPIP();
    Serial.println("");
    Serial.print("AP IP address: ");
    Serial.println(IP);
    return true;
  }
}


// Check WiFi connection

void Check_WiFi(void) {
  boolean wifiConnected = false;

  if (AP != 0) return;   // We are acting as AP. Connection check not necessary

  if (WiFi.status() != WL_CONNECTED) { // Wifi restart
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    WiFi.mode(WIFI_STA);

    Serial.println("Reconnect!");
    wifiConnected = connectWifi();

    if (!wifiConnected) {
      Serial.println("Cannot connect to WiFi. Please check data. Reset the ESP now.");
      delay(2500);
      ESP.restart();
    }
  }
}


void loop(void) {

  server.handleClient();        // Handle web server events

  if (millis() > t + 2000) {    // Every two seconds store changes values and read temperature
    t = millis();

    if (update) {               // Store values if changed
      update = false;
      Serial.println("Eprom write");
      EEPROM.write(0, TempLevel * 2.0);
      EEPROM.write(1, Auto);
      EEPROM.commit();
    }

    digitalWrite(D4, LedState); // Toggle internal LED state
    LedState = !LedState;

    sensors.requestTemperatures(); // Send the command to get temperatures
    Temp = sensors.getTempCByIndex(0) + Correction;

    Serial.print("Temp=");
    Serial.println(Temp);

    if (Auto == true ) {           // Switch On/Off depending on temperature

      if ((OnOff == Off) && (Temp < TempLevel)) {
        SwitchTasmota(On);
        OnOff = On;
      }

      if ((OnOff == On) && (Temp > TempLevel + 0.5)) {
        SwitchTasmota(Off);
        OnOff = Off;
      }
    }
  }

  if ((millis() > t2 + 30000)) {  // Every 30 seconds switch On/Off again
    t2 = millis();

    if (Auto == true ) {

      if (Temp < TempLevel) {
        SwitchTasmota(On);
        OnOff = On;
      }

      if (Temp > TempLevel + 0.5) {
        SwitchTasmota(Off);
        OnOff = Off;
      }
    }
  }
  Check_WiFi();
}
