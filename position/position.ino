/*
	IR Remote benytter interrupt kanal 2, derfor kan vi bare benytte kanal 0 og 1
*/
#include <Stepper.h>
#include <IRremote.h>
//For testing, to map all of the angles
#include <QueueList.h>
#include "tienstra.h"

//1 if all the steps should be saved in Queue, 0 if not
//For debugging (width test)
#define AQueue 0
#define BQueue 0
#define CQueue 0
#define oneRevolution 1600 //Muligens må endre dette. Fra instructables.com ....
#define MICRO_DELAY 500

#define dirPin 7 //the pin that co32480ntrols the direction of the steppermotor
#define stepPin 8 //Output pin for the steppermotor

#define RECV_PIN 12

#define testPin 11

#define VALUE_BEACON_A 339 
#define VALUE_BEACON_B 338
#define VALUE_BEACON_C 32480
float realAverage(QueueList<float>);

//Setting ip the IR receiver
IRrecv irrecv(RECV_PIN);
decode_results results;

int stepCount;
//0 if not enough angles to calculate the position, 1 if enough
int firstAstep = -1; //First step when the receiver recognize beacon A
int firstBstep = -1; //First step when the receiver recognize beacon B
int firstCstep = -1; //First step when the receiver recognize beacon C
int lastAstep = -1; //steps from start to beacon A
int lastBstep = -1;  //steps from start to beacon B
int lastCstep = -1; //steps from start to beacon C

int numberAsample = 0;
int numberBsample = 0;
int numberCsample = 0;

/*Booleans to check wether we have should zero firstA or not*/
boolean A = true;
boolean B = true;
boolean C = true;

float resolution;
float averageA = 0;
float averageB = 0;
float averageC = 0;

int a_counter = 0;
int b_counter = 0;
int c_counter = 0;

int numAngles = 0; //Number of beacons seen that are been calculated, alpha, beta, gamma

unsigned long time;

boolean towerStop = false;
Tienstra *t;

// create a queue of strings messages.
QueueList <float> queueA;
QueueList <float> queueB;
QueueList <float> queueC;

void setup() {
	t = new Tienstra();
	t->initialization();
	
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
//	testMethod();
}

/*Test function for the second prototype, with slipring which allows the tower to 
rortate continualy in the same direction.
The function, steps, and sets the angle of each of the beacons. The direction is A -> B -> C -> A*/
void loop() {
	step();
	receiveBeaconSignal();
	//printBeacon();
	
}	

void printBeacon() {
	if (irrecv.decode(&results)) {
  		int value = results.value; //lagrer IR-koden	
  		switch (value) {
  		    case VALUE_BEACON_A:
  		    	numberAsample++;
  		      // do something
  		      break;
  		    case VALUE_BEACON_B:
  		      // do something
  		      numberBsample++;
  		      break;
  		    case VALUE_BEACON_C:
  		    	numberCsample++;
  		    break;
  		    default:
  		      // do something
  		    break;
  		}
  		irrecv.resume();
	}
}

/*Takes in the position, and sends it to serial, so that it can be plotted in a processing program*/
void livePlot(float x1, float y1) {
	String s1 = String(round(x1));
	String s2 = String(round(y1));
	String s3 = String(s1 + ",");
	Serial.println(s3 + s2);
}

void printStuff(int first, int average, int last, int num)  {
	Serial.print("[");
	Serial.print(first);	
	Serial.print(", ");
	Serial.print(average);
	Serial.print("," );
	Serial.print(last);
	Serial.print("]   [");
	Serial.print(angle(first));
	Serial.print(", ");
	Serial.print(angle(average));
	Serial.print("," );
	Serial.print(angle(last));
	Serial.print("]   ");
	Serial.println(num);
}

float average_angle(float first, float last) {
	return (first+last)/2;
}

void zeroCounters() {
	lastAstep = -1;
	lastBstep = -1;
	lastCstep = -1;
	firstAstep = -1;
	firstBstep = -1;
	firstCstep = -1;
	stepCount = -1;
	averageA = 0;
	averageB = 0;
	averageC = 0;
}

/*
To make one step with the stepper motor
*/
void step() {
	digitalWrite(stepPin, HIGH);
	digitalWrite(stepPin, LOW);
	stepCount = (stepCount == 1600 ? 0 : stepCount+1); //Resets after 1 revolution
	/*if (stepCount == 1600) {
		stepCount = 0; //resets stepcount after 1 round
		Serial.print("A samples: ");
		Serial.println(numberAsample);

		Serial.print("B samples: ");
		Serial.println(numberBsample);

		Serial.print("C samples: ");
		Serial.println(numberCsample);
		numberAsample = 0;
		numberBsample = 0;
		numberCsample = 0;
	} else {
		stepCount++;
	}*/
	delayMicroseconds(MICRO_DELAY);
}

/*Method that receives the IR signals, and detects 
from which beacon the signal comes from*/
void receiveBeaconSignal() {
  if (irrecv.decode(&results)) {
  	int value = results.value; //lagrer IR-koden
    switch (value) { //sjekker om vi har et gyldig IR-signal
        case VALUE_BEACON_A:
 			setStep(1);
          break;
        case VALUE_BEACON_B:
        	setStep(2);
          break;
        case VALUE_BEACON_C:
        	setStep(3);
          	break;
        default:
        	break;
   	}
   	   //continue to look for more beacons. 
    	irrecv.resume();
  } 
} 

