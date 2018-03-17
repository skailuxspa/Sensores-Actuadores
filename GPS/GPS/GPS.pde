import processing.serial.*;

String[] NMEAString;
Serial myPort;

boolean running = true;

int index = 0;

StringList receivedString = new StringList();

void setup() {
  // connect to first available Serial COM
  try {
    NMEAString = loadStrings("pathTest.txt");
    myPort = new Serial(this, Serial.list()[0], 9600);
  } finally {
    println("Connected to COM");
  }
  
  if(myPort != null) {
    myPort.bufferUntil('\n');
  }
}

void draw() {
  if(running) {
    println(index);
    myPort.write(NMEAString[index] + "\n");
    
    running = false;  
  }
  
}

void serialEvent(Serial myPort) {
  
  String resultString = myPort.readStringUntil('\n');
  receivedString.append(resultString);
  
  index++;
  
  if(index >= NMEAString.length) {
    SaveReceivedStrings();
  }else {
    running = true;
  }
  
}

void SaveReceivedStrings() {
  saveStrings("received.txt", receivedString.array());
  println("completed");
}