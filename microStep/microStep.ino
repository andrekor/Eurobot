/*
	IR Remote benytter interrupt kanal 2, derfor kan vi bare benytte kanal 0 og 1
*/
#include <Stepper.h>
#include <IRremote.h>
#include "tienstra.h"

#define MICRO_DELAY 300 //The delay are not used inside the interrupt
#define oneRevolution 1600 //Muligens må endre dette. Fra instructables.com ....

#define dirPin 7 //the pin that co32480ntrols the direction of the steppermotor
#define stepPin 8 //Output pin for the steppermotor

#define RECV_PIN 12

#define testPin 11

#define VALUE_BEACON_A 32480
#define VALUE_BEACON_B 338
#define VALUE_BEACON_C 339

//Setting ip the IR receiver
IRrecv irrecv(RECV_PIN);
decode_results results;

int stepCount;

int firstAstep = -1; //First step when the receiver recognize beacon A
int firstBstep = -1; //First step when the receiver recognize beacon B
int firstCstep = -1; //First step when the receiver recognize beacon C
int aSteps = -1; //steps from start to beacon A
int bSteps = -1;  //steps from start to beacon B
int cSteps = -1; //steps from start to beacon C
float resolution;

float averageA = 0;
float averageB = 0;
float averageC = 0;

int counter = 0;
int a_counter = 0;
int b_counter = 0;
int c_counter = 0;

int numAngles = 0; //Number of beacons seen that are been calculated, alpha, beta, gamma

boolean towerStop = false;
Tienstra *t;

void setup() {
	t = new Tienstra();
	t->initialization();
//	setVariablesZero();
	//Setup the ir receiver
	irrecv.enableIRIn();

	//Setup for steppermotorprin
	pinMode(testPin, INPUT);
	pinMode(dirPin, OUTPUT); //output mode to direction pin
	pinMode(stepPin, OUTPUT); //output mode to step pin
	pinMode(13, OUTPUT);
	digitalWrite(dirPin, LOW); //Initialize dir pin
	digitalWrite(stepPin, LOW); //Initialize step pin

	resolution = 360.0/oneRevolution;
	stepCount = 0; 
	Serial.begin(9600);

	//Setup the interrupt
	 //set timer0 interrupt at 2kHz
	//interruptSetup();
	//testRun2();
	//testBeacon();
	//hallo();
	widthTest();
	//irTest();
}


void irTest() {
	while(1)
		decode();
}

void hallo() {
	while(1) {
		while(1) {
			step();
			receiveBeaconSignal();
			if (stepCount > oneRevolution) {
				stepCount = 0;
				int d = !digitalRead(dirPin);
				digitalWrite(dirPin, d);
				break;
			}
		}
	Serial.print("First a: ");
	Serial.println(firstAstep);
	Serial.print("A step: ");
	Serial.println(aSteps);
	Serial.println();
	delay(2000);
	}
}

//To setup the interrupts. 
void interruptSetup() {
	cli(); //stops interrupts
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


  	//Set timer1 interrupt at 4000 hz
  	TCCR1A = 0; //Set entire TCCR2A register to 0
  	TCCR1B = 0; // same for TCCR2B
  	TCNT1 = 0; //Initialize counter value to 0
  	//Set compare match register for 16 KHz increments
  	OCR1A = 1999; // (16*10^6)/(1000*8) -1 (must be < 65000)
  	//Turn on CTC mode 
  	TCCR1B |= (1 << WGM12);
  	//Set CS12 for 8 bit prescaler
  	TCCR1B |= (1 << CS11);
  	//enable timer compare interrupt
  	TIMSK1 |= (1 << OCIE2A);
  	sei(); //allow interrupts
}

int i = 0;
void loop() {
}	

