#include <iostream>
#include <armadillo>

#define TIMESTEP 0.05;

using namespace arma;

/*Create a Kalman filter class*/
class marioKalman {
	public: 
		marioKalman();
		void predict();
		void update();
		void velocity();
		
	protected:
		float aXv, aYv, v;
		float TIME=0;
		mat x = mat(5, 1); 
		mat state = mat(5, 1); //Posx, Posy, Heading 
		mat lastState = mat(5, 1); //previouse state 
		mat action = mat(5, 1); //the action (movement from encoders)
		mat A = mat(5,5); //A matrix (transition matrix)
		mat B = mat(5,5); //B matrix (transition matrix for the action)
		mat Z = mat(5,1); //Measurement matrix (Beacon position)
		mat P = eye(5,5); //covariance matrix
		mat Q = eye(5,5); // calcpos uncertainty matrix (put the error here)
		mat R = eye(5,5); //Measurement uncertainty matrix
		mat H = eye(5,5);
		mat K = mat(5,5);//kalman gain
};