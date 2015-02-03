#include <math.h>
//X og Y koordinater for beacons
#define XA 0
#define YA 100

#define XB 300
#define YB 200

#define XC 300
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

 private:
 	//Different vectors only 
 	float AB;
 	float AC; 
 	float BC;
 	float AR;
 	float BR;
 	float CR;

 	//verctor between beacons
 	float BAC;
 	float CBA;
 	float ACB;

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