/*
Test program. Turns the tower one and a half round, and calculate the angle of the beacons. 
*/
void testRun() {
	while(1) {
		step(); //steps one step of the stepper 1/1600
		//checks for beacon signal one time per two steps 2/1600 = 1/800 = 0,45 degrees resolution
		receiveBeaconSignal();
		//Breaks after 1 round
		if (stepCount == 1601)
			break;
	}
	/*
	Serial.print("A ");
	Serial.print(aSteps);
	Serial.print(" - ");
	Serial.println(angle(aSteps));
	Serial.print("B ");
	Serial.print(bSteps);
	Serial.print(" - ");
	Serial.println(angle(bSteps));
	Serial.print("C ");
	Serial.print(cSteps);
	Serial.print(" - ");
	Serial.println(angle(cSteps));
	*/
	Serial.println("Angle on the beacons:");
	Serial.println(angle(aSteps));
	Serial.println(angle(bSteps));
	Serial.println(angle(cSteps));
	delay(5000);
	aSteps = 0; bSteps = 0; cSteps = 0;
	stepCount = 0; //nullstiller stepcount
	int dir = !digitalRead(dirPin);
	digitalWrite(dirPin, dir);
	while(1) {
		step();
		receiveBeaconSignal();
		if (stepCount == 1601)
			break;
	}
	Serial.print("A ");
	Serial.print(aSteps);
	Serial.print(" last A ");
	Serial.print(firstAstep);
	Serial.print(" -> ");
	Serial.println(angle(aSteps));
	Serial.print("B ");
	Serial.print(bSteps);
	Serial.print(" last A ");
	Serial.print(firstAstep);
	Serial.print(" -> ");
	Serial.println(angle(bSteps));
	Serial.print("C ");
	Serial.print(cSteps);
	Serial.print(" last A ");
	Serial.print(firstAstep);
	Serial.print(" -> ");
	Serial.println(angle(cSteps));

}

//Run the stepper one round, and read the angles. Then calculate the position
void testRun2() {
	while(1) {
		while(1) {
			receiveBeaconSignal();
			step();
			if (stepCount >= oneRevolution) {
				break;
			}
		}
		setAverage();
		if (digitalRead(dirPin))
			angleNegative();
		else 
			anglePositive();

		if (numAngles == 3) {
			Serial.println("\n Calculateing the position: ");

			t->calculate();
		}
		//Prints the position
		Serial.println("Position");
		Serial.print(t->XR);
		Serial.print(", ");
		Serial.println(t->YR);
		Serial.println();
		Serial.print("gamma : ");
		Serial.println(t->gamma);
		Serial.print("beta : ");
		Serial.println(t->beta);
		Serial.print("alpha : ");
		Serial.println(t->alpha);

		Serial.print("First A step: ");
		Serial.print(firstAstep);
		Serial.print(" angle first: ");
		Serial.print(angle(firstAstep));
		Serial.print(" last step in A: ");
		Serial.print(aSteps);
		Serial.print(" angle last ");
		Serial.println(angle(aSteps));
		Serial.print(" Average Step A ");
		Serial.print(averageA);
		Serial.print(" * Angle from start pos average: ");
		Serial.println(angle(averageA));
		Serial.print(" num a steps: ");
		Serial.println(a_counter);

		Serial.print("First B step: ");
		Serial.print(firstBstep);
		Serial.print(" last step in B: ");
		Serial.print(bSteps);
		Serial.print(" angle from start pos (average: ");
		Serial.print(angle(averageB));
		Serial.print(" num steps: ");
		Serial.println(b_counter);

		Serial.print("First C step: ");
		Serial.print(firstCstep);
		Serial.print(" last step in C: ");
		Serial.print(cSteps);
		Serial.print(" angle from start pos (average: ");
		Serial.print(angle(averageC));
		Serial.print(" num steps: ");
		Serial.println(c_counter);

		/*
		Serial.print("Angle to beacon A: ");
		Serial.println(angle(aSteps));
		Serial.print("Angle to beacon B: ");
		Serial.println(angle(bSteps));
		Serial.print("Angle to beacon C: ");
		Serial.println(angle(cSteps));
*/
		int dir = !digitalRead(dirPin);
		digitalWrite(dirPin, dir);
		stepCount = 0;
		zeroCounters();
		delay(3500);
	}
}

float average_angle(float first, float last) {
	return (first+last)/2;
}

