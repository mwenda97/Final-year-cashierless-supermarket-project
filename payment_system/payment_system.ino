#include <SPI.h>;
#include <MFRC522.h>
#include <EEPROM.h>


#define RST_PIN 9
#define SS_PIN 10
String card1, card2;
MFRC522 mfrc522(SS_PIN, RST_PIN);
int access = 0;
int buzz = 0;
int buzzer = 6;
int cardbalance_1 = 5000;
int cardbalance_2 = 1500;
int limitswitch_1 = 1;
int limitswitch_2 = 2;
int done_button = 3;
int motorup = 7;
int motordown = 8;
int trigpinD = 4;
int echopinD = 5;
int limitswitch_state_1, limitswitch_state_2, done;
int activate = false;

void setup()
{
  // EEPROM.get(0,cardbalance_1);
  // EEPROM.get(1,cardbalance_2);
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  pinMode(motorup, OUTPUT);
  pinMode(motordown, OUTPUT);
  pinMode(limitswitch_1, INPUT);
  pinMode(limitswitch_2, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(trigpinD, OUTPUT);
  pinMode(echopinD, INPUT);
  pinMode(done_button, INPUT);
  SPI.begin();
  mfrc522.PCD_Init();
  mfrc522.PCD_DumpVersionToSerial();
  Serial.print(F("payment system"));
  Serial.println();

}
void loop()
///{
  //if (activate == true)//when done_button is pressed ,payment and access control activated
  {
    //if (limitswitch_state_2 == HIGH)//when access control arm is down that only when you can pay
    //{
    payment();
    //}
    stevie();
    access_control_open();
    if (limitswitch_state_2 == LOW) //utrasonic sensor activated only when limit switch 2 is off ie arm is moving up or up
    {
      access_control_down();
    }
  //}
  activate = false;
}
void stevie()
{
  // payment();
  if (buzz == 1)
  {
    digitalWrite(buzzer, HIGH);
    Serial.println();
    delay(100);
    buzz = 0;
  }
  else
  {
    if (buzz == 0)
    {
      digitalWrite(buzzer, LOW);
    }
  }

}

void payment()
{
  if (! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  if (content.substring(1) == "F2 DE 4C 19")
  {
    if (cardbalance_1 >= 500)
    {
      cardbalance_1 = cardbalance_1 - 500;
      Serial.println();
      Serial.print("thank you for shopping with us");
      Serial.println();
      Serial.print("your card balance is : =");
      Serial.println();
      Serial.println(cardbalance_1);
      //EEPROM.put(0,cardbalance_1);
      Serial.print("you may pick your items");
      buzz = 1;
      access = 1;
      delay(1000);
    }
    else
    {
      card1 = content.substring(1);
      Serial.println();
      Serial.print("sorry  you don't have enough cash,your card balance is:=");
      Serial.println();
      Serial.println(cardbalance_1);
      buzz = 1;
      access = 0;
    }
  }
  else if (content.substring(1) == "AB FB B0 63")
  {
    if (cardbalance_2 >= 500 )
    {
      cardbalance_2 = cardbalance_2 - 500;
      Serial.println();
      Serial.print("thank you for shopping with us");
      Serial.println();
      Serial.print("your card balance is : =");
      Serial.println();
      Serial.println(cardbalance_2);
      Serial.print("you may pick your items");
      buzz = 1;
      access = 1;
      delay(1000);

    }
    else
    {
      card1 = content.substring(1);
      Serial.println();
      Serial.print("sorry  you don't have enough cash,your card balance is:=");
      Serial.println();
      Serial.println(cardbalance_2);
      buzz = 1;
      access = 0;
    }
  }
}
void motor_up()
{
  digitalWrite(motorup, HIGH);
  digitalWrite(motordown, LOW);
}
void motor_down()
{
  digitalWrite(motorup, LOW);
  digitalWrite(motordown, HIGH);
}
void motor_off()
{
  digitalWrite(motorup, LOW);
  digitalWrite(motordown, LOW);
}

void access_control_open()
{
  limitswitch_state_1 = digitalRead(limitswitch_1);
  limitswitch_state_2 = digitalRead(limitswitch_2);
  if (access == 1 && limitswitch_state_1 == LOW)
  {
    motor_up();
  }
  else if (limitswitch_state_1 == HIGH)
  {
    motor_off();
    access = 0;
  }
}
void access_control_down()
{
  long durationD, distanceD;
  digitalWrite(trigpinD, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpinD, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpinD, LOW);
  durationD = pulseIn(echopinD, HIGH);
  distanceD = (durationD / 2) / 29.1;
  Serial.print("ultrasonic_sensorD");
  Serial.print(distanceD);
  Serial.print("cm");
  Serial.println();
  delay(100);
  if (distanceD <= 10)
  {
    motor_down();
  }
  else if (limitswitch_state_2 == HIGH)
  {
    motor_off();
  }
}
void corona()
{
  done = digitalRead(done_button);
  if (done == HIGH)
  {
    activate = true;
  }
}
