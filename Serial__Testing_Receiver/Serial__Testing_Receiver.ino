const int led1 = 8;
int incomingByte = 0;
int counter = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
}

void loop() {

  if (Serial.available())
  {

    incomingByte = Serial.read();
    
    if(incomingByte%2)
    {digitalWrite(8,HIGH);}
    else
    {digitalWrite(8,LOW);}


  }
}