void testBeacon() {
	while(1) {
		if (digitalRead(testPin)) {
		while(firstAstep < 0 && stepCount < oneRevolution){
			step();
			receiveBeaconSignal();
		}	

		Serial.print("First A step ");
		Serial.print(firstAstep);
		Serial.print("   =>   ");
		Serial.println(angle(firstAstep));
		Serial.println();

//		delay(3000);

		while(firstBstep < 0 && stepCount < oneRevolution){
			step();
			receiveBeaconSignal();
		}	

		Serial.print("First B step ");
		Serial.print(firstBstep);
		Serial.print("   =>   ");
		Serial.println(angle(firstBstep));
		Serial.println();

	//	delay(3000);

		while(firstCstep < 0 && stepCount < oneRevolution){
			step();
			receiveBeaconSignal();
		}	

		Serial.print("First C step ");
		Serial.print(firstCstep);
		Serial.print("   =>   ");
		Serial.println(angle(firstCstep));

	//	delay(500);

	//Steps to the start position
		while(stepCount < oneRevolution) 
			step();
		zeroCounters();
		Serial.println("Ready for next test");
	}
	int d = !digitalRead(dirPin);
	digitalWrite(dirPin, d);
	}
}

void zeroCounters() {
	aSteps = -1;
	bSteps = -1;
	cSteps = -1;
	firstAstep = -1;
	firstBstep = -1;
	firstCstep = -1;
	stepCount = -1;
	a_counter = 0;
	b_counter = 0;
	c_counter = 0;
	averageA = 0;
	averageB = 0;
	averageC = 0;
	numAngles = 0;
}

