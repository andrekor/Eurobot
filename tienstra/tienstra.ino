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
/* Må gjøres om til c++
// easting coordinates
var EA=0;
var EB=0;
var EC=0;

//northing coordinates
var NA=0;
var NB=0;
var NC=0;

//distances
var AB=0;
var AC=0;
var BC=0;
var AP=0;
var BP=0;
var CP=0;

//angles
var ANG_A=0;
var ANG_B=0;
var ANG_C=0;
var ANG_ALPHA=0;
var ANG_BETA=0;
var ANG_GAMMA=0;
var BAC=0;
var CBA=0;
var ACB=0;
var ALPHA=0;
var GAMMA=0;

//cotangents of angles
var COT_A=0;
var COT_B=0;
var COT_C=0;
var COT_ALPHA=0;
var COT_BETA=0;
var COT_GAMMA=0;

//Tienstra scalers
var KA=0;
var KB=0;
var KC=0;
var K=0;

//final coordinates
var E=0;
var N=0;

function clear_all() {
// easting coordinates
document.form1.EA.value="";
document.form1.EB.value="";
document.form1.EC.value="";

//northing coordinates
document.form1.NA.value="";
document.form1.NB.value="";
document.form1.NC.value="";

//angles
document.form1.ANG_ALPHA.value="";
document.form1.ANG_GAMMA.value="";
document.form1.ANG_BAC.value="";
document.form1.ANG_CBA.value="";
document.form1.ANG_ACB.value="";

//distances
document.form1.AB.value="";
document.form1.AC.value="";
document.form1.BC.value="";
document.form1.AP.value="";
document.form1.BP.value="";
document.form1.CP.value="";

//final coordinates
document.form1.E.value="";
document.form1.N.value="";

//report box
document.form1.report.value="";
}

function append(text) {
   document.form1.report.value+=text;
}

function report() {
   var datetime = new Date();
   document.form1.report.value="";
   append("Mesa Mike's Tienstra Method 3-Point Resection Solver\r\n");
   append("http://mesamike.org/geocache/GC1B0Q9/tienstra\r\n");
   append("Report generated "+datetime.toLocaleString()+"\r\n\r\n");
   append("Given:\r\n");
   append("Point A: "+EA+", "+NA+"\r\n");
   append("Point B: "+EB+", "+NB+"\r\n");
   append("Point C: "+EC+", "+NC+"\r\n");
   append("Angle Alpha: "+ALPHA+" degrees\r\n");
   append("Angle Gamma: "+GAMMA+" degrees\r\n\r\n");

   append("Calculated:\r\n");
   append("Point P: "+E+", "+N+"\r\n");
   append("Distance AB: "+AB+"\r\n");
   append("Distance AC: "+AC+"\r\n");
   append("Distance BC: "+BC+"\r\n");
   append("Distance AP: "+AP+"\r\n");
   append("Distance BP: "+BP+"\r\n");
   append("Distance CP: "+CP+"\r\n");
   append("Angle BAC: "+BAC+" degrees\r\n");
   append("Angle CBA: "+CBA+" degrees\r\n");
   append("Angle ACB: "+ACB+" degrees\r\n");
}

function calculate() {

   // get input data
   EA=document.form1.EA.value;
   EB=document.form1.EB.value;
   EC=document.form1.EC.value;
   NA=document.form1.NA.value;
   NB=document.form1.NB.value;
   NC=document.form1.NC.value;
   ALPHA = document.form1.ANG_ALPHA.value;
   GAMMA = document.form1.ANG_GAMMA.value;
   ANG_ALPHA=ALPHA * Math.PI / 180;
   ANG_GAMMA=GAMMA * Math.PI / 180;

   // calculate ANG_BETA
   ANG_BETA=2*Math.PI - ANG_ALPHA - ANG_GAMMA;

   // calculate distances
   AB=Math.sqrt( Math.pow((EA-EB),2) + Math.pow((NA-NB),2) );
   AC=Math.sqrt( Math.pow((EA-EC),2) + Math.pow((NA-NC),2) );
   BC=Math.sqrt( Math.pow((EB-EC),2) + Math.pow((NB-NC),2) );

   // calculate angles
   ANG_A = Math.acos( (Math.pow(AB, 2) + Math.pow(AC,2) - Math.pow(BC,2))/(2*AB*AC) );
   ANG_B = Math.acos( (Math.pow(AB, 2) + Math.pow(BC,2) - Math.pow(AC,2))/(2*AB*BC) );
   ANG_C = Math.acos( (Math.pow(AC, 2) + Math.pow(BC,2) - Math.pow(AB,2))/(2*AC*BC) );

   // calculate cotangents of angles
   COT_A = 1/Math.tan(ANG_A);
   COT_B = 1/Math.tan(ANG_B);
   COT_C = 1/Math.tan(ANG_C);
   COT_ALPHA = 1/Math.tan(ANG_ALPHA);
   COT_BETA = 1/Math.tan(ANG_BETA);
   COT_GAMMA = 1/Math.tan(ANG_GAMMA);

   // calculate scalers
   KA = 1/(COT_A - COT_ALPHA);
   KB = 1/(COT_B - COT_BETA);
   KC = 1/(COT_C - COT_GAMMA);
   K  = KA + KB + KC;

   // calculate final coordinates
   E = (KA*EA + KB*EB + KC*EC)/K;
   N = (KA*NA + KB*NB + KC*NC)/K;

   // calculate distances from point P
   AP=Math.sqrt( Math.pow((EA-E),2) + Math.pow((NA-N),2) );
   BP=Math.sqrt( Math.pow((EB-E),2) + Math.pow((NB-N),2) );
   CP=Math.sqrt( Math.pow((EC-E),2) + Math.pow((NC-N),2) );

   // calculate angles in degrees
   BAC = ANG_A * 180 / Math.PI
   CBA = ANG_B * 180 / Math.PI
   ACB = ANG_C * 180 / Math.PI

   // display final coordinates
   document.form1.E.value = E;
   document.form1.N.value = N;

   // display other results of interest
   document.form1.AB.value = AB;
   document.form1.BC.value = BC;
   document.form1.AC.value = AC;
   document.form1.AP.value = AP;
   document.form1.BP.value = BP;
   document.form1.CP.value = CP;
   document.form1.ANG_BAC.value = BAC;
   document.form1.ANG_CBA.value = CBA;
   document.form1.ANG_ACB.value = ACB

   report();
}
*/