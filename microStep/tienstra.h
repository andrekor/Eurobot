#include <math.h>
//X og Y koordinater for beacons
#define XA 0
#define YA 25

#define XB 100
#define YB 50

#define XC 100
#define YC 0

class Tienstra {
public:
	Tienstra();
	void initialization();
	void calculate();
	float robotAngle(float yi, float yr, float xi, float xr, float thetai);
 	float XR;
 	float YR;
 	float theta;

	//Angles we read from the stepper
 	float alpha;
 	float beta;
 	float gamma;

	//Angles between the beacons
 	float angleA;
 	float angleB;
 	float angleC;
 	float BAC;
 	float CBA;
 	float ACB;

 private:
 	//Different vectors
 	float AB;
 	float AC; 
 	float BC;
 	float AR;
 	float BR;
 	float CR;

 	//Tienstra scalers
 	float KA;
 	float KB; 
 	float KC;
 	float K; 

 	//cotangents of the angles
 	float cot_a;
 	float cot_b;
 	float cot_c;
 	float cot_alpha;
 	float cot_beta;
 	float cot_gamma;
};
