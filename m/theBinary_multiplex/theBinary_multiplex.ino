#include<Wire.h>
#include"RTClib.h" // by NeiroN

#define CLK 16
#define IO 15
#define CE 14

#define hours_s 2
#define minutes_s 3

int h = 0;
int m = 0;
const float refr_rate=40;

DS1302 rtc(CE, CLK, IO);

void setup(){

    pinMode(hours_s, OUTPUT);
    pinMode(minutes_s, OUTPUT);

    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);

    
     Wire.begin();
    rtc.begin();
  
    if (! rtc.isrunning()) {
        rtc.adjust(DateTime(__DATE__, __TIME__)); //get the damn thing up and running
    }
    
}



void setDataRail(int n){
  for(byte i=0; i<6; i++){
    byte state = bitRead(n, i);
    digitalWrite(i+8, state);
    //Serial.print(state);
  }
}

void setActive(int segment){
  digitalWrite(hours_s, LOW);
  digitalWrite(minutes_s, LOW);
  digitalWrite(segment, HIGH);
}

void multiplex(int h, int m, int t){

    unsigned long t0 = millis();
    while(millis()-t0<t*1000){
        setActive(hours_s);
        setDataRail(h);
        delay(500/refr_rate);
        setActive(minutes_s);
        setDataRail(m);
        delay(500/refr_rate);
    }

}



void loop(){
  digitalWrite(CE, HIGH);
    DateTime data = rtc.now();
    digitalWrite(CE, LOW);
    h = data.hour();
    m = data.minute();
    multiplex(h, m, 60); //we only need to recheck after 60 seconds
}

