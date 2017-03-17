const int pin = 0;
int indx = 0;
int block[320];
string sblock;

void setup() {
  Serial.begin(115200);
}

void loop() {
  t = micros();
  if(t - t_old => 62){
    block[indx] = analogRead();
    t = t_old;
    indx++;
  }
  if(indx >= 320){
    for(int i=0; i<320; i++){
      sblock += block[i];
    }
    Serial.println(sblock);
    sblock = "";
  }
}

