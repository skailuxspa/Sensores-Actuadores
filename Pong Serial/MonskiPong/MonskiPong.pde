
import processing.serial.*;     // import the Processing serial library

Serial myPort;                 // The serial port
String resultString;           // string for the results

float leftPaddle, rightPaddle;   // variables for the flex sensor values
int resetButton, serveButton;    // variables for the button values
int leftPaddleX, rightPaddleX;   // horizontal positions of the paddles
int paddleHeight = 50;           // vertical dimension of the paddles
int paddleWidth = 10;            // horizontal dimension of the paddles

float leftMinimum = 120;         // minimum value of the left flex sensor
float rightMinimum = 100;        // minimum value of the right flex sensor
float leftMaximum = 530;         // maximum value of the left flex sensor
float rightMaximum = 500;        // maximum value of the right flex sensor

int ballSize = 10;     // the size of the ball
int xDirection = 1;    // the ball's horizontal direction. 
// left is –1, right is 1.
int yDirection = 1;    // the ball's vertical direction.
// up is –1, down is 1.
int xPos, yPos;        // the ball's horizontal and vertical positions

boolean ballInMotion = false;  // whether the ball should be moving

int leftScore = 0;      // score for left paddle
int rightScore = 0;     // score for right paddle

int fontSize = 36;
void setup() {
  size(640, 480);             // set the size of the applet window
  printArray(Serial.list());     // List all the available serial ports

  // get the name of your port from the serial list.
  // The first port in the serial list on my computer 
  // is generally my microcontroller, so I open Serial.list()[0].
  // Change the 0 to the number of the serial port 
  // to which your microcontroller is attached:
  String portName = Serial.list()[0];
  // open the serial port:
  
  while(myPort == null) {
    try {
      myPort = new Serial(this, portName, 9600);
    } finally {
      println("Connected to serial port");  
    }
  }
  // read bytes into a buffer until you get a linefeed (ASCII 10):
  myPort.bufferUntil('>');

  // initialize the sensor values:
  leftPaddle = height/2;
  rightPaddle = height/2;
  resetButton = 0;
  serveButton = 0;

  // initialize the paddle horizontal positions:
  leftPaddleX = 50;
  rightPaddleX = width - 50;

  // set no borders on drawn shapes:
  noStroke();

  // initialize the ball in the center of the screen:
  xPos = width/2;
  yPos = height/2;

  // create a font with the third font available to the system:
  PFont myFont = createFont(PFont.list()[2], fontSize);
  textFont(myFont);
}

void draw() {
  // set the background and fill color for the applet window:
  background(#044f6f);
  fill(255);

  // draw the left paddle:
  rect(leftPaddleX, leftPaddle, paddleWidth, paddleHeight);

  // draw the right paddle:
  rect(rightPaddleX, rightPaddle, paddleWidth, paddleHeight);
  // calculate the ball's position and draw it:
  if (ballInMotion == true) {
    animateBall();
  }

  // if the serve button is pressed, start the ball moving:
  if (serveButton == 1) {
    ballInMotion = true;
  }

  // if the reset button is pressed, reset the scores
  // and start the ball moving:
  if (resetButton == 1) {
    leftScore = 0;
    rightScore = 0;
    ballInMotion = true;
  }
  // print the scores:
  text(leftScore, fontSize, fontSize);
  text(rightScore, width-fontSize, fontSize);
}

// serialEvent  method is run automatically by the Processing sketch
// whenever the buffer reaches the byte value set in the bufferUntil() 
// method in the setup():

void serialEvent(Serial myPort) { 
  // read the serial buffer:
  byte[] input = myPort.readBytesUntil('>');

  // split the input string at the commas
  // and convert the sections into integers:
  if (input.length == 7) {
    
    int msb = input[1];
    int lsb = input[2];
    msb = (msb << 8) & 0xFF00;
    lsb = lsb & 0xFF;
    int p1 = msb | lsb;
    
    //player 2
    msb = input[3];
    lsb = input[4];
    msb = (msb << 8) & 0xFF00;
    lsb = lsb  & 0xFF;
    int p2 = msb | lsb;
    
    //reset & serve
    int serve = input[5] & 0x02;
    int reset = input [5] & 0x01;
    serve = serve >> 1;
    
    // scale the flex sensors' results to the paddles' range:
    leftPaddle = map(p1, leftMinimum, leftMaximum, 0, height);
    rightPaddle = map(p2, rightMinimum, rightMaximum, 0, height);

    // assign the switches' values to the button variables:
    resetButton = reset;
    serveButton = serve; 

    // add the values to the result string:
    resultString += "left: "+ leftPaddle + "\tright: " + rightPaddle;
    resultString += "\treset: "+ resetButton + "\tserve: " + serveButton;
  }
  //myPort.write('\r');      // send a carriage return
}

void animateBall() {
  // if the ball is moving left:
  if (xDirection < 0) {
    //  if the ball is to the left of the left paddle:
    if  ((xPos <= leftPaddleX)) {
      // if the ball is in between the top and bottom 
      // of the left paddle:
      if ((leftPaddle - (paddleHeight/2) <= yPos) && 
        (yPos <= leftPaddle + (paddleHeight /2))) {
        // reverse the horizontal direction:
        xDirection =-xDirection;
      }
    }
  }
  // if the ball is moving right: 
  else {
    //  if the ball is to the right of the right paddle:
    if  ((xPos >= ( rightPaddleX + ballSize/2))) {
      // if the ball is in between the top and bottom 
      // of the right paddle:
      if ((rightPaddle - (paddleHeight/2) <=yPos) && 
        (yPos <= rightPaddle + (paddleHeight /2))) {

        // reverse the horizontal direction:      
        xDirection =-xDirection;
      }
    }
  }

  // if the ball goes off the screen left:
  if (xPos < 0) {
    rightScore++;
    resetBall();
  }
  // if the ball goes off the screen right:
  if (xPos > width) {
    leftScore++;
    resetBall();
  }


  // stop the ball going off the top or the bottom of the screen:
  if ((yPos - ballSize/2 <= 0) || (yPos +ballSize/2 >=height)) {
    // reverse the y direction of the ball:
    yDirection = -yDirection;
  }
  // update the ball position:
  xPos = xPos + xDirection;
  yPos = yPos + yDirection;

  // Draw the ball:
  rect(xPos, yPos, ballSize, ballSize);
}

void resetBall() {
  // put the ball back in the center
  xPos = width/2;
  yPos = height/2;
}