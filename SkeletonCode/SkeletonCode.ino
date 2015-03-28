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
Servo extendMotor;
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
const int ci_MotorEnableSwitch = 13; //this will show if motors are enabled or not on pin 13



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
unsigned long frontMotorPos;
unsigned long backMotorPos;
unsigned long leftMotorPos;
unsigned long rightMotorPos;
unsigned long clawMotorPos;
long currentEncCount = 0;
double extendtime = 0;
double distance = 0;
double liftEnc = 0;

unsigned long echoTime; //general echoTime, usefull to have because you can set it equal to L,R,or T as done in ping function
unsigned long leftEchoTime;
unsigned long rightEchoTime;
unsigned long topEchoTime;

unsigned int variance = 1; //VARIANCE

unsigned int modeIndex = 0;
unsigned int stageIndex = 0;
unsigned int liftcounter = 0;

unsigned long ul_3_S_Timer = 0;

boolean bt_3_S_TimeUp = false;
boolean bt_Heartbeat = true;
boolean bt_DoOnce = false;
boolean turning = false;
boolean bumperHit = false;
boolean lifted = false;

//function prototypes
void Stop(int);
void Drive(char Direction = 'F', int Speed = 300);
void Slide(String Direction = "FL", int Speed = 300);
void Turn(char);
void Turn90(char);
void Lift(int);
void Extend();
void ExtendDist(double);
void Retract();
void Ping(char);
void Belt(String mode = "run");
void Square();
void Search();
void Drive_Distance(char side, int Speed, float distance);



void DebugEncoders();

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

  // set up claw motors
  pinMode(ci_ClawMotor, OUTPUT);
  clawMotor.attach(ci_ClawMotor);
  clawMotor.write(ci_ClawOpen); //opens claw off start because why not? first thing we'll grab is the waterbottlee right??

  // set up arm motor
  pinMode(ci_ExtendMotor, OUTPUT);
  extendMotor.attach(ci_ExtendMotor);

  //set up lift motor
  pinMode(ci_LiftMotor, OUTPUT);
  liftMotor.attach(ci_LiftMotor);


  //Set up Conveyor Belt Motor
  pinMode(ci_BeltMotor, OUTPUT);
  beltMotor.attach(ci_BeltMotor);


  // set up motor enable switch and mode selection Button

  pinMode(ci_MotorEnableSwitch, INPUT);
  pinMode(ci_ModeButton, INPUT);
  digitalWrite(ci_ModeButton, HIGH); //enables internal pullup resistor (button pushed = LOW)

  //have to initiate I2C motors in the order they are attached starting at the Aurdino
  encoder_LeftMotor.init((25.93384736) * (1.0 / 3.0)*MOTOR_393_SPEED_ROTATIONS, MOTOR_393_TIME_DELTA);
  encoder_LeftMotor.setReversed(false);  // adjust for positive count when moving forward
  encoder_FrontMotor.init((25.93384736) * (1.0 / 3.0)*MOTOR_393_SPEED_ROTATIONS, MOTOR_393_TIME_DELTA);
  encoder_FrontMotor.setReversed(false);  // adjust for positive count when moving forward
  encoder_RightMotor.init((25.93384736) * (1.0 / 3.0)*MOTOR_393_SPEED_ROTATIONS, MOTOR_393_TIME_DELTA);
  encoder_RightMotor.setReversed(true);  // adjust for positive count when moving forward
  encoder_BackMotor.init((25.93384736) * (1.0 / 3.0)*MOTOR_393_SPEED_ROTATIONS, MOTOR_393_TIME_DELTA);
  encoder_BackMotor.setReversed(true);  // adjust for positive count when moving forward
  encoder_LiftMotor.init((25.93384736) * (1.0 / 3.0)*MOTOR_393_SPEED_ROTATIONS, MOTOR_393_TIME_DELTA);
  encoder_LiftMotor.setReversed(true);  // adjust for positive count when moving forward


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
          DebugEncoders();
