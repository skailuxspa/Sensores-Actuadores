#define RED 0
#define GREEN 1
#define BLUE 2
#define CHECK 3

const char syncChar = '<';
const char endChar = '>';
const char redChar = 'r';
const char greenChar = 'g';
const char blueChar = 'b';
const char checkChar = 'c';

const uint8_t valuesLength = 4;

uint16_t values[valuesLength] = {0, 0, 0, 0};
byte canReadValues = 0;

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
  static uint16_t receivedData = 0;
  static byte i = 0;
  byte b = Serial.read();
  switch (b) {

    case syncChar:
      canReadValues = 0;
      receivedData = 0;
      i = 100;
      break;

    case redChar:
      values[i] = receivedData;
      receivedData = 0;
      i = RED;
      break;

    case greenChar:
      values[i] = receivedData;
      receivedData = 0;
      i = GREEN;
      break;

    case blueChar:
      values[i] = receivedData;
      receivedData = 0;
      i = BLUE;
      break;

    case checkChar:
      values[i] = receivedData;
      receivedData = 0;
      i = CHECK;
      break;

    case endChar:
      canReadValues = 1;
      ShowStuff();
      break;

    case '0' ... '9':
      if (i == RED || i == GREEN || i == BLUE || i == CHECK) {
        receivedData *= 10;
        receivedData += b - '0';

      }
      break;
  }
}

void ShowStuff() {
  //if (canReadValues) {
    for (int i = 0; i < valuesLength; i++)
    {
      Serial.print(values[i]);
      Serial.print(", ");
    }
    //Serial.println();
    //canReadValues = 0;
  //}
}

