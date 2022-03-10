/* This is code for an RFID based Cashierless supermarket system. A customer places tagged items on a conveyor that transports them past an array of 4 RFID scanners
placed. Payment is done via an RFID dummy payment system with a memory feature. An access control system is incorporated to curtail thieves
Authors:
Lenny Orengo
Steve Mwenda
April 2020*/
#include <EEPROM.h>             //EEPROM library
#include <SoftwareSerial.h>     //software serial library
#include <Wire.h>               // 2-wire communication library
#include <LiquidCrystal_I2C.h>   //I2C LCD library
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display.
int count=0;                        // variable for each byte of serial data.
int data=0;                         // variable for each byte of serial data.
char input[12];                     // variable for storing item serial data.
char incoming[12];                  //variable for storing payment serial data. 
String item[]={"item_1","item_2","item_3","item_4"};  //Item and price data.
long price[]={200,300,400,500};
volatile long balance;             //Payment card balance.
int rfid[]={49,53,55,66};         //Arrays for item and payment card identifiers.
int card_id[]={49,53,55};
long total=0;                     //Total price.
//int number;                       //For item counting*
boolean var;                        //Flag for item detection and display.
char reading, reading_1, reading_2, reading_3;//For storing each rfid reading for comparison.
int i;                              //For comparison of RFID data for identification
int j;
boolean c= true;                    //Flag for conveyor motion control.
boolean x= true;                    //Flag for initializing serial communication only once per run.
boolean z= false;                   //Flag for the payment and access control ISR.
boolean y= true;                    //Flag for single execution of the "place card on reader" message.
SoftwareSerial portOne(10, 11);     // software serial #1: RX = digital pin 10, TX = digital pin 11

const int trigpinA = 4;             //Pins for ultrasonic sensors
const int echopinA = 5;
const int trigpinB = 6;
const int echopinB = 7;
const int trigpinC = 11;
const int echopinC = 12;
const int buzzer = 13;
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

void setup()
{
 EEPROM.get(0, balance);           // Read the stored balance from EEPROM memory location 0.
 pinMode(3,INPUT_PULLUP);
 attachInterrupt(digitalPinToInterrupt(3), payment ,LOW);  //Payment interrupt on pin 3.
 pinMode(2,INPUT_PULLUP);                                  //Recharge interrupt on pin 2.
 attachInterrupt(digitalPinToInterrupt(2), recharge ,LOW);
 
 lcd.init();                                              // initialize the lcd 
 lcd.setBacklight(HIGH);
 lcd.setCursor(0,0);
 lcd.print("The Cashierless");
 lcd.setCursor(3,1);
 lcd.print("Supermarket");
 delay(2000);

 pinMode(50,OUTPUT);                                    //For enabling and disabling payment EM-18 module.
 pinMode(trigpinA, OUTPUT);                             //Ultrasonic sensors
 pinMode(echopinA, INPUT);
 pinMode(trigpinB, OUTPUT);
 pinMode(echopinB, INPUT);
 pinMode(trigpinC, OUTPUT);
 pinMode(echopinC, INPUT);
 pinMode(buzzer,OUTPUT);

}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------


