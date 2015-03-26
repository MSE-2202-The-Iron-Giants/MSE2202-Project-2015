
#include <Servo.h>
#include <uSTimer2.h>
#include <CharliePlexM.h>
#include <Wire.h>
#include <I2CEncoder.h>

#define DEBUG_ULTRASONIC

const int ci_LeftUltraPing = A0;   //input plug yellow wire
const int ci_LeftUltraData = A1;   //output plug orange wire
const int ci_RightUltraPing = A2; //yellow
const int ci_RightUltraData = 2; //orange had to switch this pin so our IR sensor has an analog NEED TO SWITCH ON BOT

unsigned long echoTime; //general echoTime, usefull to have because you can set it equal to L,R,or T as done in ping function
unsigned long leftEchoTime;
unsigned long rightEchoTime;


void Ping();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ci_LeftUltraPing, OUTPUT);
  pinMode(ci_LeftUltraData, INPUT);
  pinMode(ci_RightUltraPing, OUTPUT);
  pinMode(ci_RightUltraData, INPUT);

}

void loop() {
  Ping('L');
  delay(200);
  Ping('R');
  delay(200);

}

void Ping(char side)
{
  if (side == 'L') {
    //Ping Ultrasonic
    //Send the Ultrasonic Range Finder a 10 microsecond pulse per tech spec
    digitalWrite(ci_LeftUltraPing, HIGH);
    delayMicroseconds(10);  //The 10 microsecond pause where the pulse in "high"
    digitalWrite(ci_LeftUltraPing, LOW);
    //use command pulseIn to listen to Ultrasonic_Data pin to record the
    //time that it takes from when the Pin goes HIGH until it goes LOW
    leftEchoTime = pulseIn(ci_LeftUltraData, HIGH, 10000);
  }
  else if (side == 'R') {
    digitalWrite(ci_RightUltraPing, HIGH);
    delayMicroseconds(10);
    digitalWrite(ci_RightUltraPing, LOW);
    rightEchoTime = pulseIn(ci_RightUltraData, HIGH, 10000);
  }


  // Print Sensor Readings
#ifdef DEBUG_ULTRASONIC
  if (side == 'L') echoTime = leftEchoTime;
  else if (side == 'R') echoTime = rightEchoTime;


  Serial.print("Side: ");
  Serial.print(side);
  Serial.print(", Time (microseconds): ");
  Serial.print(echoTime, DEC);
  Serial.print(", Inches: ");
  Serial.print(echoTime / 148); //divide time by 148 to get distance in inches, gonna have to play around with the number
  Serial.print(", cm: ");
  Serial.println(echoTime / 58); //divide time by 58 to get distance in cm
#endif
}
