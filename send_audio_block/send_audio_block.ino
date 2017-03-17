const int pin = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.print(micros());
  Serial.print("t");
  delay(500);

}
