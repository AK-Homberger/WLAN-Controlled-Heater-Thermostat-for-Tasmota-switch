# WLAN Controlled Heater Switch Thermostat for Tasmota switch

Thi is ESP8266 based WLAN Controlled Heater Switch Thermostat (for Tasmota switch).

- Uses Wemos D1 mini (ESP12) to measure temperature with DS18B20 sensor
- Controls temperature by sending On/Off commands to Tasmota switch
- The DS18B20 is the only external element necessary
- Connect to 3,3 V, GND and D2. Internal ESP pullup resistor is used!
- Can act as WLAN Client or AP
- Stores status (Auto) and temperature level in EEPROM and reloads after boot