void widthTest() {
	while(1) {
		if (digitalRead(testPin)){
			stepCount = 0;
			int d = !digitalRead(dirPin);
			digitalWrite(dirPin, d);
			while (stepCount < oneRevolution) {
				step();
				receiveBeaconSignal();
			}

			Serial.print("first A step ");
			Serial.print(firstAstep);
			Serial.print("   =>   ");
			Serial.println(angle(firstAstep));

			Serial.print("A step ");
			Serial.print(aSteps);
			Serial.print("   =>   ");
			Serial.println(angle(aSteps));
			Serial.print("number of signals : ");	
			Serial.println(a_counter);
			zeroCounters();
		}
	}
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

/*
To make one step with the stepper motor
*/
void step() {
	//Maybe need to or in the value for the pin
	digitalWrite(stepPin, HIGH);
	digitalWrite(stepPin, LOW);
	delayMicroseconds(MICRO_DELAY);
	stepCount++;
}

/*Method that receives the IR signals, and detects 
from which beacon the signal comes from*/
void receiveBeaconSignal() {
  if (irrecv.decode(&results)) {
  	int value = results.value; //lagrer IR-koden
  	int length = results.bits;
  	Serial.println(value);
 // 	Serial.print(value);
 // 	Serial.print("  ");
 // 	Serial.println(length);
    //The beacon towers are coded with a value from 336, 338, 339
    switch (value) {
        case VALUE_BEACON_A:
 			setStep(1);
 			a_counter++;
          break;
        case VALUE_BEACON_B:
        	setStep(2);
        	b_counter++;
          //bSteps = stepCount;
          break;
        case VALUE_BEACON_C:
        	setStep(3);
        	c_counter++;
        	//cSteps = stepCount;
        	break;
        default:
        	break;
    	}
    } 
    //continue to look for more beacons. 
	irrecv.resume();
} 


/*
steps will be the last step in the intervall where the tower sees the beacon. 
*/
void setStep(int beacon) {
	if (beacon == 1) {
		aSteps = stepCount%oneRevolution; 
		if (firstAstep < 0) {//first time on this round that we receive a signal from the given beacon
			firstAstep = aSteps; 
			numAngles++; 
		}
	} else if (beacon == 2) {
		bSteps = stepCount;
		if (firstBstep < 0) {
			firstBstep = bSteps;
			numAngles++; 
		}
	} else {
		cSteps = stepCount;
		if (firstCstep < 0) {
			firstCstep = cSteps;
			numAngles++; 	
		}
	}
}

/*Calculates the angle in degree, when we know the how many steps we have gone*/
float calcDegree(int steps) {
	return steps*(oneRevolution/360);
}

/*
A->C->B->A
Starter ved høyeste verdi..
*/
void setAverage() {
	if (abs(firstAstep-aSteps) < 1000)
		averageA = average_angle(firstAstep, aSteps);
	else 
		averageA = average_angle((oneRevolution-max(firstAstep, aSteps)), min(firstAstep, aSteps));
	if (abs(firstBstep-bSteps) < 1000)
		averageB = average_angle(firstBstep, bSteps);
	else 
		averageB = average_angle((oneRevolution-max(firstBstep, bSteps)), min(firstBstep, bSteps));
	if (abs(firstCstep-cSteps) < 1000)
		averageC = average_angle(firstCstep, cSteps);
	else 
		averageC = average_angle((oneRevolution-max(firstCstep, cSteps)), min(firstCstep, cSteps));
}

void anglePositive() {
	if (averageA > averageB && averageA > averageC) {
		//A er størst, altså konfigurasjon null mellom A og C
		t->alpha = angle(averageC-averageB);
		t->beta = angle(averageA-averageC);
		t->gamma = angle(averageB+(oneRevolution-averageA));
	} else if (averageB > averageC) {
		//B er størst, altså null konfigurasjon mellom B og A
		t->beta = angle(averageA-averageC);
		t->gamma = angle(averageB-averageC);
		t->alpha = angle(averageC+(oneRevolution-averageB));
		//t->gamma = angle(averageA+(1600-averageB));
	} else {
		//C er størst, altså null konfigurasjon mellom C og B
		t->gamma = angle(averageB-averageA);
		t->alpha = angle(averageC-averageB);
		t->beta = angle(averageA+(oneRevolution-averageC));
	}
}

/*
A->B->C->A
Starter ved høyeste verdi..
*/
void angleNegative() {
	if (aSteps > averageB && aSteps > averageC) {
		//A er størst, altså konfigurasjon null mellom A og C
		t->alpha = angle(averageB-averageC);
		t->gamma = angle(averageA-averageB);
		t->beta = angle(averageC+(oneRevolution-averageA));
	} else if (averageB > averageC) {
		//B er størst, altså null konfigurasjon mellom B og A
		t->beta = angle(averageC-averageA);	
		t->alpha = angle(averageB-averageC);
		t->gamma = angle(averageA+(oneRevolution-averageB));
	} else {
		//C er størst, altså null konfigurasjon mellom C og B
		t->gamma = angle(averageA-averageB);
		t->beta = angle(averageC-averageA);
		t->alpha = angle(averageB+(oneRevolution-averageC));
	}
}

float angle(int steps) {
	/*Serial.print(steps);
	Serial.print(" * 360 /");
	Serial.println(oneRevolution);*/
	return (steps*resolution);
}

void decode() {
	if (irrecv.decode(&results)) {
		//Serial.println(results.value);
		//Serial.println(results.bits);
  		int value = results.value;
		if (value > 0 && value < 4) {
			Serial.println(value);
  //  		towerStop = true;
    	/*	switch (value) {
    	    case VALUE_BEACON_A:
    	      	aSteps = stepCount;
    	      	break;
    	   	case VALUE_BEACON_B:
    	      	bSteps = stepCount;
    	      	break;
    	    case VALUE_BEACON_C:
    	    	averageC = stepCount;
    	    	break;
    	    default:
    	    	break;
    		}*/
  		}
  	}
}
//Commented out the interrupts that disabled the delay function in arduino
//Interrupt service routine for Timer0, at 2KHz Used by the arduion
//ISR(TIMER0_COMPA_vect){//timer0 interrupt 2kHz toggles pin 8
/*Steps the stepper motor. 2000 times a second: 
1600 steps per revolution => 1600/2000 = 0,8 seconds per revolution.
Maybe we need a it to be a bit faster.  */ 
//	cli();	//disable interrupts while we are here
	//step(); 
//	sei(); //Allow interrupts
//	step();
	/*Use the same interrupt for the receive 
	beacon, but with frequence of 2KHz/4=500Hz*/
	
	//step();
//}

/* Cannot use timer2 since its alerady used by the 
IRremote library */
/*
ISR(TIMER1_COMPA_vect) {
//	receiveBeaconSignal();
	//decode();
}
*/
