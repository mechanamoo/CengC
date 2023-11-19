#include <AFMotor.h>
#include <ezButton.h>
#include <Servo.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

#define SERVO1PIN 10
#define DUMPSTERPORT 1  //Motor for picking up 
#define AGITATORPORT 4  //Motor for agitating balls
#define BUTTONPIN 2

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_1X);
AF_DCMotor dumpMotor(DUMPSTERPORT, MOTOR12_64KHZ);
AF_DCMotor agiMotor(AGITATORPORT, MOTOR12_64KHZ);
ezButton button(BUTTONPIN);
Servo sortServ;

int colour = 0; //Store most recently read colour
//int resetCounter = 0; //Counts attempts to read colour
int timer = 0;  //Count time that has passed

void setup() {
  sortServ.attach(SERVO1PIN);
  sortServ.write(90);

  dumpMotor.setSpeed(255);
  agiMotor.setSpeed(80);
  dumpMotor.run(RELEASE);
  agiMotor.run(FORWARD);

  button.setDebounceTime(50);

  Serial.begin(9600);
}





void loop() {
  //if (button.isPressed()){
    Serial.print("hi");
    delay(500);
    timer = 0;

    //=======StartUp========
    dumpMotor.run(FORWARD);

    //Wait for limit to be hit
    delay(500);
    dumpMotor.setSpeed(0);  //Hits upper limit
    dumpMotor.run(BACKWARD);
    delay(3000);  //Wait for balls to fall out
    dumpMotor.setSpeed(150);

    //Wait for limit to be hit
    delay(500);
    dumpMotor.setSpeed(0);    //Hits lower limit
    

    //=========Sorting==============
    timer = 0;
    while (1){
      if (timer>5000){  //Occasionally run the agitator
        run_agitator();
        timer = 0;
      }
      timer+=700;

      delay(3000); //wait for balls to settle
      colour = get_colour();  //Read colour
      if (colour == 2){  //Copper or unknown
        //move, wait, return
        //resetCounter = 0;
        sortServ.write(60);
        delay(200);
        sortServ.write(90);
      }else{  //Silver
        //move, wait, return
        //resetCounter = 0;
        sortServ.write(120);
        delay(200);
        sortServ.write(90);
      }/*else if(resetCounter>5){ //Assume copper if it can't be read correctly
        resetCounter = 0;
        sortServ.write(60);
        delay(200);
        sortServ.write(90);
      }else{  //Unknown
        resetCounter+=1;
        delay(200);
      }*/
    }
  //}
}

void run_agitator(){ //Move them balls girllllll
  agiMotor.setSpeed(150);
  delay(150);
  agiMotor.run(BACKWARD);
  delay(300);
  agiMotor.run(FORWARD);
  delay(200);
  agiMotor.run(BACKWARD);
  delay(200);
  agiMotor.setSpeed(100);
  delay(100);
  agiMotor.run(FORWARD);
}

int get_colour(){  //Senses the ball colour. Return 0 for unknown, 1 for copper, 2 for silver.
  uint16_t r, g, b, c, colorTemp, lux;                            
  tcs.getRawData(&r, &g, &b, &c);                               //get RGB and clear colour data from sensor
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);   //calculate colour temperature from data
  lux = tcs.calculateLux(r, g, b);                              //calculate lux from RGB values
  Serial.print("Lux: ");
  Serial.println(lux);
  Serial.print("colorTemp: ");
  Serial.println(colorTemp);
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