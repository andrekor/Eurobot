/*
	IR Remote benytter interrupt kanal 2, derfor kan vi bare benytte kanal 0 og 1
*/
#include <Stepper.h>
#include <IRremote.h>
#include "tienstra.h"

#define MICRO_DELAY 400 //The delay are not used inside the interrupt
#define oneRevolution 1600 //Muligens må endre dette. Fra instructables.com ....

#define dirPin 7 //the pin that controls the direction of the steppermotor
#define stepPin 8 //Output pin for the steppermotor

#define RECV_PIN 12

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

int firstAstep = -1; //First step when the receiver recognize beacon A
int firstBstep = -1; //First step when the receiver recognize beacon B
int firstCstep = -1; //First step when the receiver recognize beacon C
int aSteps = -1; //steps from start to beacon A
int bSteps = -1;  //steps from start to beacon B
int cSteps = -1; //steps from start to beacon C
float resolution;
int counter = 0;

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
	testBeacon();
	//irTest();
}

void irTest() {
	while(1)
		decode();
}

void hallo() {
	while(1) {
	step();
	if (stepCount >= 1600) {
		stepCount = 0;
		int dir = !digitalRead(dirPin);
		digitalWrite(dirPin, dir);
	}

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
void loop() {}	

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
			step();
			receiveBeaconSignal();
			if (stepCount == oneRevolution) {
				break;
			}
		}
		if (digitalRead(dirPin))
			anglePositive();
		else 
			angleNegative();

		numAngles = 0;
		t->calculate();
		//Prints the position
		Serial.println("\n Position");
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

		delay(5000);
	}
}

void testBeacon() {
	while(1){
		step();
		receiveBeaconSignal();
		//we have got'n a signal from beacon and we break the loop
		if (aSteps >= 0 || stepCount > 2*oneRevolution) 
			break;
	}	

	Serial.print("first A step ");
	Serial.print(firstAstep);
	Serial.print("   =>   ");
	Serial.println(angle(firstAstep));

	Serial.print("A step ");
	Serial.print(aSteps);
	Serial.print("   =>   ");
	Serial.println(angle(aSteps));	

	delay(300);

	while(1){
		step();
		receiveBeaconSignal();
		//we have got'n a signal from beacon and we break the loop
		if (bSteps >= 0 || stepCount > 2*oneRevolution) 
			break;
	}	

	Serial.print("first B step ");
	Serial.print(firstBstep);
	Serial.print("   =>   ");
	Serial.println(angle(firstBstep));

	Serial.print("B step ");
	Serial.print(bSteps);
	Serial.print("   =>   ");
	Serial.println(angle(bSteps));	

	delay(3000);

	while(1){
		step();
		receiveBeaconSignal();
		//we have got'n a signal from beacon and we break the loop
		if (cSteps >= 0 || stepCount > 2*oneRevolution) 
			break;
	}	

	Serial.print("first C step ");
	Serial.print(firstCstep);
	Serial.print("   =>   ");
	Serial.println(angle(firstCstep));

	Serial.print("C step ");
	Serial.print(cSteps);
	Serial.print("   =>   ");
	Serial.println(angle(cSteps));	
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
	/*if (digitalRead(dirPin)) //Positiv retning på stepper
		stepCount++;	
	else
		stepCount++;
		*/
}

/*Method that receives the IR signals, and detects 
from which beacon the signal comes from*/
void receiveBeaconSignal() {
  if (irrecv.decode(&results)) {
  	int value = results.value; //lagrer IR-koden
  	int length = results.bits;
 // 	Serial.print(value);
 // 	Serial.print("  ");
 // 	Serial.println(length);
    //The beacon towers are coded with a value from 1 -> 3
    if (value > 0 && value < 4) {
    	switch (value) {
    	    case VALUE_BEACON_A:
 				setStep(1);
    	      break;
    	    case VALUE_BEACON_B:
    	    	setStep(2);
    	      //bSteps = stepCount;
    	      break;
    	    case VALUE_BEACON_C:
    	    	setStep(3);
    	    	//cSteps = stepCount;
    	    	break;
    	    default:
    	    	break;
    	}
    	//Serial.println(aSteps);
      //Sende the 0 for beacon 1, 1 for beacon 2, and 2 for beacon 3
      //So that i can get the position directly from the array
  //    readAngleOfBeacon(value-1); 
    }//continue to look for more beacons. 
    irrecv.resume();
  } 
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
		if (firstCstep < 0) {Serial.print(" last A ");
	Serial.print(firstAstep);
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
void anglePositive() {
	if (aSteps > bSteps && aSteps > cSteps) {
		//A er størst, altså konfigurasjon null mellom A og C
		t->gamma = angle(aSteps-bSteps);
		t->alpha = angle(bSteps-cSteps);
		t->beta = angle(cSteps+(1600-aSteps));
	} else if (bSteps > cSteps) {
		//B er størst, altså null konfigurasjon mellom B og A
		t->alpha = angle(bSteps-cSteps);
		t->beta = angle(cSteps-aSteps);
		t->gamma = angle(aSteps+(1600-bSteps));
	} else {
		//C er størst, altså null konfigurasjon mellom C og B
		t->beta = angle(cSteps-aSteps);
		t->gamma = angle(aSteps-bSteps);
		t->alpha = angle(bSteps+(1600-cSteps));
	}
}

/*
A->B->C->A
Starter ved høyeste verdi..
*/
void angleNegative() {
	if (aSteps > bSteps && aSteps > cSteps) {
		//A er størst, altså konfigurasjon null mellom A og B
		t->gamma = angle(aSteps-cSteps);
		t->alpha = angle(cSteps-bSteps);
		t->beta = angle(bSteps+(1600-aSteps));
	} else if (bSteps > cSteps) {
		//B er størst, altså null konfigurasjon mellom B og C
		t->alpha = angle(bSteps-aSteps);
		t->beta = angle(aSteps-cSteps);
		t->gamma = angle(cSteps+(1600-bSteps));
	} else {
		//C er størst, altså null konfigurasjon mellom C og A
		t->beta = angle(cSteps-bSteps);
		t->gamma = angle(bSteps-aSteps);
		t->alpha = angle(aSteps+(1600-cSteps));
	}
}

float angle(int steps) {
	/*Serial.print(steps);
	Serial.print(" * 360 /");
	Serial.println(oneRevolution);*/
	return (steps*resolution);
}

void decode() {
	Serial.println("yolo");
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
    	    	cSteps = stepCount;
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
