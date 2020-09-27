# WLAN Controlled Heater Thermostat for Tasmota Switch

This is an ESP8266 based WLAN Controlled Heater Thermostat for a Tasmota switch.

- Uses Wemos D1 mini (ESP12) to measure temperature with DS1820 sensor.
- Controls temperature by sending On/Off commands to Tasmota switch.
- The DS1820 sensor is the only external element necessary.
- Connect to 3,3 V, GND and D2. Internal ESP pullup resistor is used.
- Can act as WLAN Client or AP (set the AP constant accordingly).
- Stores status (Auto) and temperature level in EEPROM and reloads after boot.

In addition to the Wemos D1 mini you need a Tasmota compatible switch to switch the heater on and off. See Tasmota (https://tasmota.github.io/docs/) pages for supported devices (e.g. Sonoff devices with Tasmota firmware).

Here I use the OBI2 WLAN switch from a local DIY store.  The WLAN settings and the Tasmota address have to be changed in the code. On the Tasmota device you have to configure the corresponding WLAN settings.

This is the web page to control the thermostat settings.

You can set the thermostat temperatur an the state (Auto, On, Off).

![Display1](https://github.com/AK-Homberger/WLAN-Controlled-Heater-Thermostat-for-Tasmota-switch/blob/master/Screen.png)

The Wemos D1 mini plus the DS1820 sensor.
![Display2](https://github.com/AK-Homberger/WLAN-Controlled-Heater-Thermostat-for-Tasmota-switch/blob/master/Wemos%20D1%20Mini-1.jpg)

The OBI2 device is placed directly in the heater connected with WAGO connectors.
![Display3](https://github.com/AK-Homberger/WLAN-Controlled-Heater-Thermostat-for-Tasmota-switch/blob/master/TasmotaObi2-1.jpg)

This is the heater itself.
![Display4](https://github.com/AK-Homberger/WLAN-Controlled-Heater-Thermostat-for-Tasmota-switch/blob/master/Heater-1.jpg)

