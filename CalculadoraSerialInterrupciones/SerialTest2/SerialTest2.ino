void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available() > 0){
    byte c = Serial.read();
    if(c == '+'){
      Serial.println("oh yes");
    }else{
      Serial.println("oh no");
    }
  }
}
