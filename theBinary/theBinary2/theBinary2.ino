#include<Wire.h>
#include"RTClib.h" // by NeiroN


#define hours_s 4
#define minutes_s 5

const byte CE = 14;
const byte IO = 15;
const byte CLK = 16;

#define modeSwitch 3
#define func 2

bool mode = true;

int h = 0;
int m = 0;

int count = 0;

const float refr_rate = 120;


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
    while(millis()-t0<t){
        setActive(hours_s);
        setDataRail(h*4);
        delay(500/refr_rate);
        setActive(minutes_s);
        setDataRail(m);
        delay(500/refr_rate);
    }

}

void setup(){
    
   	Wire.begin();
   	rtc.begin();
    DDRB = B111111;
    
    pinMode(hours_s, OUTPUT);
    pinMode(minutes_s, OUTPUT);

    pinMode(modeSwitch, INPUT_PULLUP);
    pinMode(func, INPUT_PULLUP);
    
    if (! rtc.isrunning()) {
        rtc.adjust(DateTime(__DATE__, __TIME__)); //get the damn thing up and running
    }
    
}

void timer(){
  digitalWrite(CE, HIGH);
  DateTime data = rtc.now();
  digitalWrite(CE, LOW);
  h = data.hour();
  m = data.minute();
  if(h>12)h=h-12;
  multiplex(h, m, 1000);

  if(!digitalRead(func)){                            //ustawianie zegara
    multiplex(h, m, 2000);
    if(!digitalRead(func)){
      DateTime setData= new DateTime(1514790000);      // godzina default po wejściu w ust. zeg.
      h=setData.hour();
      m=setData.minute();
      if(h>12)h=h-12;
      multiplex(h, m, 500);                           // 2018-01-01 07:00:00                              // czas na zauważenie że poszło
      while(digitalRead(modeSwitch)){
        while(!digitalRead(func)){
          setData = DateTime(setData.unixtime() + 60); // zmiana godziny za każdym krokiem.
          h=setData.hour();
          m=setData.minute();
          if(h>12)h=h-12;
          multiplex(h, m, 150);                          // czas trwania kroku
        }
        multiplex(h, m, 10);
      }



    
      digitalWrite(CE, HIGH);
      rtc.adjust(setData);                             //po zakończeniu beczki upload godz do rtc
      digitalWrite(CE, HIGH);
    }
  }
}

void counter(){
  multiplex(count/64, count%64, 100);
  if(!digitalRead(func)){
    count++;
    while(!digitalRead(func)){
      multiplex(count/64, count%64, 1);
    }
      
  }
}

void modeCheck(){
  if(!digitalRead(modeSwitch)){
    mode=!mode;
    count=0;
    while(!digitalRead(modeSwitch));
  }
}

void loop(){
    if(mode)timer();
    if(!mode)counter();
    modeCheck();
}

