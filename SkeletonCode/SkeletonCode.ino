/*
make variable that holds value of wheel speed

both us sensors ping and compare and adjust the wheelspeed accordingling to each distance
updates wheelspeed once per loop and else accordinglything roughout the code accordingly ,
or is this not neccessary because we alreadying have functions?




 */

#include <Servo.h>
#include <uSTimer2.h>
#include <CharliePlexM.h>
#include <Wire.h>
#include <I2CEncoder.h>


//*******uncoment to debug******
#define DEBUG_ULTRASONIC
//#define DEBUG_ENCODERS





Servo frontMotor;
Servo backMotor;
Servo leftMotor;
Servo rightMotor;
Servo liftMotor;
Servo armMotor;
Servo clawMotor;
Servo beltMotor;
//add x and y tower motors

I2CEncoder encoder_FrontMotor;
I2CEncoder encoder_BackMotor;
I2CEncoder encoder_LeftMotor;
I2CEncoder encoder_RightMotor;
I2CEncoder encoder_LiftMotor;
//possible have to add more encoders depending on which motors we use for x and y axis

boolean bt_MotorsEnabled = true; //(true = motors turned on)

//pins, pin numbers will change once we know everything that needs a pin

//const int ci_CharlieplexLED1 = 4; //will we use these? if so we dont have enough pins i think...
//const int ci_CharlieplexLED2 = 5;
//const int ci_CharlieplexLED3 = 6;
//const int ci_CharlieplexLED4 = 7;
const int ci_LeftUltraPing = A0;   //input plug yellow wire
const int ci_LeftUltraData = A1;   //output plug orange wire
const int ci_RightUltraPing = A2; //yellow
const int ci_RightUltraData = 2; //orange had to switch this pin so our IR sensor has an analog NEED TO SWITCH ON BOT
const int ci_TopUltraPing = 13; // both topUS pins will go on second board
const int ci_TopUltraData = 13;
const int ci_BeltMotor = 3;
const int ci_LiftMotor = 4;
const int ci_ExtendMotor = 5;
const int ci_ClawMotor = 6;
const int ci_BumperSwitch = 7;
const int ci_FrontMotor = 8;
const int ci_BackMotor = 9;
const int ci_LeftMotor = 10;
const int ci_RightMotor = 11;
const int ci_ModeButton = 12;
const int ci_MotorEnableSwitch = 13; //this will show if motors are enebled or not on pin 13



//const int ci_TopLightSensor = A3; //these two are for testing, likely these connections will go on Board one
//const int ci_BottomLightSensor = A2;
unsigned int topLightData;
unsigned int bottomLightData;

//encoder wires
const int ci_I2C_SDA = A4;         // I2C data = white
const int ci_I2C_SCL = A5;         // I2C clock = yellow

//constant values
const int motorStopSpeed = 1500; //DC motors
const int ci_ClawOpen = 180; //Claw Servo limits
const int ci_ClawClosed = 0;

//variables
unsigned int frontMotorSpeed;
unsigned int backMotorSpeed;
unsigned int leftMotorSpeed;
unsigned int rightMotorSpeed;
//unsigned int motorStopSpeed = 1500;
unsigned long frontMotorPos;
unsigned long backMotorPos;
unsigned long leftMotorPos;
unsigned long rightMotorPos;
unsigned long clawMotorPos;

unsigned long echoTime; //general echoTime, usefull to have because you can set it equal to L,R,or T as done in ping function
unsigned long leftEchoTime;
unsigned long rightEchoTime;
unsigned long topEchoTime;

unsigned int modeIndex = 0;
unsigned int stageIndex = 0;

unsigned long ul_3_S_Timer = 0;

boolean bt_3_S_TimeUp = false;
boolean bt_Heartbeat = true;
boolean bt_DoOnce = false;

