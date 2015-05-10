#include <iostream>
#include <armadillo>
#include <zmq.hpp>
#include <stdlib.h>
//#include "serial.h"

#define TIMESTEP 0.05; 
#define N 3

//namespace for armadillo matrix library
using namespace arma;

//Prepare the context and socket
/*zmq::context_t context(1);
zmq::socket_t socket (context, ZMQ_REP);
socket.bind("tcp://*:5555");
*/

/*Create a Kalman filter class*/
class marioKalman {
	public: 
		marioKalman();
		void initKalman();
		void predict();
		void update();
		void velocity();
		void setMeasure(float, float, float);
		void setAmatrix(mat, float[][N]);
		void matrix(mat, float[][N]);
		void setBmatrix(mat);
		void queuePosition();
		void receivePosition();
		void setState(mat);
		mat getState();
		mat getMeasures();
		
	protected:
		float aXv, aYv, v;
		float TIME;
		mat x; 
		mat state; //Posx, Posy, Heading 
		mat lastState; //previouse state 
		mat action; //the action (movement from encoders)
		mat A; //A matrix (transition matrix)
		mat B; //B matrix (transition matrix for the action)
		mat Z; //Measurement matrix (Beacon position)
		mat P; //covariance matrix
		mat Q; // calcpos uncertainty matrix (put the error here)
		mat R; //Measurement uncertainty matrix
		mat H; //Measurement likelihood matrix
		mat K;//kalman gain
};