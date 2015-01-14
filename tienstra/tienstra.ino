#include <math.h>
//X og Y koordinater for beacons
#define XA 0
#define YA 1

#define XB 3
#define YB 2

#define XC 3
#define YC 0

//position off Robot
float XR = 0;
float YR = 0;
float theta = 0;

//Different vectors
float AB = 0;
float AC = 0;
float BC = 0;
float AR = 0;
float BR = 0;
float CR = 0;

//Angles we read from the stepper
float alpha = 0;
float beta = 0;
float gamma = 0;

//Angles between the beacons
float angleA = 0;
float angleB = 0;
float angleC = 0;
float BAC = 0;
float CBA = 0;
float ACB = 0;

//cotangents of the angles
float cot_a = 0;
float cot_b = 0;
float cot_c = 0;
float cot_alpha = 0;
float cot_beta = 0;
float cot_gamma = 0;

//Tienstra scalers
float KA = 0;
float KB = 0; 
float KC = 0;
float K = 0; 

void setup() {
   initialization();
   alpha = 180; //Hardcoded angles 
   gamma = 90; //Hardcoded angles
   beta = 90; //Hardcoded angles 
   Serial.begin(9600);
   calculate();
} 

void loop() {

}

void initialization() {
   AB = sqrt(pow((XA-XB), 2) + pow((YA-YB), 2)); //Lengden på vektor AB
   AC = sqrt(pow((XA-XC), 2) + pow((YA-YC), 2)); //Lengden av AC vektoren
   BC = sqrt(pow((XB-XC), 2) + pow((YB-YC), 2)); //Lengden av BC vektoren

   angleA = acos((pow(AB, 2) + pow(AC, 2) - pow(BC, 2))/(2*AB*AC)); 
   angleB = acos((pow(AB, 2) + pow(BC, 2) - pow(AC, 2))/(2*AB*BC));
   angleC = acos((pow(AC, 2) + pow(BC, 2) - pow(AB, 2))/(2*AC*BC));

   //Calculate cotagens angles
   cot_a = 1/tan(angleA);
   cot_b = 1/tan(angleB);
   cot_c = 1/tan(angleC);

}

void calculate() {
   //Calculation of the angles from the stepper motor. should be here...


   // .....
   //calculations of the position starts here..
   //Cotangents of angles
   float alphaRad = (alpha/180)*M_PI;
   float betaRad = (beta/180)*M_PI;
   float gammaRad = (gamma/180)*M_PI;
   cot_alpha = 1/tan(alphaRad);
   cot_beta = 1/tan(betaRad);
   cot_gamma = 1/tan(gammaRad);   

   //calculate scalers
   KA = 1/(cot_a - cot_alpha);
   KB = 1/(cot_b - cot_beta);
   KC = 1/(cot_c - cot_gamma);

   K = KA + KB + KC;

   //Calculate the complete coordinates
   XR = (KA*XA + KB*XB + KC*XC)/K;
   YR = (KA*YA + KB*YB + KC*YC)/K;

   //Calculate distance from point P
   AR = sqrt(pow((XA-XR), 2) + pow((YA-YR), 2));
   BR = sqrt(pow((XB-XR), 2) + pow((YB-YR), 2));
   CR = sqrt(pow((XC-XR), 2) + pow((YC-YR), 2));

   //Calculate angles in degrees
   BAC = angleA * 180 / M_PI;
   CBA = angleB * 180 / M_PI;
   ACB = angleC * 180 / M_PI;
   Serial.print(XR);
   Serial.print(",");
   Serial.print(YR);
   Serial.print("    BAC -> ");
   Serial.print(BAC);
   Serial.print("   CBA -> ");
   Serial.print(CBA);
   Serial.print("   ACB -> ");
   Serial.println(ACB);

   //Benytter vinkel til beacon c
   float thetaR = atan2(YC-YR, XC-XR) - (M_PI/2);
   Serial.println(thetaR);
}
