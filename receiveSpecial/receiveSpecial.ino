
#define RECV_PIN 12
#define MICRO_DELAY 500

int beacons = 0x0;

void setup() {
	pinMode(RECV_PIN, INPUT);
	Serial.begin(9600);
}

void loop() {
	if (findStartBit()) {
		delayMicroseconds(MICRO_DELAY);
		if (digitalRead(RECV_PIN)) {
			beacons |= 0x1;
		} 
		delayMicroseconds(MICRO_DELAY);
		if (digitalRead(RECV_PIN)) {
			beacons |= 0x2;
		} 
		delayMicroseconds(MICRO_DELAY);
		if (digitalRead(RECV_PIN)) {
			beacons |= 0x3;
		}
		Serial.println(beacons);
	}
}

boolean findStartBit() {
	if (digitalRead(RECV_PIN)) {
		delayMicroseconds(MICRO_DELAY);
		if (!digitalRead(RECV_PIN)) {
			delayMicroseconds(MICRO_DELAY);
			if (digitalRead(RECV_PIN)) {
				return true;
			}
		}
	}
	return false;
}