/*
steps will be the last step in the intervall where the tower sees the beacon. 
*/
void setStep(int beacon) {
 	if (beacon == 1) {
		if (A) { //er true, hvis dette er første signal fra denne beaconen på denne runden
			A = false;
			zeroOldestBeacon(1);
			firstAstep = stepCount; 
		}
		lastAstep = stepCount;
	} else if (beacon == 2) {
		if (B) {
			B = false;
			zeroOldestBeacon(2);
			firstBstep = stepCount;
		}
		lastBstep = stepCount;
	} else {
		if (C) {
			C = false;
			zeroOldestBeacon(3);
			firstCstep = stepCount;
		}
		lastCstep = stepCount;
	}
}

/*Skal kunn komme inn her når vi får signal fra en ny beacon*/
void zeroOldestBeacon(int beacon) {
	//Zero the oldest beacon angles
	switch (beacon) {
		case 1: //if we see first signal from beacon A, we are allowed to look for B and C beacon. (calculate averageB)
			B = true;
			if (firstBstep > lastBstep)
				averageB = ((int)average_angle(firstBstep, oneRevolution + lastBstep))%1600;
			else 
				averageB = average_angle(firstBstep, lastBstep);
			/*Serial.print("B: ");
			Serial.print(firstBstep);
			Serial.print(", ");
			Serial.println(lastBstep);*/
			break;
		case 2: //If we see first signal from beacon B, we are allowed to look for A and C beacon. (calculate averageA)
			C = true;
			if (firstCstep > lastCstep) 
				averageC = ((int)average_angle(firstCstep, lastCstep+oneRevolution))%1600;
			else 
				averageC = average_angle(firstCstep, lastCstep);
			/*Serial.print("C: ");
			Serial.print(firstCstep);
			Serial.print(", ");
			Serial.println(lastCstep);*/
			break;	
		case 3: //If we see first signal from beacon C, we are allowed to look for A and B beacon. (calculate averageB)
			A = true;
			if (firstAstep > lastAstep)
				averageA = ((int)average_angle(firstAstep, oneRevolution + lastAstep))%1600;
			else 
				averageA = average_angle(firstAstep, lastAstep);
			/*Serial.print("A: ");
			Serial.print(firstAstep);
			Serial.print(", ");
			Serial.println(lastAstep);*/
			break;
		default:
			break;
	}
	anglePositive();
	t->calculate();
	//printAngles();
	/*Position*/
	printPos();
	//The angles between the beacons
	/*Serial.print("[alpha, beta, gamma]");
	Serial.print("[");
	Serial.print(t->alpha);
	Serial.print(", ");
	Serial.print(t->beta);
	Serial.print(", ");
	Serial.print(t->gamma);
	Serial.println("]");*/
		
	//IF the tower starts strait forward: takes in position of beacon we have a measure to
	//should use the beacon that gets correct angles the most
	//t->robotAngle(0, 100, averageA);
}


/*Calculates the angle in degree, when we know the how many steps we have gone*/
float calcDegree(int steps) {
	return steps*(oneRevolution/360);
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

void angleNegative() {
	if (averageA > averageB && averageA > averageC) {
		//A er størst, altså konfigurasjon null mellom A og C
		
		t->alpha = angle(averageB-averageC);
		t->gamma = angle(averageA-averageB);
		t->beta = angle(averageC+oneRevolution-averageA);
	} else if (averageB > averageC) {
		//B er størst, altså null konfigurasjon mellom B og A
		t->beta = angle(averageC-averageA);
		t->alpha = angle(averageB-averageC);
		t->gamma = angle(averageA+oneRevolution-averageB);
		//t->gamma = angle(averageA+(1600-averageB));
	} else {
		//C er størst, altså null konfigurasjon mellom C og B
		t->gamma = angle(averageA-averageB);
		t->beta = angle(averageC-averageA);
		t->alpha = angle(averageB+oneRevolution-averageC);
	}
}

float angle(int steps) {
	return (steps*resolution);
}
void printPos() {
	Serial.print(t->XR);
	Serial.print(", ");
	Serial.println(t->YR);
}

void printAngles() {
	Serial.print(t->alpha);
	Serial.print(", ");
	Serial.print(t->beta);
	Serial.print(", ");
	Serial.println(t->gamma);
}

void printSteps() {
	Serial.print("A: ");
	Serial.print(angle(averageA));
	Serial.print(",");
	Serial.print("B: ");
	Serial.print(angle(averageB));
	Serial.print(",");
	Serial.print("C: ");
	Serial.println(angle(averageC));
}

/*Method to send the average position to the computer. 
It turned out that the calculation of the position was a
bit heavy for the ardduino*/
/*void sendAverage() {
	String s1 = String(averageA);
	String s2 = String(s1+",");
	String s3 = String(s2+averageB);
	String s4 = String("," + averageC);
	String s5 = String(s3+s4);
	//averageA,averageB,averageC
	Serial.println(s5);
}*/

void testMethod() {
	int i = 0;
	while(i<9600) {
		step();
		receiveBeaconSignal();
		i++;
	}
}