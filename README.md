# WLAN Controlled Heater Thermostat for Tasmota Switch

This is an ESP8266 based WLAN controlled heater thermostat for a Tasmota switch.

- Uses D1 mini (ESP12) to measure temperature with DS1820 sensor.
- Controls temperature by sending On/Off commands to Tasmota switch.
- The DS1820 sensor is the only external element necessary.
- Connect to 3,3 V to VDD, GND to GND and D2 to DQ. The internal ESP pullup resistor is used. ![DS1820](https://github.com/AK-Homberger/WLAN-Controlled-Heater-Thermostat-for-Tasmota-switch/blob/master/DS1820.png)
- Can act as WLAN Client or AP (set the AP constant accordingly).
- Stores status (Auto) and temperature level in EEPROM and reloads after boot.

In addition to the D1 mini you need a Tasmota compatible switch to switch the heater on and off. See Tasmota (https://tasmota.github.io/docs/) pages for supported devices. This device has Tasmota firmware already flashed: https://templates.blakadder.com/delock_11826.html

For configuration you can change the following settings in the code:

```cpp
#define AP 1             // Set to 1 to act as Access Point, 0 as client 
#define Correction -7    // Defines correction value for temperature sensor
const char *TasmotaIP = "192.168.4.20";    // Defines address of Tasmota switch
const char *ssid      = "MyHeaterControl"; // Set WLAN name
const char *password  = "password";        // Set password
```
The OneWire and the DallasTemperature libraries have to be installed with the library manager.

As Tasmota switch I use the OBI2 WLAN switch from a local DIY store (https://tasmota.github.io/docs/devices/OBI-Socket-2/).  The WLAN settings and the Tasmota address have to be changed in the code. On the Tasmota device you have to configure the corresponding WLAN settings.

This is the web page to control the thermostat settings:

![Display1](https://github.com/AK-Homberger/WLAN-Controlled-Heater-Thermostat-for-Tasmota-switch/blob/master/Screen.png)

You can set the thermostat temperature and the state (Auto, On, Off).


The D1 mini plus the DS1820 sensor:

![Display2](https://github.com/AK-Homberger/WLAN-Controlled-Heater-Thermostat-for-Tasmota-switch/blob/master/Wemos%20D1%20Mini-1.jpg)

Here the OBI2 device is placed directly in the heater connected with WAGO connectors. But of course you can also use the standard socket instead or any other Tasmota switch/socket.

![Display3](https://github.com/AK-Homberger/WLAN-Controlled-Heater-Thermostat-for-Tasmota-switch/blob/master/TasmotaObi2-1.jpg)

This is the heater itself:

![Display4](https://github.com/AK-Homberger/WLAN-Controlled-Heater-Thermostat-for-Tasmota-switch/blob/master/Heater-1.jpg)

# Partlist
- DS1820 [Link](https://www.reichelt.de/de/en/digital-thermometer-1-wire--0-5-c-to-92-ds-18b20-p58169.html?&nbc=1)
- D1 Mini [Link](https://www.reichelt.de/de/en/d1-mini-esp8266-v3-0-d1-mini-p253978.html?&nbc=1)
- Socket switch [Link](https://templates.blakadder.com/delock_11826.html)
- Heater [Link](https://www.bauhaus.info/heizluefter/voltomat-heating-infrarot-heizstrahler/p/21324817)



