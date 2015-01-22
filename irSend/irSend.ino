#include "IRremote.h"

IRsend irsend;

void setup() {
}

void loop() {
    irsend.sendSony(0x0003, 20);
    delay(100);
}