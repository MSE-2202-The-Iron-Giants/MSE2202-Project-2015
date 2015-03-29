/*

BOARD 1 CODE, THIS WILL HAVE THE TOP US SENSOR AND THE LIGHT TRACKERS




 */

#include <Servo.h>
#include <uSTimer2.h>
#include <CharliePlexM.h>
#include <Wire.h>
#include <I2CEncoder.h>


//*******uncoment to debug******
//#define DEBUG_ULTRASONIC
//#define DEBUG_ENCODERS









boolean bt_MotorsEnabled = true; //(true = motors turned on)

//pins, pin numbers will change once we know everything that needs a pin

//const int ci_CharlieplexLED1 = 4; //will we use these? if so we dont have enough pins i think...
//const int ci_CharlieplexLED2 = 5;
//const int ci_CharlieplexLED3 = 6;
//const int ci_CharlieplexLED4 = 7;

const int ci_TopUltraPing = 13;
const int ci_TopUltraData = 13;

const int ci_TopLightSensor = A3;
const int ci_BottomLightSensor = A2;
unsigned int topLightData;
unsigned int bottomLightData;

//variables

long currentEncCount = 0;
double extendtime = 0;
double distance = 0;
double liftEnc = 0;

unsigned long echoTime; //general echoTime, usefull to have because you can set it equal to L,R,or T as done in ping function
unsigned long topEchoTime;

unsigned int variance = 1; //VARIANCE

unsigned int modeIndex = 0;
unsigned int stageIndex = 0;


unsigned long ul_3_S_Timer = 0;

boolean bt_3_S_TimeUp = false;
boolean bt_Heartbeat = true;
boolean bt_DoOnce = false;

//function prototypes
void Ping();
void PingIR();
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
  pinMode(ci_TopUltraPing, OUTPUT);
  pinMode(ci_TopUltraData, INPUT);


  //set up light sensors
  pinMode(ci_TopLightSensor, INPUT);
  pinMode(ci_BottomLightSensor, INPUT);

  // set up motor enable switch and mode selection Button

  //might use a mode button or this might just read a HIGH or low from the other board to sync both boards modes
//  pinMode(ci_ModeButton, INPUT);
//  digitalWrite(ci_ModeButton, HIGH); //enables internal pullup resistor (button pushed = LOW)

}


void loop()
{

  if ((millis() - ul_3_S_Timer) > 3000)
  {
    bt_3_S_TimeUp = true;
  }

//  // button-based mode selection
//  if (digitalRead(ci_ModeButton) == LOW) //LOW means button is pushed
//  {
//    if (bt_DoOnce == false)
//    {
//      bt_DoOnce = true;
//      modeIndex++;
//      modeIndex = modeIndex & 7; // bitwise AND operator, this will reset mode to 0 if pressed 8 times, kinda useful not really haha
//      ul_3_S_Timer = millis();
//      bt_3_S_TimeUp = false;
//    }
//  }
//  else
//  {
//    bt_DoOnce = false;
//  }

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



        }
        break;
      }

    case 3:    // after 3 seconds
      {
        if (bt_3_S_TimeUp)
        {

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

    case 5:
      {

      }
  }
}

//**************end of void loop() **************************************************************************************************************************************

//Functions
//measure distance to target using ultrasonic sensor
void Ping()
{

  digitalWrite(ci_TopUltraPing, HIGH);
  delayMicroseconds(10);
  digitalWrite(ci_TopUltraPing, LOW);
  topEchoTime = pulseIn(ci_TopUltraData, HIGH, 10000);


  // Print Sensor Readings
#ifdef DEBUG_ULTRASONIC
  echoTime = topEchoTime;

  Serial.print("Side: T");
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

//made this its own function because its used so much it will make code cleaner to read





