import processing.serial.*;

Serial port;
String inString = "(x,y)"; //String received from serial at the form: "x,y"

int xSize = 900;
int ySize = 600;

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
   if (i < 8) { 
      drawBot(test[0][i], test[1][i]);
      delay(500);
      textSize(32);
      String s = "Pos: (" + test[0][i] + "," + test[1][i] + ")";
      fill(255);
      textSize(25);
      text(s, 10, ySize-30); 
      i++;
   }
   drawXaxis();
   drawYaxis();
}

void drawBot(int x, int y) {
   noStroke();
   background(0,0, 230); 
   fill(255, 69, 0);
   rectMode(CENTER);
   rect(x,y, 11, 11);
  //The beacon
   fill(0, 0, 0);
   rect(x,y, 3, 3); 
}

/*Event method for receiving serial data from beacon, and plotting it realtime*/
void serialEvent(Serial port) {
  inString = port.readString();
  String[] coordinate = inString.split(",");
  if (coordinate.length == 2)
    drawBot(Integer.parseInt(coordinate[0]), Integer.parseInt(coordinate[1]));
    textSize(28);
    String s = "Pos: " + inString;
    text(s, 10, 30); 
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
