#include "marioKalman.h"

/*
	Predict
	x = A*state + 
	P = A*P*A' + Q

	Update
	K = P*C*(C*P*C'+R)

	y=C*state 
	state = x + K*(y-C*lastState)
	P = (I-K*C)*P

	a = state(0:3, 0);
*/

marioKalman::marioKalman() {
	lastState = state;
	aXv = 0;
	aYv = 0;
}

/*Should initialize the state matrix and covariance matrix*/
void marioKalman::initKalman() {
	lastState = state;
	aXv = 0;
	aYv = 0;
}

void marioKalman::predict() {
	lastState = state;
	x = A*lastState+B*action; //prediction of the position
	P = A*P*A.t()+Q; //.t() => transposes the matrix
}

void marioKalman::update() {
 	//Kalman gain
 	K = P*H*(H*P*H.t()+R).i();
 	//Z should be updated each time new values are
 	// calculated from the beaconsystem
 	state = x + K*(Z-H*lastState);
 	P = (eye(3,3)-K*H)*P;
}

//Calculate velocity from position. Velocity in X and Y
void marioKalman::velocity() {
	aXv = (state(0,0)-lastState(0,0))/TIMESTEP;
	aYv = (state(1,0)-lastState(1,0))/TIMESTEP;

	/*OR IF ABSOLUTE VELOCITY*/
	v = ((state(0,0)-lastState(0,0))+(state(1,0)-lastState(1,0)));
	TIME +=TIMESTEP;
}

int main() {
	initKalman(); 
	std::cout << P(0,0) << std::endl;
	std::cout << state << std::endl;
}