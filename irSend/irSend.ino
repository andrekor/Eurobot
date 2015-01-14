#include "IRremote.h"

IRsend irsend;

void setup() {
}

void loop() {
    irsend.sendSony(0x0001, 20);
    delay(100);
}