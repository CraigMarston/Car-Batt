# Car-Batt
A monitor for a vehicle's 12V lead-acid starter battery

12.6 V = 100%
12.4 V =  75%
12.2 V =  50%
12.0 V =  new battery required!!

Currently this is using the ESP32's built-in ADC which is only 12-bit and has a non-linear response. For this reason, the sketch includes a calibration polynomial (which I generated here; https://arachnoid.com/polysolve/) which must be derived by measurement for each device. This is not ideal, but was an interesting learning-curve and was definitely a bit sciencey!!

The ESP32 goes into a deep sleep periodically to save power. At full whack it'll pull around 200 mA, but for around 20 seconds per waking period. The maximum sleep time is limited by the 32-bit variable holding the number of microseconds; 

2^32 ÷ 60 µs ≈ 71 minutes

The ADC will max out to 4095 at 3.3V so a potential divider is required to avoid damaging the chip, AND to be able to make useful measurements!
I used 30 kΩ and 7.5kΩ resistors in my divider which gives a 4:1 ratio, along with a bit of voltage headroom up to 16.5 V