void serialEvent()                                      //ISR's for item identification for each of the EM-18 modules.
{
 count=0;
  while(Serial.available()&&count<12)
  {
    input[count]=Serial.read();
    count++;
    delay(2);
  }
 
  for( i=0;i<4;i++){
  if (int(input[6])==rfid[i])
    {
     reading= input[6];
     var=true;
     j=i;
     
    }
    }  
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

void serialEvent1()                                   //Module 2
{
 count=0;
  while(Serial1.available()&&count<12)
  {
    input[count]=Serial1.read();
    count++;
    delay(2);
  }
  
  for(int i=0;i<4;i++){
  if (int(input[6])==rfid[i])
    {
   reading_1=input[6];
   var=true;
   j=i;
    }  
    } 
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

void serialEvent2()                                 //Module 3
{
 count=0;
  while(Serial2.available()&&count<12)
  {
    input[count]=Serial2.read();
    count++;
    delay(2);
  }
  
  for(int i=0;i<4;i++){
  if (int(input[6])==rfid[i])
    {
   reading_2=input[6];
   var=true;
   j=i;
    }  
    }  
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

void serialEvent3()                              //Module 4
{
 count=0;
  while(Serial3.available()&&count<12)
  {
    input[count]=Serial3.read();
    count++;
    delay(2);
  }
  
  for(int i=0;i<4;i++){
  if (int(input[6])==rfid[i])
    {
   reading_3=input[6];
   var=true;
   j=i;
    }  
    } 
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------


void payment()                                //ISR that sets the payment flag.
{
   z=true;               
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------


void recharge()                               //ISR that tops up the balance.
{
   balance=20000;               
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

void card_detect()                          //Function for payment and access control
{
  
  portOne.listen();
  data=0;
  incoming[6]={0};
 
if(portOne.available())
{
  while(portOne.available()&&data<12)
  {
    incoming[data]=portOne.read();
    data++;
    delay(2);
   }

 if(incoming[6]==card_id[0])
 {
start:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Card balance:");
    lcd.setCursor(6,1);
    lcd.print(balance);
    delay(2000);
    if(balance<total)
    {
        lcd.clear();
        lcd.setCursor(3,0);
        lcd.print("Insufficient");
        lcd.setCursor(3,1);
        lcd.print("balance");
        delay(1000);
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("Press key 2");
        lcd.setCursor(2,1);
        lcd.print("To top up");
        delay(1000);
        goto start;
    }
    balance=balance-total;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" New balance:");
    lcd.setCursor(4,1);
    lcd.print(balance);
    EEPROM.put(0,balance);          //store balance in EEPROM location 0
    delay(3000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Thanks,you can");
    lcd.setCursor(0,1);
    lcd.print("now exit");
    delay(2000);
    lcd.clear();
    z=false;                         //Reset flags
    x=true;
    y=true;
    total=0;
    lcd.setCursor(0,0);
    lcd.print("Next customer");
    
 
    digitalWrite(52,HIGH);            // will be replaced by access control code
    delay(3000);
    digitalWrite(52,LOW);
    
      
 }
 else
 {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Kindly place");
    lcd.setCursor(0,1);
    lcd.print("the correct card");
    delay(3000);
    y=true;
    z=true;
    
 }
}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------


void sensors() {
  long durationA, distanceA;
  digitalWrite(trigpinA, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpinA, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpinA, LOW);
  durationA = pulseIn(echopinA, HIGH);
  distanceA = (durationA / 2) / 29.1;
  delay(100);

  long durationB, distanceB;
  digitalWrite(trigpinB, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpinB, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpinB, LOW);
  durationB = pulseIn(echopinB, HIGH);
  distanceB = (durationB / 2) / 29.1;
  delay(100);


  long durationC, distanceC;
  digitalWrite(trigpinC, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpinC, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpinC, LOW);
  durationC = pulseIn(echopinC, HIGH);
  distanceC = (durationC / 2) / 29.1;
  delay(100);
  if (distanceA <=10 || distanceB <=10 || distanceC <=10){
    digitalWrite(buzzer,HIGH);                                      //Replace with motor code
  
  }
  else
  {digitalWrite(buzzer,LOW);                                        //replace with motor code
  }
  }
 //-------------------------------------------------------------------------------------------------------------------------------------------------------------


void loop()
{ 

  if(x)                   //Executed only once per run
  {

 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Press key 1 when");
 lcd.setCursor(6,1);
 lcd.print("done");
 delay(2000);
 lcd.clear();
 portOne.end();
 digitalWrite(50,LOW);        //Disable payment module.
 var= false; 
 Serial.begin(9600);            //set baud rate for each of the serial ports to 9600bps
 Serial1.begin(9600);
 Serial2.begin(9600);
 Serial3.begin(9600);
 c= true;                      // Reset flags
 x= false;
 
  }
  if(var)                       //Executed only when an item is detected.Arguments are passed from the serial# ISR.
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(item[j]);
    lcd.setCursor(6,0);
    lcd.print("price:");
    lcd.setCursor(13,0);
    lcd.print(price[j]);
    total=total+price[j];
    lcd.setCursor(3,1);
    lcd.print("Bill:");
    lcd.setCursor(8,1);
    lcd.print(total);
    var=false;              //reset var and j
    j=0;
  }
if(c){                      // item detection code
     sensors();
}
 if(z)
  { 
    c=false;
    digitalWrite(50,HIGH);
    portOne.begin(9600);
    Serial.end();            //Disable the other serial ports.
    Serial1.end();
    Serial2.end();
    Serial3.end();
    if(y)
    {
    
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Place payment");
    lcd.setCursor(1,1);
    lcd.print("card on reader");
  
    }
  y=false;
  card_detect();          //Payment and access control function
 
   }
  }
  //-----------------------------------------------------------------------END--------------------------------------------------------------------------------------
