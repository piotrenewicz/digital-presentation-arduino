#include<Wire.h>
#include"RTClib.h"


const byte CLK = 16;
const byte IO = 15;
const byte CE = 14;

const byte modeSwitch = 19;
const byte func = 18;

const byte hl = 4;
const byte ml = 6;
const byte al = 10;

byte hp[] = {2, 3, 4, 5};
byte mp[] = {6, 7, 8, 9, 10, 11};
byte ap[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

byte count = 0;
bool mode = true;

DS1302 rtc(CE, CLK, IO);

void setup () {
  pinMode (modeSwitch, INPUT);
  pinMode (func, INPUT);
  
  for(byte i=0; i<hl; i++){
    pinMode(hp[i], OUTPUT);
  }
  
  for(byte i=0; i<ml; i++){
    pinMode(mp[i], OUTPUT);
  }
  
  //Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  
  //rtc.adjust(DateTime(__DATE__, __TIME__));
  if (! rtc.isrunning()) {
    //Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }
}

void loop () {
  if(mode)timer();
  if(!mode)counter();
  modeCheck();
}

void timer(){
  digitalWrite(CE, HIGH);
  DateTime now = rtc.now();
  digitalWrite(CE, LOW);

  byte h = now.hour();
  byte m = now.minute();

  for(byte i=0; i<hl; i++){
    byte state = bitRead(h, i);
    digitalWrite(hp[i], state);
    //Serial.print(state);
  }
  //Serial.println();
  
  for(byte i=0; i<ml; i++){
    byte state = bitRead(m, i);
    digitalWrite(mp[i], state);
    //Serial.print(state);
  }
  //Serial.println();

  if(digitalRead(func)){
    digitalWrite(CE, HIGH);
    rtc.adjust(DateTime(1420113600));
    digitalWrite(CE, HIGH);
    while(digitalRead(func));
  }
  
  /*Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();*/

  delay(1000);
}

void counter(){
  for(byte i=0; i<al; i++){
    byte state = bitRead(count, i);
    digitalWrite(ap[i], state);
    //Serial.print(state);
  }
  //Serial.println();
  delay(100);
  if(digitalRead(func)){
    count++;
    while(digitalRead(func));
  }
}

void modeCheck(){
  if(digitalRead(modeSwitch)){
    mode=!mode;
    count=0;
    while(digitalRead(modeSwitch));
  }
}

