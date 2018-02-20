uint32_t a = 0;
const uint8_t dataSize = 15;
char data[dataSize + 1] = {'|', '|', '|', '|', '|', '|', '|', '|', '|', '|', '|', '|', '|', '|', '|', 0};
uint8_t index = -1;
String charStr = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Starting...");
  charStr.reserve(200);
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available() > 0) {
    char b = (char)Serial.read();
    switch (b) {
      case '\n':
        charStr = String(data);
        Serial.println(charStr);
        EmptyData();
        break;
      default:
        if(index >= dataSize){
          ForceTerminateStream();
        }else{
          data[index++] = (char)b;
        }
        break;
    }
  }
}

void ForceTerminateStream(){
  Serial.println("Stream overflow.");
  EmptyData();
}

void EmptyData() {
  for (int i = 0; i < dataSize; i++){
    data[i] = '|';
  }
  index = -1;
}

