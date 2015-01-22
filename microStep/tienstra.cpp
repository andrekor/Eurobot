#include "tienstra.h"

Tienstra::Tienstra() {
    //position off Robot
   XR = 0;
   YR = 0;
   theta = 0;

   //Different vectors
   AB = 0;
   AC = 0;
   BC = 0;
   AR = 0;
   BR = 0;
   CR = 0;

   //Angles we read from the stepper
   alpha = 0;
   beta = 0;
   gamma = 0;

   //Angles between the beacons
   angleA = 0;
   angleB = 0;
   angleC = 0;
   BAC = 0;
   CBA = 0;
   ACB = 0;

   //cotangents of the angles
   cot_a = 0;
   cot_b = 0;
   cot_c = 0;
   cot_alpha = 0;
   cot_beta = 0;
   cot_gamma = 0;

   //Tienstra scalers
   KA = 0;
   KB = 0; 
   KC = 0;
   K = 0;  
}


/*Makes the vectors between the known beacons, and the angles between them*/
void Tienstra::initialization() {
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

/*Should calculate the position of the robot when knowing alpha, beta and gamma*/
void Tienstra::calculate() {
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
   /*
   Serial.print(XR);
   Serial.print(",");
   Serial.print(YR);
   Serial.print("    BAC -> ");
   Serial.print(BAC);
   Serial.print("   CBA -> ");
   Serial.print(CBA);
   Serial.print("   ACB -> ");
   Serial.println(ACB);
*/
   //Benytter vinkel til beacon c, kunne hvert fra en av de andre. (burde bare ta en vi har tilgjengelig)
  // float thetaR = atan2(YC-YR, XC-XR) - (M_PI/2); //should be thetaC, and not pi/2
   //Serial.println(thetaR);
}


/*
Takes in the position of the robot, the position of the beacon, and the angle of the beacon
Returns the angle of the robot. 
*/
float robotAngle(float yi, float yr, float xi, float xr, float thetai) {
   return atan2(yi-yr, xi-xr) - thetai;
}