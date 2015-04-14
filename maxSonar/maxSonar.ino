/*
Max sonar 
Inputs: TX, PW, AN
Filters: None, median, highest_mode, lowest_mode, simple, best
*/

#include <SoftwareSerial.h>

#define txPin 3
#define rxPin 2
#define ANALOGPIN 1
#define PW_PIN 7
#define PW_SCALE 50 //calculated pulse width scale
#define AN_SCALE 

SoftwareSerial sonarSerial(rxPin, txPin, true); //define serial port for receiving data, output from maxSonar is inverted requiring true to be set

double anVolt, inches, cm, pulse;
int sum = 0;
int avgRange = 60; //Quantity of values to average

void setup() {
	Serial.begin(9600);
	sonarSerial.begin(9600); //Start serial for maxsonar
	delay(500);
}

//Since the analog readings from maxsonar are very sensitiv
//it will be more accurate to average n samples
void loop() {
	//analog();
//	serialRead();
	pw();
}

/*To read from the analog pin*/
void analog(){
	for (int i = 0; i < avgRange; i++) {
		//Used to read in the analog voltage output that is being sent by the maxSonar device
		//Scale facros is (Vcc/512) per inch. 5V supply yields ~9.8mV/in
		//Arduino analog pin goes from 0 to 1024, so the value has to be divided by 2 to het the actual inhes
		anVolt = analogRead(ANALOGPIN);
		Serial.println(anVolt);
		sum += anVolt;
		delay(10);
	}
	inches = sum/avgRange;
	cm = inches*2.54;
	//Serial.print("cm: ");
	//Serial.println(cm);
	/*Serial.print(inches)
;	Serial.print("in, ");
	Serial.print(cm);
	Serial.println("cm");*/
	sum = 0; 
	//delay(400);
}

/*
From calculation we have found a scalingfactor on pw/50 per cm. This is for white paper material. 
*/
void pw() {
	pinMode(PW_PIN, INPUT);
	//USed to read in the pulse that is being sent by the maxSonar device. 
	//Pulse width representation with a scale factor of 147 us per Inch

	pulse = pulseIn(PW_PIN, HIGH);
	Serial.println(pulse);

	//147us per inch
	/*inches = pulse/147;
	cm = inchToCm(inches);
	Serial.print(inches);
	Serial.print("in, ");
	Serial.print(cm);
	Serial.println("cm");*/

}

double inchToCm(double inch) {
	return inch*2.54;
}


boolean stringComplete = false;

void serialRead() {
	int range = EZread();
	if (stringComplete) {
		stringComplete = false;
		Serial.print("Range");
		Serial.println(range);

	}
}

int EZread() {
	int result; 
	char inData[4]; 	//read data in to char array
	int index = 0;
	sonarSerial.flush();
	while (!stringComplete) {
		if (sonarSerial.available()) {
			char rByte = sonarSerial.read();
			if (rByte == 'R') { //Serial data marked R at start of data
				while(index < 3) {
					if (sonarSerial.available()) {
						inData[index] = sonarSerial.read();
						index++;
					}
				}
				inData[index] = 0x00; //nullbyte at the end to use atoi
			}
			stringComplete = true;
			result = atoi(inData);
		}
	}
	return result;
}