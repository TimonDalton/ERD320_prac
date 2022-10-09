// #include "hardware/adc.h"
#include <U8x8lib.h>
#include <Wire.h>
#include <SPI.h>
// #define _USE_MATH_DEFINES
#include <math.h>
#include <arduino.h>
// #define PI 3.1415926535897932384626433832795

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);

  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);//Small Font 16 Characters
  u8x8.noInverse();//Normal
  u8x8.clear();
  u8x8.setCursor(5,0); //horisontal, vertical
  u8x8.print("STARTUP");


}

void loop(){}