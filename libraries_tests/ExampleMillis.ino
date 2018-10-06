void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

unsigned long long int MILLISSEC = 0;
void loop() {
  if(millis() > MILLISSEC){
    MILLISSEC += 2000;
    Serial.println(millis());
  }
}
