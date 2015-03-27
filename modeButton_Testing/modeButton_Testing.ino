
const int led1 = 8;
const int led2 = 9;
const int ci_ModeButton = 12;
boolean bt_DoOnce = false;
int modeIndex = 0;

unsigned long ul_3S_Timer = 0;
boolean bt_3S_TimeUp = false;

void setup() {

  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(ci_ModeButton, INPUT);
  digitalWrite(ci_ModeButton, HIGH);

}

void loop() {
  Serial.println(modeIndex);

  //  if (digitalRead(modeButton) == LOW)
  //  {
  //    digitalWrite(led2, HIGH);
  //  }
  //  else digitalWrite(led2, LOW);

  if ((millis() - ul_3S_Timer) > 3000)
  {
    bt_3S_TimeUp = true;
  }

  if (digitalRead(ci_ModeButton) == LOW) //need to figure out the button
  {
    if (bt_DoOnce == false)
    {
      bt_DoOnce = true;
      modeIndex++;
      modeIndex = modeIndex & 7; // bitwise AND, eaugin said it resests counter if you press 7 times or something
      digitalWrite(led2, HIGH);
      ul_3S_Timer = millis();
      bt_3S_TimeUp = false;
    }

  }
  else
  { bt_DoOnce = false;
    digitalWrite(led2, LOW);
  }

  switch (modeIndex)
  {
    case 0:
      {
        if (bt_3S_TimeUp) //Run after 3 seconds
        {

        }
        break;
      }

    case 1:
      {
        if (bt_3S_TimeUp) //Run after 3 seconds
        {

          digitalWrite(led1, HIGH);



        }
        break;
      }

    case 2:
      {
        if (bt_3S_TimeUp) //Run after 3 seconds
        {

          digitalWrite(led2, HIGH);


        }
        break;
      }
  }
}
