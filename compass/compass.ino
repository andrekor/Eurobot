/*
André Kramer Ortn

Uno: 
I2C
Analog 4: SDA (Data line)
Analog 5: SCL (Clock line)
*/

#include <Wire.h> //Arduino library for I2C

#define Addr 0x1E //7-bit address of HMC5883 compass
#define PI 3.14

int x0, y0, z0;

void setup() {
	Serial.begin(9600);
	delay(100); //Power up delay
	Wire.begin(); 

	//set operating mode to continious
	Wire.beginTransmission(Addr);
	Wire.write(byte(0x02)); //Select mode register
	Wire.write(byte(0x00)); //Continous measurement mode
	Wire.endTransmission();
	//attachInterrupt(interrupt, function, mode);
}
boolean first = true;
void loop() {
	int x, y, z; //data from each of the axis
	//Initiate communications with compass
	Wire.beginTransmission(Addr); //Read data from HMC5883
	Wire.write(byte(0x03));  //Send request to X MSB register
	Wire.endTransmission();

	Wire.requestFrom(Addr, 6); //Request 6 bytes; 2 bytes per axis
	if (Wire.available() >= 6) {
		x = Wire.read() << 8 | Wire.read();
		z = Wire.read() << 8 | Wire.read();
		y = Wire.read() << 8 | Wire.read();
	}

	if (first) {
		x0 = x;
		y0 = y;
		z0 = z;
		first = false;
	}

	/*Serial.print("Y0: ");
	Serial.print(y0);
	Serial.print(", X");
	Serial.println(x);*/
	//Print
	//Serial.print(x);
	//Serial.print(", ");
	//Serial.println(y);
	/*
	Serial.print(", Z= ");
	Serial.println(z);*/
	Serial.println(heading(x, y));
	
	delay(1000);
}

float heading(int hx, int hy) {
	float divider = (hx*1.0)/hy;
	if (hy > 0) {
		return (90-atan(divider)*180/PI);
	} else if (hy < 0) {
		return (270-atan(divider)*180/PI);
	} else if (hy == 0 && hx > 0) {
		return 180;
	} else {
		return 0;
	}
}