int trigpinA = 4;
int echopinA = 5;
int trigpinB = 6;
int echopinB = 7;
int trigpinC = 8;
int echopinC = 9;
int buzzer = 10;
void setup() {
  Serial.begin(9600);
  pinMode(trigpinA, OUTPUT);
  pinMode(echopinA, INPUT);
  pinMode(trigpinB, OUTPUT);
  pinMode(echopinB, INPUT);
  pinMode(trigpinC, OUTPUT);
  pinMode(echopinC, INPUT);
  pinMode(buzzer,OUTPUT);
  // put your setup code here, to run once:

}

void sensors() {
  long durationA, distanceA;
  digitalWrite(trigpinA, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpinA, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpinA, LOW);
  durationA = pulseIn(echopinA, HIGH);
  distanceA = (durationA / 2) / 29.1;
  Serial.print("ultrasonic_sensorA");
  Serial.print(distanceA);
  Serial.print("cm");
  Serial.println();
  delay(100);

  long durationB, distanceB;
  digitalWrite(trigpinB, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpinB, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpinB, LOW);
  durationB = pulseIn(echopinB, HIGH);
  distanceB = (durationB / 2) / 29.1;
  Serial.print("ultrasonic_sensorB");
  Serial.print(distanceB);
  Serial.print("cm");
  Serial.println();
  delay(100);


  long durationC, distanceC;
  digitalWrite(trigpinC, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpinC, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpinC, LOW);
  durationC = pulseIn(echopinC, HIGH);
  distanceC = (durationC / 2) / 29.1;
  Serial.print("ultrasonic_sensorC");
  Serial.print(distanceC);
  Serial.print("cm");
  Serial.println();
  delay(100);
  if (distanceA <=10 || distanceB <=10 || distanceC <=10){
    digitalWrite(buzzer,HIGH);
  
  }
  else
  {digitalWrite(buzzer,LOW);
  }}
 

  void loop()
  {
    sensors();

  }


  // put your main code here, to run repeatedly:
