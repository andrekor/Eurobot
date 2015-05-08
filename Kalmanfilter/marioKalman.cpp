#include "marioKalman.h"

/*The Flow of the Kalman filter
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
	A.eye();
	B.eye();
	Z.zeros();
	B = mat(3,3); //B matrix (transition matrix for the action)
	Z = mat(3,1); //Measurement matrix (Beacon position)
	P = eye(3,3); //covariance matrix
	Q = eye(3,3); // calc-pos uncertainty matrix (put the error here)
	R = eye(3,3); //Measurement uncertainty matrix
	H = eye(3,3);
	K = mat(3,3);//kalman gain
	K.eye();
	lastState = state;
	aXv = 0;
Kalmanestimate:
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
	float q[N][N] = {{0.064, 0, 0}, {0, 0.064, 0}, {0, 0, 0.025}};
	float r[N][N] = {{5, 0, 0}, {0, 5, 0}, {0, 0, 0.5}};
	//std::cout << "a: " << sizeof(a) << " float: " << sizeof(float) << " a[1] " << sizeof(a[1]) << std::endl;
	for (int i = 0; i < (sizeof(a[1])/sizeof(float)); i++) {
		for (int j = 0; j < (sizeof(a[1])/sizeof(float)); j++) {
			//std::cout << i << ", " << j << std::endl;
			R(i, j) = r[i][j];
			Q(i, j) = q[i][j];
			//A(i, j) = a[i][j];
		}
	}
}

void marioKalman::predict() {
	//std::cout << "Predict part: " << std::endl;
	lastState = state;
	x = A*lastState; //+B*action; //prediction of the position
//	std::cout << A << " * " << lastState << std::endl;
	P = A*P*A.t()+Q; //Q-is the noise
	//std::cout << P << " + " << Q << " (noise)" << std::endl;
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

	/*For absolute velocity*/
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


void marioKalman::setState(mat initState) {
	lastState = initState;
	state = initState;
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

void kalmanTest(marioKalman *m) {
	mat wantedPath(16, 3); //fixed datapoints..
	mat initState(3,1);
	float test[17][3] = {{50, 0.0, 0.0},{50.0,0.0,90.0},{50.0,20.0,90.0},{50.0,40.0,90.0},{55.0,50.0,45.0},{65.0,60.0,45.0},{75.0,65.0,20.0},{80.0,65.0,0.0},{90.0,65.0,0.0},{110.0,65.0,0.0},{130.0,65.0,0.0},{145.0,55.0,-45.0},{150.0,45.0,-90.0},{150.0,35.0,-90.0},{150.0,20.0,-90.0},{150.0,0.0,-90.0},{150.0,0.0,0.0}};
	initState(0,0) = test[0][0]; initState(1,0) = test[0][1]; initState(2,0) = test[0][2];
	//float *test[3] = {{4.0, 3.0, 3.0}, {4.0, 4.0, 4.0}};
	//float *test[3] = {{50,0,90},{50,20,90},{50,40,90},{55,50,45},{65,60,45},{75,65,20},{80,65,0},{90,65,0},{110,65,0},{130,65,0},{145,55,-45},{150,45,-90},{150,35,-90},{150,20,-90},{150,0,-90},{150,0,0}};
	//for (int i = )
	m->initKalman();
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 3; j++) {
			wantedPath(i,j) = test[i][j];
		}
	}
	mat saveState(16, 3);
	//16 iterations for the kalman filter
	m->setState(initState);
	mat temp(3, 1);
	temp.zeros();
	for (int i = 0; i <= 16; i++) {
		m->setMeasure(test[i+1][0]+(rand()%20), test[i+1][1]+(rand()%10), test[i+1][2]+(rand()%5));
		m->predict();
		m->update();
		//Her we apply an action to the encoders so (lastState - wantedState) => tells how much to drive
		temp(0, 0) = test[i+1][0]; temp(1, 0) = test[i+1][1]; temp(2, 0) = test[i+1][2];
		std::cout << "The " << i << " iteration" << std::endl;
		std::cout << "Kalmanfilter " << std::endl << m->getState() << std::endl; //state from Kalmanfilter
		std::cout << "Measure from beacon " << std::endl << m->getMeasures() << std::endl;
		std::cout << "Wanted path " << std::endl << temp << std::endl;
		mat K = m->getState();
		std::cout << "[" << K(0) << ";" << K(1) << ";" << K(2) << "]" << std::endl;
		m->setState(temp);
	}
	mat pos = m->getState();

}

int getArguments(std::string input, int *pos) {
	int i = 0; 
	std::istringstream f(input); 
	std::string s; 
	while(getline(f, s, ',')) { 
		pos[i] = atoi(s.c_str()); 
		i++; //
	}
	return i;
}

/*
int main() {
	//marioKalman *m = new marioKalman();
	//kalmanTest(m);
	Serial *s = new Serial();
	while(1) {
		s->readLine();
	}
	server();
//int p[3];
//getArguments("3,2,5", p);
//
//	for (int i = 0; i < sizeof(p)/sizeof(*p); i++) {
		//std::cout << p[i] << std::endl;
	//}

	//m->initKalman();
	//std::cout << m->getState() << std::endl;/
	//m->setMeasure(10, 10, 10);
	//std::cout << m->getMeasures() << std::endl;

	return 0;
}*/