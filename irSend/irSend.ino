#include "IRremote.h"

IRsend irsend;

void setup() {
}

void loop() {
    irsend.sendSony(0x0002, 16);
    delay(25);
}