import processing.serial.*;

Serial port;
String inString = "(x,y)"; //String received from serial at the form: "x,y"

int xSize = 900;
int ySize = 600;

int x = 9;
int y = 9;
String pos = "Pos: ";

void setup() {
    
    println(Serial.list()); 
    if (Serial.list().length > 0)
      port = new Serial(this, Serial.list()[0], 9600);
    size(xSize, ySize);
    //port.bufferUntil();
    frameRate(30); 
}

int i = 0;
int test[][] = {{10, 11, 50, 30, 20, 55, 77, 100},{10, 10, 10, 20, 20, 30, 40, 33}};

void draw() {
   //drawBot(7, 7);
    /*while(port.available() > 0) {
      int b = port.read();        
    }*/

  drawBot(x, y);
 // String s = "Pos: " + x + "," + (ySize-y);
  fill(255);
  textSize(25);
  text(pos, 10, ySize-30);
   drawXaxis();
   drawYaxis();

}

void drawBot(int x, int y) {
   noStroke();
   background(0,0, 230); 
   fill(255, 69, 0);
   rectMode(CENTER);
   rect(x*3, (ySize-(3*y)), 11, 11);
  //The beacon
   fill(0, 0, 0);
   rect(x*3,(ySize-(3*y)), 3, 3); 
}


/*Event method for receiving serial data from beacon, and plotting it realtime*/
void serialEvent(Serial port) {
    inString = port.readStringUntil(10);
    if (inString != null) {
      String[] coordinate = inString.split(","); //Should split on , splits everythin
      //char check = inString.charAt(0);
     // String[] coordinate = splitString(inString); //Should split on , splits everything
      /*for (int i = 0; i < coordinate.length; i++) {
        print(coordinate[i] + " "); 
      }*/
    //  print(inString);
      if (coordinate.length == 2) { 
        x = Integer.parseInt(trim(coordinate[0]));
        y = Integer.parseInt(trim(coordinate[1]));
        pos = "Pos: " + x + "," + y;
      }
  }
}
void drawXaxis() {
  String s = "";
  for (int i = 0; i <= xSize; i+=25) {
    if ((i/3)%25 == 0) {
      s = (i/3) + "";
      textSize(16);
      text(s, i, ySize-2);
      stroke(255);
      line(i, ySize, i, 0);
    }
  } 
}

void drawYaxis() {
  String s = "";
 for(int i = ySize-25; i > 0; i-=25) {
    if ((i/3)%25 == 0) {
        s = ((ySize-i)/3) + "";
        textSize(16);
        text(s, 2, i);
        stroke(255);
        line(0, ySize-i, xSize, ySize-i);
    }  
 }   
}
