#include <Servo.h>
#include <uSTimer2.h>
#include <CharliePlexM.h>
#include <Wire.h>
#include <I2CEncoder.h>


const int ci_TopLightSensor = A3; //these two are for testing, likely these connections will go on Board one
const int ci_BottomLightSensor = A2;
unsigned int topLightData;
unsigned int bottomLightData;

int time;
int counter = 1;
int currentTime=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ci_TopLightSensor, INPUT);
  pinMode(ci_BottomLightSensor, INPUT);
}

void loop() {

  time = millis();

  if (time-currentTime>2000)
  {
    Serial.println(counter++);
    currentTime=millis();
  }



}
