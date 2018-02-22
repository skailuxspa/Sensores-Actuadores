const char syncChar = '<';
const char endChar = '>';
const char redChar = 'r';
const char greenChar = 'g';
const char blueChar = 'b';
const char checkChar = 'c';

typedef enum {NONE, RED, GREEN, BLUE, CHECK} states;
states state = NONE;

const uint8_t valuesLength = 5;
//state = NIL;

uint16_t values[valuesLength] = {0, 0, 0, 0, 0};

//byte canReadValues = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");
}

void loop() {
  while (Serial.available() > 0) {
    ProcessSerial();
  }

  //RGBLed();
  //delay(100);
}

void RGBLed() {
  Serial.print("R: ");
  Serial.print(values[RED]);
  Serial.print(", G: ");
  Serial.print(values[GREEN]);
  Serial.print(", B: ");
  Serial.println(values[BLUE]);
}

void ProcessSerial() {
  static uint16_t valuesBuffer[valuesLength] = {0, 0, 0, 0};
  static uint16_t dataBuffer = 0;
  //static uint8_t i = 5;
  
  byte b = Serial.read();
  
  switch (b) {

    case syncChar:
      //canReadValues = 0;
      dataBuffer = 0;
      state = NONE;
      break;

    case redChar:
      valuesBuffer[state] = dataBuffer;
      state = RED;
      dataBuffer = 0;
      break;

    case greenChar:
      valuesBuffer[state] = dataBuffer;
      dataBuffer = 0;
      state = GREEN;
      break;

    case blueChar:
      valuesBuffer[state] = dataBuffer;
      dataBuffer = 0;
      state = BLUE;
      break;

    case checkChar:
      valuesBuffer[state] = dataBuffer;
      dataBuffer = 0;
      state = CHECK;
      break;

    case endChar:
      valuesBuffer[state] = dataBuffer;
      dataBuffer = 0;
      //canReadValues = 1;
      state = NONE;
      ShowStuff();
      break;

    case '0' ... '9':
      if (state != NONE) {
        dataBuffer *= 10;
        dataBuffer += b - '0';

      }
      break;
  }
}

void Checksum() {
  if(true){
    //values = valuesBuffer;
    Serial.println("Accepted");
  }else{
    Serial.println("Error");
  }
}

void ShowStuff() {
  //if (canReadValues) {
    for (int i = 0; i < valuesLength; i++)
    {
      Serial.print(values[i]);
      Serial.print(", ");
    }
    Serial.println();
    //canReadValues = 0;
  //}
}



