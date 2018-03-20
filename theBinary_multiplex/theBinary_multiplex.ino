#include<Wire.h>
#include"RTClib.h" // by NeiroN


#define hours_s 8
#define minutes_s 9

#define CE 14
#define IO 15
#define CLK 16

int h = 0;
int m = 0;
const float refr_rate = 120;

DS1302 rtc(CE, CLK, IO);

void setDataRail(int n){
  PORTD=n;
}

void setActive(int segment){
  digitalWrite(hours_s, HIGH);
  digitalWrite(minutes_s, HIGH);
  digitalWrite(segment, LOW);
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

void setup(){
    
   	Wire.begin();
   	rtc.begin();
    DDRD=225;
    
    pinMode(hours_s, OUTPUT);
    pinMode(minutes_s, OUTPUT);
    
    if (! rtc.isrunning()) {
        rtc.adjust(DateTime(__DATE__, __TIME__)); //get the damn thing up and running
    }
    
}

void loop(){
    digitalWrite(CE, HIGH);
    DateTime data = rtc.now();
    digitalWrite(CE, LOW);
    h = data.hour();
    m = data.minute();
    multiplex(h, m, 1);
}

