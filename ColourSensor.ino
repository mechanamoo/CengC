#include <Wire.h>
#include "Adafruit_TCS34725.h"
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_1X);


void setup() {
  Serial.begin(9600);
}

void loop() {
  ColourSensor();
  Serial.println(ColourSensor());
}

int ColourSensor() {
  uint16_t r, g, b, c, colorTemp, lux;                            
  tcs.getRawData(&r, &g, &b, &c);                               //get RGB and clear colour data from sensor
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);   //calculate colour temperature from data
  lux = tcs.calculateLux(r, g, b);                              //calculate lux from RGB values
  Serial.print("Lux: ");
  Serial.println(lux);
  Serial.print("colorTemp: ");
  Serial.println(colorTemp);
  //if((lux>610) && (lux<800)) {                                  //use experimentally determined ranges of lux and colour temperature to differentiate copper or silver
  //  if((colorTemp>2880) && (colorTemp<3200)) {
  //    return 1;                                                 //return 1 for copper
  //  }
  //  else {
  //    return 0;                                                 //return 0 for neither copper or silver (no marble or can't identify)
  //  }
  //}
  if((lux>575) && (lux<690)) {
    if((colorTemp>3700) && (colorTemp<4000)) {
      return 2;                                                 //return 2 for silver
    }
    else {
      return 0;
    }
  }
  else {
    return 0;
  }
}
