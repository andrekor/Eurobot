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
	TIME = 0;
	x = mat(3, 1); 
	state = mat(3, 1); //Posx, Posy, Heading 
	lastState = mat(3, 1); //previouse state 
	action = mat(3, 1); //the action (movement from encoders)
	A = mat(3,3); //A matrix (transition matrix)
	B = mat(3,3); //B matrix (transition matrix for the action)
	Z = mat(3,1); //Measurement matrix (Beacon position)
	P = eye(3,3); //covariance matrix
	Q = eye(3,3); // calc-pos uncertainty matrix (put the error here)
	R = eye(3,3); //Measurement uncertainty matrix
	H = eye(3,3);
	K = mat(3,3);//kalman gain

	lastState = state;
	aXv = 0;
	aYv = 0;
}

/*Should initialize the state matrix and covariance matrix*/
void marioKalman::initKalman() {
	lastState = state;
	aXv = 0;
	aYv=0;
	float a[N][N] = {{1,0,0},{0,1,0},{0,0,1}}; //Identity matric
	float p[N][N] = {{1,0,0},{0,1,0},{0,0,1}}; //Identity matric. Will be updated at later point
	float h[N][N] = {{1,0,0},{0,1,0},{0,0,1}}; //Identity matric
	float q[N][N] = {{0.64, 0, 0}, {0, 0.64, 0}, {0, 0, 0.25}};
	float r[N][N] = {{0.44, 0, 0}, {0, 0.18, 0}, {0, 0, 0.6}};
	//std::cout << "a: " << sizeof(a) << " float: " << sizeof(float) << " a[1] " << sizeof(a[1]) << std::endl;
	for (int i = 0; i < (sizeof(a[1])/sizeof(float)); i++) {
		for (int j = 0; j < (sizeof(a[1])/sizeof(float)); j++) {
			//std::cout << i << ", " << j << std::endl;
			A(i, j) = a[i][j];
		}
	}
}

void marioKalman::predict() {
	lastState = state;
	x = A*lastState; //+B*action; //prediction of the position
	P = A*P*A.t()+Q; //Q-is the noise
}

void marioKalman::update() {
 	K = P*H*(H*P*H.t()+R).i();//Kalman gain
 	//Z should be updated each time calculating (use setMeasure)
 	state = x + K*(Z-H*lastState); //Z-H*lastState describe the error between measure, and prev position
 	P = (eye(3,3)-K*H)*P; //update the P matrix
}

//Calculate velocity from position. Velocity in X and Y
void marioKalman::velocity() {
	aXv = (state(0,0)-lastState(0,0))/TIMESTEP;
	aYv = (state(1,0)-lastState(1,0))/TIMESTEP;

	/*OR IF ABSOLUTE VELOCITY*/
	v = ((state(0,0)-lastState(0,0))+(state(1,0)-lastState(1,0)));
	TIME +=TIMESTEP;
}

/*
Sets the measure matrix. Should be filled with values from the 
beaconsystem. Should be called when new values are calculated
*/
void marioKalman::setMeasure(float xPos, float yPos, float theta) {
	Z(0,0) = xPos;
	Z(1,0) = yPos;
	Z(2,0) = theta;
}

/*Returns the values from the measure matrix Z*/
mat marioKalman::getMeasures() {
	return Z;
}

/*Returns the current state*/
mat marioKalman::getState() {
	return state;
}

/*
Sets the state transition matrix
Set it initialy to the Identity matrix.
Maybe this should be changed
*/
void marioKalman::matrix(mat matrix, float array[][N]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			matrix(i, j) = array[i][j];
		}	
	}
}

/**/
void marioKalman::setBmatrix(mat b) {
	B = b;
}

/*Puts the estimated position in the queue for the controll*/
void queuePosition() {
/*	zmq::message_t send;
	memcpy((void *) send.data(), "pos", 3);
	socket.send(send);*/
}

/*Get the encoder position from the queue*/
void receivePosition() {
/*	zmq::message_t request;
	socket.recv(&request);
	std::cout << request.data() << std::endl;
	*/
}

int main() {
	marioKalman *m = new marioKalman();
	m->initKalman();
	std::cout << m->getState() << std::endl;
	m->setMeasure(10, 10, 10);
	std::cout << m->getMeasures() << std::endl;

	return 0;
}