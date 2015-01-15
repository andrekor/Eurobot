/*
	IR Remote benytter interrupt kanal 2, derfor kan vi bare benytte kanal 0 og 1
*/

#include <Stepper.h>
#include <IRremote.h>

#define MICRO_DELAY 300 //The delay are not used inside the interrupt
#define oneRevolution 1600 //Muligens må endre dette. Fra instructables.com ....

#define dirPin 8 //the pin that controls the direction of the steppermotor
#define stepPin 9 //Output pin for the steppermotor

#define RECV_PIN 11

#define testPin 3

//Setting ip the IR receiver
IRrecv irrecv(RECV_PIN);
decode_results results;

#define X_BEACON1 0  //0 m i X retning
#define Y_BEACON1 1  // 1 m i y retning

//Defines for beacon2
#define X_BEACON2 3  //0 m i X retning
#define Y_BEACON2 2  // 1 m i y retning

//Defines for beacon3
#define X_BEACON3 3  //0 m i X retning
#define Y_BEACON3 0  // 1 m i y retning

#define VALUE_BEACON_A 0x0001 
#define VALUE_BEACON_B 0x0002
#define VALUE_BEACON_C 0x0003

float X_beaconPos[3] = {0, 3, 3};
float Y_beaconPos[3] = {1, 2, 0};

int stepCount;

int aSteps = 0; //steps from start to beacon A
int bSteps = 0;  //steps from start to beacon B
int cSteps = 0; //steps from start to beacon C

float alpha;
float beta;
float gamma;

boolean towerStop = false;

void setup() {

	//Setup the ir receiver
	irrecv.enableIRIn();

	//Setup for steppermotor
	pinMode(testPin, INPUT);
	pinMode(dirPin, OUTPUT); //output mode to direction pin
	pinMode(stepPin, OUTPUT); //output mode to step pin
	digitalWrite(dirPin, LOW); //Initialize dir pin
	digitalWrite(stepPin, LOW); //Initialize step pin
	
	stepCount = 0; 
	Serial.begin(9600);

	//Setup the interrupt
	cli(); //Stop interrupts
	 //set timer0 interrupt at 2kHz
	interruptSetup();
}

void interruptSetup() {
	 //set timer0 interrupt at 2kHz
  	TCCR0A = 0;// set entire TCCR2A register to 0
  	TCCR0B = 0;// same for TCCR2B
  	TCNT0  = 0;//initialize counter value to 0
  	// set compare match register for 2khz increments
  	OCR0A = 124;// = (16*10^6) / (2000*64) - 1 (must be <256)
  	// turn on CTC mode
  	TCCR0A |= (1 << WGM01);
  	// Set CS01 and CS00 bits for 64 prescaler
  	TCCR0B |= (1 << CS01) | (1 << CS00);   
  	// enable timer compare interrupt
  	TIMSK0 |= (1 << OCIE0A);

  	TCCR2A = 0x02;
  	sei(); //allow interrupts
}

int i = 0;
void loop() {
	//if (stepCount < oneRevolution + 1) 
//		digitalWrite(dirPin, LOW);
//	else
//		digitalWrite(dirPin, HIGH);
/*
	digitalWrite(stepPin, HIGH);
	digitalWrite(stepPin, LOW);
	delayMicroseconds(MICRO_DELAY);
	stepCount++;
//	Serial.println(stepCount);
	if (stepCount == 1600) {
		Serial.println(i++);
		stepCount = 0;
	}
//	if (stepCount < oneRevolution*2) //Reset counter
*/
//	rotate();
	//if (digitalRead(testPin));
	//	step();
	//receiveBeaconSignal();
	//if (!towerStop) {
//		test();
//	}

		//delay(30);
	
	/*if (stepCount >= 1600) {
		stepCount = 0;
		int dir = !digitalRead(dirPin);
		digitalWrite(dirPin, dir);
	//	delay(5000);
	}*/
}	

void rotate() {
	digitalWrite(stepPin, HIGH);
	digitalWrite(stepPin, LOW);
	delayMicroseconds(MICRO_DELAY);
	stepCount++; //Increase the step count
	i = stepCount/oneRevolution;
	if (i >= 2) {
//		Serial.println(i);
		Serial.println(stepCount);
		stepCount = 0;
		//Change the direction of the tower
		int dir = !digitalRead(dirPin);
		Serial.print("Direction ");
		Serial.println(dir);
		digitalWrite(dirPin, dir);	
	}
}


