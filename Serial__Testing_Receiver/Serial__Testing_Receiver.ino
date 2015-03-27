const int led1 = 8;

int counter=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led1,OUTPUT);
}

void loop() {

  if(Serial.available())
  {
    counter=Serial.read();
  }
  
  if(counter>0)
  digitalWrite(led1,HIGH);
  
//  for(int i=0;i<counter;i++)
//  {
//    digitalWrite(led1,HIGH);
//    delay(50);
//    digitalWrite(led1,LOW);
//    delay(50);
//  }
//  
}
