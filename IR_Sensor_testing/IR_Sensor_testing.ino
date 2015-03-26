const int IRSensor=A1;
double distance;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(IRSensor,INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  distance=analogRead(IRSensor);
  Serial.print("Distanc: ");
  Serial.println(distance);
}