void step() {
	digitalWrite(stepPin, HIGH);
	digitalWrite(stepPin, LOW);
	delayMicroseconds(MICRO_DELAY);
	stepCount++;
}

/*Method that receives the IR signals, and detects 
from which beacon the signal comes from*/
void receiveBeaconSignal() {
  if (irrecv.decode(&results)) {
  	int value = results.value;
  	Serial.println(value);
  //	Serial.print("Signal from beacon");
 //   Serial.println(value);
 //   Serial.print("IR received : ");
    //The beacon towers are coded with a value from 1 -> 3
    if (value > 0 && value < 4) {
    	towerStop = true;
 	  //  Serial.print("Signal from beacon");
    //	Serial.println(value);
    	switch (value) {
    	    case VALUE_BEACON_A:
    	      aSteps = stepCount;
    	      break;
    	    case VALUE_BEACON_B:
    	      bSteps = stepCount;
    	      break;
    	    case VALUE_BEACON_C:
    	    	cSteps = stepCount;
    	    	break;
    	    default:
    	    	break;
    	}

    	Serial.print("steps to beacon A: ");
    	Serial.println(aSteps);
      //Sende the 0 for beacon 1, 1 for beacon 2, and 2 for beacon 3
      //So that i can get the position directly from the array
  //    readAngleOfBeacon(value-1); 
    } else {
    //  Serial.println("No valid signal");
     // towerStop = false;
    }
    //continue to look for more beacons. 
    irrecv.resume();
  } 
}

/*Calculates the angle in degree, when we know the how many steps we have gone*/
float calcDegree(int steps) {
	return steps*(oneRevolution/360);
}

/*Reads the angle from the steppermotor, 
and puts it in the angle array. Takes in 
the id of the beacon.*/
/*
void readAngleOfBeacon(int beacon) {
  //The fixed position of the beacon we have found
  float xi = X_beaconPos[beacon];
  float yi = Y_beaconPos[beacon];

  //Find the angle of the stepper motor
  beaconAngle[beacon] = 0; //should be the angle read from the stepper motor
  //Do this three times to get three equations and three unknown
  //Use Trust Region Methods
  Serial.println(sizeof(beaconAngle)/sizeof(float));
  for (int i = 0; i < (sizeof(beaconAngle)/sizeof(float)); i++) {
    //thetai = atan2(yi-yr, xi-xr) - thetaR;
    atan2(yi - yr, xi-xr) - thetaR;
    }
}*/

/*
A->C->B->A
Starter ved høyeste verdi..
*/
void anglePositive() {
	if (aSteps > bSteps && aSteps > cSteps) {
		//A er størst, altså konfigurasjon null mellom A og C
		gamma = angle(aSteps-bSteps);
		alpha = angle(bSteps-cSteps);
		beta = angle(cSteps+(1600-aSteps));
	} else if (bSteps > cSteps) {
		//B er størst, altså null konfigurasjon mellom B og A
		alpha = angle(bSteps-cSteps);
		beta = angle(cSteps-aSteps);
		gamma = angle(aSteps+(1600-bSteps));
	} else {
		//C er størst, altså null konfigurasjon mellom C og B
		beta = angle(cSteps-aSteps);
		gamma = angle(aSteps-bSteps);
		alpha = angle(bSteps+(1600-cSteps));
	}
}

/*
A->B->C->A
Starter ved høyeste verdi..
*/
void angleNegative() {
	if (aSteps > bSteps && aSteps > cSteps) {
		//A er størst, altså konfigurasjon null mellom A og B
		gamma = angle(aSteps-cSteps);
		alpha = angle(cSteps-bSteps);
		beta = angle(bSteps+(1600-aSteps));
	} else if (bSteps > cSteps) {
		//B er størst, altså null konfigurasjon mellom B og C
		alpha = angle(bSteps-aSteps);
		beta = angle(aSteps-cSteps);
		gamma = angle(cSteps+(1600-bSteps));
	} else {
		//C er størst, altså null konfigurasjon mellom C og A
		beta = angle(cSteps-bSteps);
		gamma = angle(bSteps-aSteps);
		alpha = angle(aSteps+(1600-cSteps));
	}
}

float angle(int steps) {
	return steps*(360/oneRevolution);
}

ISR(TIMER0_COMPA_vect){//timer0 interrupt 2kHz toggles pin 8
/*Steps the stepper motor. 2000 times a second: 
1600 steps per revolution => 1600/2000 = 0,8 seconds per revolution.
Maybe we need a it to be a bit faster.  */ 
	cli();	//disable interrupts while we are here
	step(); 
	sei(); //Allow interrupts
}
