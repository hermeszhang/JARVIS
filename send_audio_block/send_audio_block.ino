const int pin = 0;
const int vcc = 5;
const float midlevel = vcc/2.0;
const float quantlevel = 1024.0;
const int BLOCK_SIZE = 100;

int indx = 0;
float block[BLOCK_SIZE] = {0.0};
int t = 0;
int t_old = 0;
String sblock;


void setup() {
  Serial.begin(57600);
  
}

void loop() {
  t = micros();
  if((t - t_old) >= 62){
    block[indx] = ((analogRead(pin)/quantlevel)*vcc - midlevel); //Convert signal to valid sound signal
    t_old = t;
    indx++;
  }
  if(indx >= BLOCK_SIZE){
    for(int i=0; i<BLOCK_SIZE; i++){
      //sblock += block[i];
      //sblock += ", ";
      Serial.println(block[i]);
    }
    indx = 0;
  }
}