//function prototypes
void Drive(char Direction = 'F', int Speed = 300);
void Slide(String Direction = "FL", int Speed = 300);
void Lift(int);
void Extend();
void Ping(char);
void Belt(String mode = "run");

void setup()
{
  Wire.begin();
  Serial.begin(9600);



  //if we wanna use charliplex
  // 2pin=2LED, 3p=6, 4p=12, not sure if need all 4 pins to use button
  //CharliePlexM::setBtn(ci_CharlieplexLED1, ci_CharlieplexLED2,
  //                     ci_CharlieplexLED3, ci_CharlieplexLED4, ci_ModeButton);


  //set up ultrasonic
  pinMode(ci_LeftUltraPing, OUTPUT);
  pinMode(ci_LeftUltraData, INPUT);
  pinMode(ci_RightUltraPing, OUTPUT);
  pinMode(ci_RightUltraData, INPUT);

  //set up light sensors
//  pinMode(ci_TopLightSensor, INPUT);
//  pinMode(ci_BottomLightSensor, INPUT);

  // set up drive motors
  pinMode(ci_FrontMotor, OUTPUT);
  frontMotor.attach(ci_FrontMotor);
  pinMode(ci_BackMotor, OUTPUT);
  backMotor.attach(ci_BackMotor);
  pinMode(ci_LeftMotor, OUTPUT);
  leftMotor.attach(ci_LeftMotor);
  pinMode(ci_RightMotor, OUTPUT);
  rightMotor.attach(ci_RightMotor);

  // set up arm motors
  pinMode(ci_ClawMotor, OUTPUT);
  clawMotor.attach(ci_ClawMotor);
  clawMotor.write(ci_ClawOpen); //opens claw off start because why not? first thing we'll grab is the waterbottlee right??


  //Set up Conveyor Belt Motor
  pinMode(ci_BeltMotor, OUTPUT);
  beltMotor.attach(ci_BeltMotor);


  // set up motor enable switch and mode selection Button

  pinMode(ci_MotorEnableSwitch, INPUT);
  pinMode(ci_ModeButton, INPUT);
  digitalWrite(ci_ModeButton, HIGH); //enables internal pullup resistor (button pushed = LOW)

  //have to initiate I2C motors in the order they are attached starting at the Aurdino
  encoder_LeftMotor.init((25.93384736) * (1.0 / 3.0)*MOTOR_393_SPEED_ROTATIONS, MOTOR_393_TIME_DELTA);
  encoder_LeftMotor.setReversed(true);  // adjust for positive count when moving forward
  encoder_FrontMotor.init((25.93384736) * (1.0 / 3.0)*MOTOR_393_SPEED_ROTATIONS, MOTOR_393_TIME_DELTA);
  encoder_FrontMotor.setReversed(true);  // adjust for positive count when moving forward
  encoder_RightMotor.init((25.93384736) * (1.0 / 3.0)*MOTOR_393_SPEED_ROTATIONS, MOTOR_393_TIME_DELTA);
  encoder_RightMotor.setReversed(false);  // adjust for positive count when moving forward
  encoder_BackMotor.init((25.93384736) * (1.0 / 3.0)*MOTOR_393_SPEED_ROTATIONS, MOTOR_393_TIME_DELTA);
  encoder_BackMotor.setReversed(false);  // adjust for positive count when moving forward
  encoder_LiftMotor.init((25.93384736) * (1.0 / 3.0)*MOTOR_393_SPEED_ROTATIONS, MOTOR_393_TIME_DELTA);
  encoder_LiftMotor.setReversed(false);  // adjust for positive count when moving forward


  //this means position should be measured in cm, and speed in cm/minuite, but we likely wont be measuring speed
  //might need to chang the setReverse() paramaters based on testing but i know how they need to be in relation to one another
}


