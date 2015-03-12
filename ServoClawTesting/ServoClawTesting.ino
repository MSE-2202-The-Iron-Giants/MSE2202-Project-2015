#include <Servo.h>

Servo clawMotor;

const int ci_ClawMotor=10;

void setup()
{
 Serial.begin(9600);
 pinMode(ci_ClawMotor,OUTPUT);
 clawMotor.attach(ci_ClawMotor);
 clawMotor.write(0);
}

void loop()
{
//  for(int i=0;i<180;i++)
//  {
//    clawMotor.write(i);
//    delay(20);
//  }
//  
//    for(int i=180;i>0;i--)
//  {
//    clawMotor.write(i);
//    delay(20);
//  }

//  clawMotor.write(0);
//  delay(1000);
//  clawMotor.write(30);
//    delay(1000);
//  clawMotor.write(70);
//    delay(1000);
//  clawMotor.write(120);
//    delay(1000);
//  clawMotor.write(180);
//    delay(1000);

clawMotor.write(50);
delay(1000);
clawMotor.write(180);
delay(3000);
  
}
