#include <Arduino.h>


const unsigned int MAX_INPUT = 120;

void setup() {
    Serial.begin(9600);
    Serial.println("Starting...");
}

void loop() {
    while(Serial.available() > 0) {
        processIncome(Serial.read());    
    }
}

void processIncome(const byte inByte) {
    static char input_line [MAX_INPUT];
    static unsigned int input_pos = 0;

    switch (inByte) {

        case '\n':   // end of text
            input_line [input_pos] = 0;  // terminating null byte
            // terminator reached! process input_line here ...
            process_data (input_line);
            // reset buffer for next time
            input_pos = 0;  
            break;

        case '\r':   // discard carriage return
            break;

        case '$':
            break;

        case '*':
            if (input_pos < (MAX_INPUT - 1))
                input_line [input_pos++] = ',';
            break;

        default:
            // keep adding if not full ... allow for terminating null byte
            if (input_pos < (MAX_INPUT - 1))
                input_line [input_pos++] = inByte;
            break;

    } 
}

void process_data (const char * data) {
    char * pch;
    pch = strtok (data, ",");
    Serial.println(strtok (data, ",")[0]);
}   // end of process_data