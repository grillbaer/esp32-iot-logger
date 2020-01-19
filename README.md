# IoT data logger with Thingspeak Channel

Currently used to measure and log a solar panel's input voltage, current and power and a battery's input voltage, current and power.

Uses a LILYGO ESP32 OLED (SSD1306 at I2C pins SCK=4 and SDA=5) and two ADS1115 ADC converters at I2C pins SCK=25 and SDA=26.

TODO:
* extract config (pins, ...)
* doc