/*

 
 */



#include <Servo.h>
#include <uSTimer2.h>
#include <CharliePlexM.h>
#include <Wire.h>
#include <I2CEncoder.h>


Servo frontMotor;
Servo backMotor;
Servo leftMotor;
Servo rightMotor;
Servo clawMotor;

I2CEncoder encoder_FrontMotor;
I2CEncoder encoder_BackMoror;
I2CEncoder encoder_LeftMotor;
I2CEncoder encoder_RightMotor;

boolean bt_MotorsEnabled = true;

//pins, pin numbers will change once we know everything that needs a pin
const int ci_LeftUltraPing = 2;   //input plug
const int ci_LeftUltraData = 3;   //output plug
const int ci_RightUltraPing ;
const int ci_RightUltraData; //can they have the same input pin since we are only reading one at a time??
const int ci_CharlieplexLED1 = 4; //will we use these? if so we dont have enough pins i think...
const int ci_CharlieplexLED2 = 5;
const int ci_CharlieplexLED3 = 6;
const int ci_CharlieplexLED4 = 7;
const int ci_ModeButton = 7;
const int ci_FrontMotor = 8;
const int ci_BackMotor = 9;
const int ci_LeftMotor = 10;
const int ci_RightMotor = 11;
const int ci_ClawMotor = 12;
const int ci_MotorEnableSwitch = 13;//dont really wanna use this pin if possible i think 


const int ci_I2C_SDA = A4;         // I2C data = white
const int ci_I2C_SCL = A5;         // I2C clock = yellow

//constant values
const int ci_motorStop = 1500;
const int ci_ClawOpen = 180;
const int ci_ClawClosed = 0;

//variables
unsigned int frontMotorSpeed;
unsigned int backMotorSpeed;
unsigned int leftMotorSpeed;
unsigned int rightMotorSpeed;
unsigned long frontMotorPos;
unsigned long backMotorPos;
unsigned long leftMotorPos;
unsigned long rightMotorPos; 

unsigned long echoTime;
unsigned long leftEchoTime;
unsigned long rightEchoTime;

unsigned int modeIndex=0;
unsigned int stageIndex;



void setup()
{
  Wire.begin();
  Serial.begin(9600);


  //set up ultrasonic
  pinMode(ci_LeftUltraPing, OUTPUT);
  pinMode(ci_LeftUltraData, INPUT);
  pinMode(ci_RightUltraPing, OUTPUT);
  pinMode(ci_RightUltraData, INPUT);

  // set up drive motors
  pinMode(ci_FrontMotor, OUTPUT);
  frontMotor.attach(ci_FrontMotor);
  pinMode(ci_BackMotor, OUTPUT);
  backMotor.attach(ci_BackMotor);
  pinMode(ci_LeftMotor, OUTPUT);
  leftMotor.attach(ci_LeftMotor);
  pinMode(ci_RightMotor, OUTPUT);
  rightMotor.attach(ci_RighMotor);

  // set up arm motors
  pinMode(ci_ClawMotor, OUTPUT);
  clawMotor.attach(ci_ClawMotor);
  clawMotor.write(ci_ClawOpen);

  // set up motor enable switch
  pinMode(ci_MotorEnableSwitch, INPUT);

  //have to initiate I2C motors in the order they are attached starting at the Aurdino

}


void loop()
{


  // modes 
  // 0 = default after power up/reset
  // 1 = Press mode button once to enter. Run robot.
  // 2 = Press mode button twice to enter. 
  // 3 = Press mode button three times to enter.
  // 4 = Press mode button four times to enter.
  switch(ui_Robot_State_Index)
  {
  case 0:    //Robot stopped
    {
      break;
    } 

  case 1:    //Robot Run after 3 seconds
    {
      break;
    } 

  case 2:    //Calibrate line tracker light levels after 3 seconds
    {
      break;
    }

  case 3:    // Calibrate line tracker dark levels after 3 seconds
    {
      break;
    }

  case 4:    //Calibrate motor straightness after 3 seconds.
    {
    }
  }
}

//**************end of void loop() *****************************************************************


//measure distance to target using ultrasonic sensor
void Ping(char side)
{
  if(side==L){
  //Ping Ultrasonic
  //Send the Ultrasonic Range Finder a 10 microsecond pulse per tech spec
  digitalWrite(ci_LeftUltraPing, HIGH);
  delayMicroseconds(10);  //The 10 microsecond pause where the pulse in "high"
  digitalWrite(ci_LeftUltraPing, LOW);
  //use command pulseIn to listen to Ultrasonic_Data pin to record the
  //time that it takes from when the Pin goes HIGH until it goes LOW 
  leftEchoTime = pulseIn(ci_LeftUltraData, HIGH, 10000);
  }
  else if(side==R){
  digitalWrite(ci_RightUltraPing, HIGH);
  delayMicroseconds(10);
  digitalWrite(ci_RightUltraPing, LOW);
  rightEchoTime = pulseIn(ci_RightUltraData, HIGH, 10000);
  }
  

  // Print Sensor Readings
#ifdef DEBUG_ULTRASONIC
  if(side==L) echoTime = leftEchoTime;
  else if(side==R) echoTime = rightEchoTime;
 
  Serial.print("Side: ");
  Serial.print(side);
  Serial.print(", Time (microseconds): ");
  Serial.print(echoTime, DEC);
  Serial.print(", Inches: ");
  Serial.print(echoTime/148); //divide time by 148 to get distance in inches, gonna have to play around with the number
  Serial.print(", cm: ");
  Serial.println(echoTime/58); //divide time by 58 to get distance in cm 
#endif
}


