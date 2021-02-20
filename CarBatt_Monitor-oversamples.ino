
/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP32 chip.

  Note: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32

  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 ************************************************************
 
 :: Car Battery Monitor v1.0
 :: by
 :: @CraigMarston
 :: This uses the ESP32's ADC which is only 12-bit
 :: and is not very linear, hence the polynomial
 :: which is derived from actual measurements taken.
 :: A lead-acid car battery is 100% charged at 12.6 V
 :: it is 75% charged at 12.4 V
 :: it is 50% charged at 12.2 V
 :: and at 12.0V it is only 25% charged and probably knackered!!
 
 */


// Node 32s board, Hold down the boot button!!


/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ThingSpeak.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "-------------"; // Voltage Monitor - Craig's Blynk

//Thing Speak
unsigned long myChannelNumber = ----------;
const char * myWriteAPIKey = "-------------";

// Your WiFi credentials.

char ssid[] = "---------";
char pass[] = "----------";

uint16_t ADC_val0;
uint16_t ADC_val1;
uint16_t ADC_val2;
uint16_t ADC_val3;
uint16_t ADC_val4;
uint16_t ADC_val5;
uint16_t ADC_val6;
uint16_t ADC_val7;
uint16_t ADC_val8;
uint16_t ADC_val9;
uint16_t ADC_val_mean;


const float coeffA = -9.433922E-7; // these coefficients are
const float coeffB =  9.282729E-3; // worked out from calibration data
const float coeffC = -6.640744E0; // using https://arachnoid.com/polysolve/ 2-degree!

float BattVolts;

const uint8_t sleepyTime = 30; // ------------------------::--::--::--> sleep duration in mins, max 70
const uint32_t time_in_us = (sleepyTime * 60000000); // multiple of microseconds to seconds

WiFiClient  client;


void setup()
{
  //Debug console
  Serial.begin(115200);
  pinMode(2, OUTPUT);    // sets the digital pin 2 as output
  digitalWrite(2, HIGH); // puts the LED on so we can see activity
  Blynk.begin(auth, ssid, pass);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  
  esp_sleep_enable_timer_wakeup(time_in_us); // limited to 70 mins due to variable maxing out

  Blynk.run(); // all the Blynk magic happens here
  
  ADC_val0 = analogRead(32); //reading the sensor on ...
  delay(333);
  ADC_val1 = analogRead(32);
  delay(333);
  ADC_val2 = analogRead(32);
  delay(333);
  ADC_val3 = analogRead(32);
  delay(333);
  ADC_val4 = analogRead(32);
  delay(333);
  ADC_val5 = analogRead(32);
  delay(333);
  ADC_val6 = analogRead(32);
  delay(333);
  ADC_val7 = analogRead(32);
  delay(333);
  ADC_val8 = analogRead(32);
  delay(333);
  ADC_val9 = analogRead(32);
  ADC_val_mean = (ADC_val0+ADC_val1+ADC_val2+ADC_val3+ADC_val4+ADC_val5+ADC_val6+ADC_val7+ADC_val8+ADC_val9) / 10 ;

  
  // f(x) = Ax^2 + Bx + C the polynomial equation for calibration
  BattVolts = (coeffA*sq(ADC_val_mean)) + (coeffB*ADC_val_mean) + coeffC ; 

  Blynk.virtualWrite(V5, ADC_val_mean); //sending to Blynk
  Blynk.virtualWrite(V6, BattVolts);    // conversion to voltage
  
  Serial.print("ADC: ");
  Serial.println(ADC_val_mean); 
  Serial.print("V = ");
  Serial.println(BattVolts);
;

  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different

  //setField (field, value)

  ThingSpeak.setField (1, ADC_val_mean);
  ThingSpeak.setField (2, BattVolts); // disable while calibrating

  //writeFields (channelNumber, writeAPIKey)

  ThingSpeak.writeFields (myChannelNumber, myWriteAPIKey); 
  digitalWrite(2, LOW);
  
  esp_deep_sleep_start(); 
}

void loop()
{
  // nowt happens here!
}