void loop()
{
  Serial.println(modeIndex);
  if ((millis() - ul_3_S_Timer) > 3000)
  {
    bt_3_S_TimeUp = true;
  }

  // button-based mode selection
  if (digitalRead(ci_ModeButton) == LOW) //LOW means button is pushed
  {
    if (bt_DoOnce == false)
    {
      bt_DoOnce = true;
      modeIndex++;
      modeIndex = modeIndex & 7; // bitwise AND operator, this will reset mode to 0 if pressed 8 times, kinda useful not really haha
      ul_3_S_Timer = millis();
      bt_3_S_TimeUp = false;
    }
  }
  else
  {
    bt_DoOnce = false;
  }

  // modes
  // 0 = default after power up/reset
  // 1 = Press mode button once to enter. Run robot.
  // 2 = Press mode button twice to enter.
  // 3 = Press mode button three times to enter.
  // 4 = Press mode button four times to enter.

  switch (modeIndex) //mode to operate in
  {
    case 0:    //Robot stopped
      {
        break;
      }

    //******************RUNNING MODE***********************************************************
    //*****************************************************************************************

    case 1:
      {
        if (bt_3_S_TimeUp) //Run after 3 seconds
        {


#ifdef DEBUG_ENCODERS
          frontMotorPos = encoder_FrontMotor.getPosition();
          backMotorPos = encoder_BackMotor.getPosition();
          leftMotorPos = encoder_LeftMotor.getPosition();
          rightMotorPos = encoder_RightMotor.getPosition();

          Serial.print("Encoders F: ");
          Serial.print(encoder_FrontMotor.getPosition());
          Serial.print(", B: ");
          Serial.print(encoder_BackMotor.getPosition());
          Serial.print(", L: ");
          Serial.println(leftMotorPos, DEC);
          Serial.print(", R: ");
          Serial.println(RightMotorPos, DEC);
#endif

          //ive set this up so we can just add a new case for every new stage of the course
          //*******PLEASE remember break; can't emphasize this enough #goodCoding
          switch (stageIndex) //stage of the course
          {

            case 0:
              {

                Ping('R');
                delay(100);
                //                Ping('L');
                //                delay(100);
                //                Serial.println("blah");
                //                topLightData=analogRead(ci_TopLightSensor);
                //                bottomLightData=analogRead(ci_BottomLightSensor);
                //
                //                Serial.print(" T: ");
                //                Serial.print(topLightData);
                //                Serial.print(" B: ");
                //                Serial.println(bottomLightData);


                //                Drive();
                //                delay(1000);
                //                Drive('R', 300);
                //                delay(1000);
                //                Drive('B', 300);
                //                delay(1000);
                //                Drive('L', 300);
                //                delay(1000);
                //                Drive('F', 300);
                //                delay(1000);
                //                Slide("FR", 300);
                //                delay(1000);
                //                Slide("BR", 300);
                //                delay(1000);
                //                Slide("BL", 300);
                //                delay(1000);
                //                Slide();
                //                delay(1000);

                break; //remeber if you dont put this it will just go into the next case once current case is completed
              }

            case 1:
              {
                break;
              }

            case 2:
              {

                break;
              }

            case 3:
              {

                break;
              }





          }



        }
        break;
      }

    //******************END OF RUNNING MODE****************************************************
    //*****************************************************************************************

    case 2:    //after 3 seconds
      {
        if (bt_3_S_TimeUp)
        {
          Ping('L');
          delay(100);
          Ping('R');
          delay(100);
        }
        break;
      }

    case 3:    // after 3 seconds
      {
        if (bt_3_S_TimeUp)
        {
          Belt("run");
          delay(2000);
          Belt("stop");
          delay(2000);
        }
        break;
      }

    case 4:    //after 3 seconds.
      {
        if (bt_3_S_TimeUp)
        {

        }

        break;
      }
  }
}

//**************end of void loop() *****************************************************************

//Functions

