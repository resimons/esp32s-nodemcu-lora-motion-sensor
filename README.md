# Connect a keystudio PIR motion sensor KS0052 to a ESP 32S with LoRa SX1278 Module RA-02

The ESP will read the data from the sensor and puts it on the LCD display.

## Pin layout ESP32

![image](https://github.com/resimons/esp32s-nodemcu-lora-motion-sensor/blob/main/images/esp32-esp-32s-nodemcu-pinout.jpg)
![image](https://www.tinytronics.nl/image/cache/catalog/products_2024/keyestudio-pir-motion-sensor-main-1500x1500.jpg)

## Where to buy

[ESP-32S](https://elektronicavoorjou.nl/product/esp32-development-board-wifi-bluetooth)
[KS0052](https://www.tinytronics.nl/nl/sensoren/beweging/keyestudio-pir-bewegingssensor-module)

## How to connect KS0052 to ESP32
COMPONENT | COLOUR | ESP
------------ | ---------- | -------------
VCC | RED | 3.3V / 5V
GND | BLACK | GND
S (signal) | YELLOW | GPIO35

[Wiki KS0052](https://wiki.keyestudio.com/Ks0052_keyestudio_PIR_Motion_Sensor)

# Pinlayout LoRa SX1278 Module ra-02

![image](https://images.tcdn.com.br/img/img_prod/557243/sx1278_lora_433mhz_ra_02_breakout_board_10km_959_1_20191128221303.png)

## Wiring LoRa SX1278 module with ESP32
SX1278 | COLOR | ESP32
-------- |-| ----------
3.3V | RED | 3.3V
GND | BLACK | GND
SCK | ORANGE | GPIO18
MOSI | WHITE | GPIO23
DIO0 | BROWN | GPIO4
NSS | PINK | GPIO5
MISO | GREY | GPIO19
RST | YELLOW | GPIO2

[Wiring and more](https://www.circuitstate.com/tutorials/interfacing-ra-01-ra-02-sx1278-lora-modules-with-esp32-using-arduino/)
[Sample code](https://how2electronics.com/esp32-lora-sx1278-76-transmitter-receiver/)
