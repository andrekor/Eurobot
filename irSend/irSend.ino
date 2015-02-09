#include "IRremote.h";

int h = 38;
unsigned int buf[6] = {600, 600, 1200, 600, 600, 600};

IRsend irsend;

void setup() {
}

void loop() {
	//irsend.sendSony(0x0000, 16);
  	irsend.sendRaw(buf, 5, h);	
    delay(5);
}