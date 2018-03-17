#include <Arduino.h>

#define SERVE 2
#define RESET 3
#define P1 A0
#define P2 A1

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    Serial.println("ASDFASDF");
    pinMode(P1, INPUT);
    pinMode(P2, INPUT);
    pinMode(SERVE, INPUT);
    pinMode(RESET, INPUT);
}

void loop() {
    uint16_t p1 = analogRead(P1);
    uint16_t p2 = analogRead(P2);
    //p1 = constrain(p1, 170, 250);
    //p2 = constrain(p2, 170, 250);
    p1 = map(p1, 0, 1023, 120, 480);
    p2 = map(p2, 0, 1023, 100, 450);

    uint8_t serve = digitalRead(SERVE);
    uint8_t reset = digitalRead(RESET);
    
    sendStream(p1, p2, serve, reset);
    //delay(500);
}

void sendStream(uint16_t _p1, uint16_t _p2, uint8_t _s, uint8_t _r) {
    uint8_t p1msb = _p1 >> 8;
    uint8_t p1lsb = _p1 & 0xFF;
    
    uint8_t p2msb = _p2 >> 8;
    uint8_t p2lsb = _p2 & 0xFF;

    _s = _s << 1;
    uint8_t ctrl = _s | _r;

    Serial.write('<');
    Serial.write(p1msb);
    Serial.write(p1lsb);
    Serial.write(p2msb);
    Serial.write(p2lsb);
    Serial.write(ctrl);
    Serial.write('>');
}