#endif

          //ive set this up so we can just add a new case for every new stage of the course
          //*******PLEASE remember break; can't emphasize this enough #goodCoding
          switch (stageIndex) //stage of the course
          {

            case 0:
              {
                Drive('F');
                delay(1000);
                Stop();
                delay(700);
                Drive('R');
                delay(1000);
                Stop();
                delay(700);
                Drive('B');
                delay(1000);
                Stop();
                delay(700);
                Drive('L');
                delay(1000);
                Stop();
                delay(700);
                Slide("FL");
                delay(1000);
                Stop();
                delay(700);
                Slide("FR");
                delay(1000);
                Stop();
                delay(700);
                Slide("BR");
                delay(1000);
                Stop();
                delay(700);
                Slide("BL");
                delay(1000);
                Stop();
                delay(700);


                break; //remeber if you dont put this it will just go into the next case once current case is completed
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


          //Drive('R');

          frontMotor.writeMicroseconds(1700);
          backMotor.writeMicroseconds(1700);

          Ping('L');
          delay(100);
          Ping('R');

          //SQUARE TO WALL
          if (leftEchoTime / 58 > (rightEchoTime / 58 + variance)) // If left side of bot is too far from wall
          {
            leftMotor.writeMicroseconds(motorStopSpeed + 200);        //Adjust position
            rightMotor.writeMicroseconds(motorStopSpeed);
          }
          else if (rightEchoTime / 58 > (leftEchoTime / 58 + variance)) //Right side of bot is too far from wall
          {
            rightMotor.writeMicroseconds(motorStopSpeed + 200);        //Adjust position
            leftMotor.writeMicroseconds(motorStopSpeed);
          }

          else if (rightEchoTime / 58 == leftEchoTime / 58) //Square to wall
          {
            rightMotor.writeMicroseconds(motorStopSpeed);        // do nothing
            leftMotor.writeMicroseconds(motorStopSpeed);
          }

        }
        break;
      }

    case 3:    // after 3 seconds
      {
        if (bt_3_S_TimeUp)
        {

          //extendMotor.writeMicroseconds(2000);
          //           Serial.println("running");
          //           extendMotor.writeMicroseconds(1000);


          liftMotor.writeMicroseconds(2000);
          Serial.println(encoder_LiftMotor.getPosition());
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

//**************end of void loop() **************************************************************************************************************************************

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
  DebugEncoders()
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
  DebugEncoders();
#endif
}

void Drive_Distance(char side, int Speed, float distance) //Drive a specific direction, speed, and distance. Drive_Distance ('R', 200, 2.0) = Drive right at 1700 for 2.0 encoder distance
{
  frontMotorPos = encoder_FrontMotor.getPosition(); //Get encoder position
  frontMotorPos = frontMotorPos + distance; //Set variable to encoder position + amount we want to move
  while (encoder_FrontMotor.getPosition() > frontMotorPos)
  {
    Drive(side, Speed); //Drive
  }
}

void Turn(char Direction)//There is no length of time/distance designated for this function.  A break of the function needs to be introduced in your code
{

  if (Direction == 'L') {
    frontMotor.writeMicroseconds(1300);//possible need to change the direction of these motors.
    backMotor.writeMicroseconds(1700);
    leftMotor.writeMicroseconds(1300);
    rightMotor.writeMicroseconds(1700);
  }
  else if (Direction == 'R') {
    frontMotor.writeMicroseconds(1700);
    backMotor.writeMicroseconds(1300);
    leftMotor.writeMicroseconds(1700);
    rightMotor.writeMicroseconds(1300);
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
void Turn90(char Direction)//this function will turn the robot 90 degrees from where it starts.
{
  frontMotorPos = encoder_FrontMotor.getPosition(); //continuously updated

  if (turning == false) {
    currentEncCount = encoder_FrontMotor.getPosition();
    turning = true;
  }

  if (Direction == 'L' && frontMotorPos > currentEncCount + 1.5) { //the 1.5 value is arbitrary. the encoder count for 90 degrees is what should go in
    frontMotor.writeMicroseconds(1300);
    backMotor.writeMicroseconds(1700);
    leftMotor.writeMicroseconds(1300);//correct motor directions?
    rightMotor.writeMicroseconds(1700);
  }
  else if (Direction == 'R' && frontMotorPos > currentEncCount + 1.5) {
    frontMotor.writeMicroseconds(1700);
    backMotor.writeMicroseconds(1300);
    leftMotor.writeMicroseconds(1700);
    rightMotor.writeMicroseconds(1300);
  }

  if (turning == true && frontMotorPos == currentEncCount + 1.5) {
    turning = false;
    frontMotor.writeMicroseconds(1500);//stops the robot
    backMotor.writeMicroseconds(1500);
    leftMotor.writeMicroseconds(1500);
    rightMotor.writeMicroseconds(1500);
    return;//breaks out of the function
  }

}

void Stop(int time)
{
  frontMotor.writeMicroseconds(1500);
  backMotor.writeMicroseconds(1500);
  leftMotor.writeMicroseconds(1500);
  rightMotor.writeMicroseconds(1500);
  delay(time);
  return;//stops the robot for a certain amount of time and then breaks the function.
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

void Lift()
{
  {
    currentEncCount = encoder_LiftMotor.getPosition();
    if (lifted = true)
     {return;}

    else
    {
      if (liftcounter == 0)
      {
        liftcounter++;
        liftEnc = encoder_LiftMotor.getPosition();
      }

      if (currentEncCount < liftEnc + 20 && liftcounter == 1)
      {
        liftMotor.writeMicroseconds(1600);
        liftcounter++;
      }

      else if (currentEncCount < liftEnc + 40 && liftcounter == 2)
      {
        liftMotor.writeMicroseconds(1700);
        liftcounter++;
      }

      else if (currentEncCount < liftEnc + 60 && liftcounter == 3)
      {
        liftMotor.writeMicroseconds(1800);
        liftcounter++;
      }

      else if (currentEncCount < liftEnc + 140 && liftcounter == 4)
      {
        liftMotor.writeMicroseconds(2000);
        liftcounter++;
      }

      else if (currentEncCount >= liftEnc + 140 && liftcounter == 5)
      {
        liftMotor.writeMicroseconds(1500);
        liftcounter = 0;
        lifted = true;
      }

      else
      {
        liftMotor.writeMicroseconds(1500);
      }

    }

    return;
  }
}


void ExtendDist(double distance)
{
  //at 2000, arm extends at 1.70 in/sec or 4.32 cm/sec
  extendtime = distance / 4.32;

  extendMotor.writeMicroseconds(2000);
  delay(extendtime);
  extendMotor.writeMicroseconds(1500);
  return;
}

void Retract(double distance)//if extended a variable distance, main code needs to include an edit to the double variable "distance"
{
  extendtime = distance / 4.32;

  extendMotor.writeMicroseconds(1000);
  delay(extendtime);
  extendMotor.writeMicroseconds(1500);
  return;
}

void Extend()
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

void Square() //Function traces walls to find objectives
{
  //PING ULTRASONIC
  Ping('L');
  delay(100);
  Ping('R');

  //SQUARE TO WALL
  if (leftEchoTime > (rightEchoTime + variance)) // If left side of bot is too far from wall
  {
    leftMotor.writeMicroseconds(motorStopSpeed + 200);        //Adjust position
    //rightMotor.writeMicroseconds(1400);
  }
  else if (rightEchoTime > (leftEchoTime + variance)) //Right side of bot is too far from wall
  {
    rightMotor.writeMicroseconds(motorStopSpeed + 200);        //Adjust position
    //leftMotor.writeMicroseconds(1400);
  }

  else if (rightEchoTime == leftEchoTime) //Square to wall
  {
    rightMotor.writeMicroseconds(motorStopSpeed);        // do nothing
    leftMotor.writeMicroseconds(motorStopSpeed);
  }

}//END OF SQUARE

void Search()
{
  if (bumperHit) //Hit a wall
  {
    Drive_Distance('L', 200, 2.0); // "back up" to provide room to turn
    Turn('R'); //Turn right
  }

  if (rightEchoTime > 100)// Where 100 is arbitrary large number, which would indicate that bot has passed the table boundary
  {
    Drive_Distance('R', 200, 2.0); //Drive far enough to clear table
    Turn ('L'); //Turn Left to face table
  }

  Square(); //Constantly square to wall
  Drive('R'); //Drive Right, along wall
}//END OF SEARCH

//My idea was to have something like : while bottomLightData ==0, Search(); so that seeing a light cancels the search function.


//made this its own function because its used so much it will make code cleaner to read
void DebugEncoders() {
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
  Serial.println(rightMotorPos, DEC);
}



