int led = 13;
int ldr = A0;
int count = 0;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(ldr, INPUT);

  // put your setup code here, to run once:
}

void loop() {
  Serial.print("ldr=");
  Serial.print(ldr);
  if (analogRead(ldr) < 100)
  { count=count+1;
  Serial.print("count=");
    Serial.print(count);
    //break;
  }
  delay(1000);
  if (count > 0)
    digitalWrite(led, HIGH);
  else
    digitalWrite(led, LOW);
  // put your main code here, to run repeatedly:

}
