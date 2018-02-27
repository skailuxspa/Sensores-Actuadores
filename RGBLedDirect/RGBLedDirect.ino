#define RLED 8  //RED_LED
#define GLED 9  //GREEN_LED
#define BLED 10 //BLUE_LED

const char syncChar = '<';
const char endChar = '>';
const char redChar = 'r';
const char greenChar = 'g';
const char blueChar = 'b';
const char checkChar = 'c';

typedef enum { NONE,
			   RED,
			   GREEN,
			   BLUE,
			   CHECK } states;
states state = NONE;

const uint8_t valuesLength = 5;
uint16_t valuesBuffer[valuesLength] = {0, 0, 0, 0, 0};
uint16_t dataBuffer = 0;

void setup()
{
	Serial.begin(9600);
	Serial.println("Starting...");
}

void loop()
{
	while (Serial.available() > 0)
	{
		ProcessSerial();
	}
}

void ProcessSerial()
{
	//static uint16_t valuesBuffer[valuesLength] = {0, 0, 0, 0, 0};
	//static uint16_t dataBuffer = 0;

	byte b = Serial.read();

	switch (b)
	{

	case syncChar:
		state = NONE;
		ResetValueBuffer();
		ResetDataBuffer();
		break;

	case redChar:
		DBToVB();
		state = RED;
		ResetDataBuffer();
		break;

	case greenChar:
		DBToVB();
		state = GREEN;
		ResetDataBuffer();
		break;

	case blueChar:
		DBToVB();
		state = BLUE;
		ResetDataBuffer();
		break;

	case checkChar:
		DBToVB();
		state = CHECK;
		ResetDataBuffer();
		break;

	case endChar:
		DBToVB();
		state = NONE;
		if (Checksum(valuesBuffer[RED], valuesBuffer[GREEN], valuesBuffer[BLUE], valuesBuffer[CHECK]))
		{
			RefreshRGB(valuesBuffer[RED], valuesBuffer[GREEN], valuesBuffer[BLUE]);
			Serial.println("Led refreshed.");
		}
		else
		{
			Serial.println("Data corrupted.");
		}
		ResetValueBuffer();
		ResetDataBuffer();
		Serial.println("Buffers reset");
		break;

	case '0' ... '9':
		if (state != NONE)
		{
			dataBuffer *= 10;
			dataBuffer += b - '0';
		}
		break;
	}
}

bool Checksum(uint8_t _r, uint8_t _g, uint8_t _b, uint16_t _key)
{
	uint16_t buffer = _r + _g + _b;
	Serial.print("Checksum: ");
	Serial.print(buffer);
	Serial.print(", Given Key: ");
	Serial.println(_key);
	return (buffer == _key) ? true : false;
}

bool CheckRange(uint32_t _b)
{
	return (_b < 256) ? true : false;
}

void ResetDataBuffer()
{
	dataBuffer = 0;
}

void ResetValueBuffer()
{
	valuesBuffer[RED] = 0;
	valuesBuffer[GREEN] = 0;
	valuesBuffer[BLUE] = 0;
	valuesBuffer[CHECK] = 0;
}

void DBToVB()
{
	bool pass = CheckRange(dataBuffer);
	if (pass || state == CHECK)
	{
		valuesBuffer[state] = dataBuffer;
		Serial.println("Data pass");
	}
	else
	{
		valuesBuffer[state] = 255;
		Serial.println("Data out of range, setting to 255");
	}
}

void RefreshRGB(uint8_t _r, uint8_t _g, uint8_t _b)
{
	//analogWrite(RLED, values[RED]);
	//analogWrite(GLED, values[GREEN]);
	//analogWrite(BLED, values[BLUE]);
	Serial.print("R: ");
	Serial.println(_r);
	Serial.print("G: ");
	Serial.println(_g);
	Serial.print("B: ");
	Serial.println(_b);
}