//Base movement functions
void Stop() //stops all base motors
{
  frontMotor.writeMicroseconds(motorStopSpeed);
  backMotor.writeMicroseconds(motorStopSpeed);
  leftMotor.writeMicroseconds(motorStopSpeed);
  rightMotor.writeMicroseconds(motorStopSpeed);
}
//Speed is your value from 1500 (ie Drive(F,300); will make L&R wheels writeMicroseconds(1800)
//Drive is is F,B,L,R
void Drive(char Direction, int Speed) //note i made Speed/Dirrection with a capital S/D because lowercase made it highlighted so didnt know if that would have affected anything
{
  if (Direction == 'F') {
    frontMotor.writeMicroseconds(motorStopSpeed);
    backMotor.writeMicroseconds(motorStopSpeed);
    leftMotor.writeMicroseconds(motorStopSpeed + Speed);
    rightMotor.writeMicroseconds(motorStopSpeed + Speed);
  }
  else if (Direction == 'B') {
    frontMotor.writeMicroseconds(motorStopSpeed);
    backMotor.writeMicroseconds(motorStopSpeed);
    leftMotor.writeMicroseconds(motorStopSpeed - Speed);
    rightMotor.writeMicroseconds(motorStopSpeed - Speed);
  }
  else if (Direction == 'L') {
    frontMotor.writeMicroseconds(motorStopSpeed - Speed);
    backMotor.writeMicroseconds(motorStopSpeed - Speed);
    leftMotor.writeMicroseconds(motorStopSpeed);
    rightMotor.writeMicroseconds(motorStopSpeed);
  }
  else if (Direction == 'R') {
    frontMotor.writeMicroseconds(motorStopSpeed + Speed);
    backMotor.writeMicroseconds(motorStopSpeed + Speed);
    leftMotor.writeMicroseconds(motorStopSpeed);
    rightMotor.writeMicroseconds(motorStopSpeed);
  }

#ifdef DEBUG_ENCODERS
  Serial.print("Encoders F: ");
  Serial.print(encoder_FrontMotor.getPosition());
  Serial.print(", B: ");
  Serial.print(encoder_BackMotor.getPosition());
  Serial.print(", L: ");
  Serial.print(encoder_LeftMotor.getPosition());
  Serial.print(", R ");
  Serial.println(encoder_RightMotor.getPosition());
#endif
}

//Slide is for diagonal movement (all 4 motors running) FL,FR,BL,BR
void Slide(String Direction, int Speed)
{
  if (Direction == "FL") {
    frontMotor.writeMicroseconds(motorStopSpeed - Speed);
    backMotor.writeMicroseconds(motorStopSpeed - Speed);
    leftMotor.writeMicroseconds(motorStopSpeed + Speed);
    rightMotor.writeMicroseconds(motorStopSpeed + Speed);
  }
  else if (Direction == "FR") {
    frontMotor.writeMicroseconds(motorStopSpeed + Speed);
    backMotor.writeMicroseconds(motorStopSpeed + Speed);
    leftMotor.writeMicroseconds(motorStopSpeed + Speed);
    rightMotor.writeMicroseconds(motorStopSpeed + Speed);
  }
  else if (Direction == "BL") {
    frontMotor.writeMicroseconds(motorStopSpeed - Speed);
    backMotor.writeMicroseconds(motorStopSpeed - Speed);
    leftMotor.writeMicroseconds(motorStopSpeed - Speed);
    rightMotor.writeMicroseconds(motorStopSpeed - Speed);
  }
  else if (Direction == "BR") {
    frontMotor.writeMicroseconds(motorStopSpeed + Speed);
    backMotor.writeMicroseconds(motorStopSpeed + Speed);
    leftMotor.writeMicroseconds(motorStopSpeed - Speed);
    rightMotor.writeMicroseconds(motorStopSpeed - Speed);
  }
#ifdef DEBUG_ENCODERS
  Serial.print("Encoders F: ");
  Serial.print(encoder_FrontMotor.getPosition());
  Serial.print(", B: ");
  Serial.print(encoder_BackMotor.getPosition());
  Serial.print(", L: ");
  Serial.print(encoder_LeftMotor.getPosition());
  Serial.print(", R ");
  Serial.println(encoder_RightMotor.getPosition());
#endif
}

