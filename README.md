# Car-Batt
A monitor for a vehicle's 12V lead-acid starter battery

Currently this is using the ESP32's built-in ADC which is only 12-bit and has a non-linear response. For this reason, the sketch includes a calibration polynomial which must be derived by measurement for each device. This is not ideal, but was an interesting learning-curve and was definitely a bit sciencey!!

The ADC will max out to 4095 at 3.3V so a potential divider is required to avoid damaging the chip, AND to be able to make useful measurements!
I used 30 kΩ and 7.5kΩ resistors in my divider which gives a 4:1 ratio, along with a bit of voltage headroom up to 16.5 V
