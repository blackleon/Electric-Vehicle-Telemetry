#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include<Wire.h>
#include<LiquidCrystal_I2C.h> 


int sicaklikpin[4] = {A2, A3, A4, A6};

LiquidCrystal_I2C lcd(0x27 , 20 , 4 );

RF24 radio(7, 8); // CE, CSN

long int count = 0;
long int tTemp = 0;

int VoltMetre = A5;
float vin = 0;
float vout = 0;
float R1 = 10000;
float R2 = 1000;
float value = 0;
int buzz = 5;

int time1;
int latency = 2000;
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  lcd.begin();
  
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);

  attachInterrupt(0, InputA, RISING);
  //attachInterrupt(0, InputB, RISING);

  tTemp = millis();
  
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {

  if(millis() - tTemp >= 1000)
  {
    double rotation = (double)count - 0.0;
    float velocity = 1.50793 * rotation / 1000 ;
    if(velocity<0)
    {
      velocity*=-1;
    }
    count=0;

    value = analogRead(VoltMetre);
    vout = value*(10.0/2000.0)*(66.4/56.3);
    vin = vout / (R2 /(R1+R2));
    
    if(vin <= 0.09)
    {
      vin=0.0;
    }
    
  double sicaklikvolt[4];
  double sicaklikC[4];
  char sicakliksatir[24];
  memset(sicakliksatir, 0, 24);
  for(int a = 0; a<4; a++)
  {
    char sicaklikdeger[6];
    memset(sicaklikdeger, 0, 6);
    sicaklikvolt[a] = analogRead(sicaklikpin[a]);
    sicaklikC[a] = ((sicaklikvolt[a] / 1023)*5000)/10;
    if(sicaklikC[a] >=100)
    {
      sicaklikC[a] = 99.99;
    }else if(sicaklikC[a] < 0)
    {
      sicaklikC[a] = 0.0;
    }
    dtostrf(sicaklikC[a], 5, 2, sicaklikdeger);
    strcat(sicakliksatir, sicaklikdeger);
  }
    //Serial.println(velocity);
    //Serial.println(vin);
    char message[32];
    memset(message, 0, 32);
    
    
    strcat(message, sicakliksatir);

    
    char ctemp[5];
    dtostrf(velocity,4,1,ctemp);
    strcat(message, ctemp);
    dtostrf(vin, 4,1,ctemp);
    strcat(message, ctemp);
    radio.write(&message, sizeof(message));



    Serial.println(message);
    Serial.println(millis()-tTemp);
    String smessage(message);
    String temp1 = smessage.substring(0,5);
    String temp2 = smessage.substring(5,10);
    String temp3 = smessage.substring(10,15);
    String temp4 = smessage.substring(15,20);
    String hiz = smessage.substring(20,24);
    String volt = smessage.substring(24,28);
    /*Serial.print(temp1);
    Serial.print(" ");
    Serial.print(temp2);
    Serial.print(" ");
    Serial.print(temp3);
    Serial.print(" ");
    Serial.print(temp4);
    Serial.print(" ");
    Serial.print(hiz);
    Serial.print(" ");
    Serial.print(volt);
    Serial.println();*/

    lcd.setCursor(3,0);
    lcd.print("HIZ=      km/h");
    lcd.setCursor(8,0);
    lcd.print(hiz);
    lcd.setCursor(0,1);
    lcd.print("BATARYA =        V");
    lcd.setCursor(10,1);
    lcd.print(volt);

    lcd.setCursor(0,2);
    lcd.print("M =     C");
    lcd.setCursor(3,2);
    lcd.print(temp1);
    lcd.setCursor(9,2);
    lcd.print("| MS=     C");
    lcd.setCursor(14,2);
    lcd.print(temp2);
    lcd.setCursor(0,3);
    lcd.print("B1=     C|");
    lcd.setCursor(3,3);
    lcd.print(temp3);
    lcd.setCursor(11,3);
    lcd.print("B2=     C");
    lcd.setCursor(14,3);
    lcd.print(temp4);

    float t1 = sicaklikC[0];
    float t2 = sicaklikC[1];
    float t3 = sicaklikC[2];
    float t4 = sicaklikC[3];

    if(t3>60 || t4>60)
    {
      tone(buzz, 1000, latency);
    }
    else if(t3>50 || t4>50)
    {
      if(millis()-time1>latency)
      {
        tone(buzz, 1000, latency/4);
        time1=millis();
      }
    }
    
    tTemp = millis();
  }
}

void InputA()
{
  if(digitalRead(3)==LOW)
  {
    count++;
  }else{
    count--;
  }
}

void InputB()
{
  if(digitalRead(2)==LOW)
  {
    count--;
  }else{
    count++;
  }
}

