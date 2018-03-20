#include<Wire.h>
#include"RTClib.h" // by NeiroN


#define hours_s 2
#define minutes_s 3

int h = 0;
int m = 0;
const float refr_rate;

DS1302 rtc(CE, CLK, IO);

void setDataRail(int n){
  PORTB=n;
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
  
    if (! rtc.isrunning()) {
        rtc.adjust(DateTime(__DATE__, __TIME__)); //get the damn thing up and running
    }
    
}

void loop(){
    DateTime data = rtc.now();
    h = data.hour();
    m = data.minute();
    multiplex(h, m, 60); //we only need to recheck after 60 seconds
}

