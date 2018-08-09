#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include<Wire.h>
#include<LiquidCrystal_I2C.h>

#define latency 2000
,
LiquidCrystal_I2C lcd(0x27 , 20 , 4 );
RF24 radio(7, 8); // CE, CSN
int buzz = 5;

bool mode = false;
int time1;
int time2;
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  lcd.begin();
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  time1 = millis();
  time2 = millis();
}

void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    String message = String(text);
    String temp1 = message.substring(0,5);
    String temp2 = message.substring(5,10);
    String temp3 = message.substring(10,15);
    String temp4 = message.substring(15,20);
    String hiz = message.substring(20,24);
    String volt = message.substring(24,28);
    Serial.println(message);

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

    float t1 = temp1.toFloat();
    float t2 = temp2.toFloat();
    float t3 = temp3.toFloat();
    float t4 = temp4.toFloat();

    if(t3>60 || t4>60)
    {
      tone(buzz, 1000, latency);
    }
    else if(t3>50 || t4>50)
    {
      if(millis()-time1>latency)
      {
        tone(buzz, 1000, latency/2);
        time1=millis();
      }
    }
    time2 = millis();
  }else{
    if(millis()-time2>1000)
    {
        //lcd.clear();
        //lcd.print("Sinyal Yok");
        time2=millis();
    }
  }
}