//Turn turns about centre of base, direction is dirrection front will turn -> L = counter clockwise, R = clockwise
void Turn(char Direction)
{
  if (Direction == 'L') {
    frontMotor.writeMicroseconds(motorStopSpeed);
    backMotor.writeMicroseconds(motorStopSpeed);
    leftMotor.writeMicroseconds(motorStopSpeed);
    rightMotor.writeMicroseconds(motorStopSpeed);
  }
  else if (Direction == 'R') {
    frontMotor.writeMicroseconds(motorStopSpeed);
    backMotor.writeMicroseconds(motorStopSpeed);
    leftMotor.writeMicroseconds(motorStopSpeed);
    rightMotor.writeMicroseconds(motorStopSpeed);
  }
#ifdef DEBUG_ENCODERS
  Serial.print("Encoders F: ");
  Serial.print(encoder_FrontMotor.getPosition());
  Serial.print(", B: ");
  Serial.print(encoder_BackMotor.getPosition());
  Serial.print(", L: ");
  Serial.print(encoder_LeftMotor.getPosition());
  Serial.print(", R ");
  Serial.println(encoder_RightMotor.getPosition());
#endif
}

//TurnAngle will turn to a specific angle (should ONLY be used when all that needs to be done is the turn, no other
//polling as it will not exit this function until the turn is complete
//Defult Direction is R/Clockwise
/*in progress, ill have to figure out the relationship between angle and encoder counts
 this may just become a turn 90degree function as the only times we likly have to turn to a speecific known angle would be 90 or multiples of it
 void TurnAngle(int Angle, char Direction ='R')
 {
 if(Direction=='L'){
 frontMotor.writeMicroseconds(motorStopSpeed-Speed);
 backMotor.writeMicroseconds(motorStopSpeed+Speed);
 leftMotor.writeMicroseconds(motorStopSpeed-Speed);
 rightMotor.writeMicroseconds(motorStopSpeed+Speed);
 }
 else if(Direction=='R'){
 frontMotor.writeMicroseconds(motorStopSpeed+Speed);
 backMotor.writeMicroseconds(motorStopSpeed-Speed);
 leftMotor.writeMicroseconds(motorStopSpeed+Speed);
 rightMotor.writeMicroseconds(motorStopSpeed-Speed);
 }
 #ifdef DEBUG_ENCODERS
  Serial.print("Encoders F: ");
  Serial.print(encoder_FrontMotor.getPosition());
  Serial.print(", B: ");
  Serial.println(encoder_BackMotor.getPosition());
  Serial.print(", L: ");
  Serial.println(encoder_LeftMotor.getPosition());
  Serial.print(", R ");
  Serial.println(encoder_RightMotor.getPosition());
#endif
 }*/

void Lift(int height)
{
}


void Extend()
{
}

//measure distance to target using ultrasonic sensor
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
  else if (side == 'T') {
    digitalWrite(ci_TopUltraPing, HIGH);
    delayMicroseconds(10);
    digitalWrite(ci_TopUltraPing, LOW);
    topEchoTime = pulseIn(ci_TopUltraData, HIGH, 10000);
  }

  // Print Sensor Readings
#ifdef DEBUG_ULTRASONIC
  if (side == 'L') echoTime = leftEchoTime;
  else if (side == 'R') echoTime = rightEchoTime;
  else if (side == 'T') echoTime = topEchoTime;

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

void PingIR()
{

}

void Belt(String mode)
{
  if (mode == "run") {
    beltMotor.writeMicroseconds(1900);
  }
  else if (mode == "stop") {
    beltMotor.writeMicroseconds(1500);
  }
}



