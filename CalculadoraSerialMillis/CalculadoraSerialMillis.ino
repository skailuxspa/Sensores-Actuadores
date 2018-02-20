/*
  Flujo:
  Pide operacion -> Revisa si hay operación -> Pone TimeOut -> Pide primer digito -> Pone TimeOut -> Pide segundo digito -> imprime resultado
  
  TODO:
  1) Serial Input Handle API: Generalize the processing method.
*/

#define REQUEST_OPERATION 0
#define WAITING_FOR_OPERATION 1
#define SET_TIME_OUT 2
#define WAITING_FOR_INPUT 3
#define EXECUTE_OPERATION 4

#define ABORT_TIME 5000

//Serial communication protocol
const char endOfSentenceDelimiter = '\n';
const char startOfSentenceDelimiter = ':';

//Calculator state machine
uint8_t state = REQUEST_OPERATION;
uint8_t operation;
uint8_t inputState = 0;
int32_t a, b;
uint32_t previousMillis = 0;

//Abortion Flag
byte cancelFlag = 0;

//Communication API flags
bool newData = false;
int32_t data;

void setup() {
  Serial.begin(9600);
  Serial.println("Start");
}

void loop() {
  ReadInput();
  Calculator();
}

//Calculator state machine
void Calculator() {

  uint32_t currentMillis = millis();
  if(currentMillis - previousMillis >= ABORT_TIME){
    previousMillis = currentMillis;
    cancelFlag = 1;
  }
  
  switch (state) {
    case REQUEST_OPERATION:
      RequestOperation();
      break;
    case WAITING_FOR_OPERATION:
      WaitForOperation();
      break;
    case WAITING_FOR_INPUT:
      if(!cancelFlag){
        WaitForInput(inputState);
      }else{
        Serial.println("Abort");
        SetState(REQUEST_OPERATION);
      }
      break;
  }
}

//Prints to the serial console the value of the states
void debugStates() {
  //Serial.print("Input state: ");
  //Serial.print(inputState);
  //Serial.print(", State: ");
  //Serial.println(state);
}

//Reinitializes everything and goes to the next state
void RequestOperation() {
  debugStates();
  Serial.println("Please enter an operation to complete. (+, -, *, / or %)");
  inputState = 0;
  ResetData();
  SetState(WAITING_FOR_OPERATION);
}

//Check if there is an operation available for fetching and if there is, sets it as the operation to fulfill and goes to the next state
void WaitForOperation() {
  if (GetNewData()) {
    char incommingByte = GetData();
    Serial.println(incommingByte);
    ResetData();
    if (incommingByte == '+' || incommingByte == '-' || incommingByte == '/' || incommingByte == '*' || incommingByte == '%') {
      operation = incommingByte;
      inputState = 1;
      SetTimeOut();
    } else {
      Serial.println("Invalid operation.");
    }
  }
}

//Checks if there is new data available for fetching and if there is, sets it to one of the registers and procedes depende on if it stores the first or the second digit
void WaitForInput(uint8_t _inputState) {
  if (GetNewData()) {
    int32_t incommingByte = GetData();
    Serial.println(incommingByte);
    ResetData();
    switch (inputState) {
      case 1:
        a = incommingByte;
        inputState++;
        SetTimeOut();
        break;
      case 2:
        b = incommingByte;
        ExecuteOperation();
        break;
    }
  }
}

//Turn on the timer/attach interrupt that activates go abortion system and then go to the next state.
void SetTimeOut() {
  cancelFlag = 0;
  previousMillis = millis();
  
  Serial.print("Enter a number(digit #");
  Serial.print(inputState);
  Serial.println(")");
  debugStates();
  SetState(WAITING_FOR_INPUT);
}

//Executes a mathematical operation depending on the value stores in 'operation'
void ExecuteOperation() {
  //Timer1.stop();
  uint32_t r = 0;
  switch (operation) {
    case '+':
      r = a + b;
      Serial.print("Sum: ");
      Serial.println(r);
      break;

    case '-':
      r = a - b;
      Serial.print("Difference: ");
      Serial.println(r);
      break;

    case '*':
      r = a * b;
      Serial.print("Product: ");
      Serial.println(r);
      break;

    case '/':
      r = a / b;
      Serial.print("Division: ");
      Serial.println(r);
      break;

    case '%':
      r = a % b;
      Serial.print("Module: ");
      Serial.println(r);
      break;
  }
  a, b, r = 0;
  SetState(REQUEST_OPERATION);
}

//Sets the state
void SetState(uint8_t _state) {
  state = _state;
}


//SERIAL INPUT HANDLING API

//Input handler state machine. Processes information differently according to the state of the calculator
void ReadInput() {
  switch (state) {
    case WAITING_FOR_OPERATION:
      ProcessOperation();
      break;
    case WAITING_FOR_INPUT:
      ProcessDigit();
      break;
  }
}

//process digits (WAIT_FOR_INPUT)
void ProcessDigit() {
  static int32_t receivedNumber = 0;
  static bool negative = false;

  byte c = Serial.read ();

  switch (c) {
    case endOfSentenceDelimiter:
      if (negative)
        SetData(-receivedNumber);
      else
        SetData(receivedNumber);
      break;

    case startOfSentenceDelimiter:
      receivedNumber = 0;
      negative = false;
      ResetData();
      break;

    case '0' ... '9':
      receivedNumber *= 10;
      receivedNumber += c - '0';
      break;

    case '-':
      negative = true;
      break;
  }
}

//process operation (WAIT_FOR_OPERATION)
void ProcessOperation() {
  static int32_t receivedData = 0;

  byte c = Serial.read();

  switch (c) {
    case endOfSentenceDelimiter:
      SetData(receivedData);
      break;

    case startOfSentenceDelimiter:
      receivedData = 0;
      ResetData();
      break;

    case '+':
      receivedData = '+';
      break;

    case '-':
      receivedData = '-';
      break;

    case '*':
      receivedData = '*';
      break;

    case '/':
      receivedData = '/';
      break;

    case '%':
      receivedData = '%';
      break;
  }
}

//set the data and set a flag that new data is available
void SetData(int32_t _data) {
  newData = true;
  data = _data;
  //Serial.print("Informacion seteada a: ");
  //Serial.println(data);
}

//get the value of the stores data
int32_t GetData() {
  if (newData = true && data != 0) {
    return data;
  }
  //Serial.println("Devolviendo que NO hay informacion");
  return 0;

}

//get the flag for the new data
bool GetNewData() {
  return newData;
}

//resets the flag and the value of the stores data
void ResetData() {
  newData = false;
  data = 0;
  //Serial.print("Informacion reseteada a: ");
  //Serial.println(data);
}


