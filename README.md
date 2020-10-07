# WLAN Controlled Heater Thermostat for Tasmota Switch

This is an ESP8266 based WLAN controlled heater thermostat for a Tasmota switch.

- Uses Wemos D1 mini (ESP12) to measure temperature with DS1820 sensor.
- Controls temperature by sending On/Off commands to Tasmota switch.
- The DS1820 sensor is the only external element necessary.
- Connect to 3,3 V to VDD, GND to GND and D2 to DQ. The internal ESP pullup resistor is used. ![DS1820](https://github.com/AK-Homberger/WLAN-Controlled-Heater-Thermostat-for-Tasmota-switch/blob/master/DS1820.png)
- Can act as WLAN Client or AP (set the AP constant accordingly).
- Stores status (Auto) and temperature level in EEPROM and reloads after boot.

In addition to the Wemos D1 mini you need a Tasmota compatible switch to switch the heater on and off. See Tasmota (https://tasmota.github.io/docs/) pages for supported devices. This device has Tasmota firmware already flashed: https://templates.blakadder.com/delock_11826.html

For configuration you can change the following settings in the code:

```cpp
#define AP 1             // Set to 1 to act as Access Point, 0 as client 
#define Correction -7    // Defines corection value for temperature sensor
const char *TasmotaIP = "192.168.4.20";    // Defines address of Tasmota switch
const char *ssid      = "MyHeaterControl"; // Set WLAN name
const char *password  = "password";        // Set password
```
The OneWire and the DallasTemperature libraries have to be installed with the library manager.

As Tasmota switch I use the OBI2 WLAN switch from a local DIY store (https://tasmota.github.io/docs/devices/OBI-Socket-2/).  The WLAN settings and the Tasmota address have to be changed in the code. On the Tasmota device you have to configure the corresponding WLAN settings.

This is the web page to control the thermostat settings:

![Display1](https://github.com/AK-Homberger/WLAN-Controlled-Heater-Thermostat-for-Tasmota-switch/blob/master/Screen.png)

You can set the thermostat temperature and the state (Auto, On, Off).


The Wemos D1 mini plus the DS1820 sensor:

![Display2](https://github.com/AK-Homberger/WLAN-Controlled-Heater-Thermostat-for-Tasmota-switch/blob/master/Wemos%20D1%20Mini-1.jpg)

Here the OBI2 device is placed directly in the heater connected with WAGO connectors. But of course you can also use the standard socket instead or any other Tasmota switch/socket.

![Display3](https://github.com/AK-Homberger/WLAN-Controlled-Heater-Thermostat-for-Tasmota-switch/blob/master/TasmotaObi2-1.jpg)

This is the heater itself:

![Display4](https://github.com/AK-Homberger/WLAN-Controlled-Heater-Thermostat-for-Tasmota-switch/blob/master/Heater-1.jpg)

# Partlist
- DS1820 [Link](https://www.amazon.de/AZDelivery-%E2%AD%90%E2%AD%90%E2%AD%90%E2%AD%90%E2%AD%90-DS18B20-digitaler-Temperatursensor/dp/B01LZCZOJJ/ref=sr_1_3?adgrpid=72630429073&dchild=1&gclid=EAIaIQobChMIzYnyqouO7AIVBfhRCh0rXAqBEAAYAiAAEgIuVfD_BwE&hvadid=353030382657&hvdev=c&hvlocphy=9043858&hvnetw=g&hvqmt=e&hvrand=2376973298400010304&hvtargid=kwd-301195599018&hydadcr=29223_1661108&keywords=ds1820&qid=1601373046&sr=8-3&tag=googhydr08-21&th=1)
- D1 Mini [Link](https://www.amazon.de/AZDelivery-D1-Mini-Entwicklungsboard-kompatibel/dp/B0754N794H/ref=sxts_sxwds-bia-wc-drs1_0?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&cv_ct_cx=wemos+d1+mini&dchild=1&keywords=wemos+d1+mini&pd_rd_i=B0754N794H&pd_rd_r=ed6a8344-b232-431a-9440-fdb14ec37770&pd_rd_w=peIEj&pd_rd_wg=UK5OL&pf_rd_p=578deb70-f9b7-4aa5-9f96-98765f2717c8&pf_rd_r=QV1RZ15XME8GEACPYMCS&psc=1&qid=1601373135&sr=1-1-5a42e879-3844-4142-9c14-e77fe027c877)
- Socket switch [Link](https://templates.blakadder.com/delock_11826.html)
- Heater [Link](https://www.bauhaus.info/heizluefter/voltomat-heating-infrarot-heizstrahler/p/21324817)



