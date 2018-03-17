byte state = 1;
uint32_t ok, error = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  randomSeed(analogRead(A0));
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  switch(state)
  {
    case 0:
      while(Serial.available() > 0) {
        char b = Serial.read();
        switch(b) {
          case '0':
            error++;
            printState();
            state = 1;
            break;
          case '1':
            ok++;
            printState();
            state = 1;
            break;
        }
        
      }
      break;
    case 1:
    */
      SendRGB();
  /*
      state = 0;
      break; 
  */

  delay(500);
}

void SendRGB() {
  uint8_t r = random(201);
  uint8_t g = random(201);
  uint8_t b = random(201);
  uint16_t c = r + g + b;

  Serial.print("<");
  Serial.print("r");
  Serial.print(r);
  Serial.print("g");
  Serial.print(g);
  Serial.print("b");
  Serial.print(b);
  Serial.print("c");
  Serial.print(c);
  Serial.print(">");
  //Serial.println();
}

void printState(){
  Serial.print("OK: ");
  Serial.println(ok);
  Serial.print("Errors: ");
  Serial.println(error);
}
