#include "IRremote.h"

IRsend irsend;

void setup() {
}

void loop() {
    irsend.sendSony(0x0002, 20);
    delay(100);
}