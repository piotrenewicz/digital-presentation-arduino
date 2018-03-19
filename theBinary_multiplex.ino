#define hours_s 2
#define minutes_s 3

void setDataRail(int n){
  PORTB=n;
}

void setActive(int segment){
  digitalWrite(hours_s, HIGH);
  digitalWrite(minutes_s, HIGH);
  digitalWrite(segment, LOW);
}

void multiplex(int h, int m, int t, int n){

  for(int i=0;i<n;i++){
    setActive(hours_s);
    setDataRail(h);
    delay(t);
    setActive(minutes_s);
    setDataRail(m);
    delay(t);
  }

}


