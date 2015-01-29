/*
TCCR2B |= (1 << CS22);  // Set CS#2 bit for 64 prescaler for timer 2
TCCR1B |= (1 << CS11);  // Set CS#1 bit for 8 prescaler for timer 1
TCCR0B |= (1 << CS02) | (1 << CS00);  // Set CS#2 and CS#0 bits for 1024 
*/

#include <IRremote.h>

#define RECV_PIN 12
#define INTERRUPT_FREQUENCY 1000 //The frequency of the interrupts in Hz

IRrecv irrecv(RECV_PIN);

decode_results results;

//storage variables
boolean toggle0 = 0;
boolean toggle1 = 0;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  //irrecv.blink13(true);
 
   //set pins as outputs
  pinMode(10, OUTPUT);

  cli(); //stops interrupts
  
  //set timer0 interrupt at 2kHz
  TCCR0A = 0;// set entire TCCR2A register to 0
  TCCR0B = 0;// same for TCCR2B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 2khz increments
  OCR0A = 124;// = (16*10^6) / (2000*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (1 << CS01) | (1 << CS00);   
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);


    TCCR1A = 0; //Set entire TCCR2A register to 0
    TCCR1B = 0; // same for TCCR2B
    TCNT1 = 0; //Initialize counter value to 0
    //Set compare match register for 16 KHz increments
    OCR1A = 1; // (16*10^6)/(1000*8) -1 (must be < 65000)
    //Turn on CTC mode 
    TCCR1B |= (1 << WGM12);
    //Set CS12 for 8 bit prescaler
    TCCR1B |= (1 << CS12) | (1 << CS10);
    //enable timer compare interrupt
    TIMSK1 |= (1 << OCIE2A); 

  sei(); //allows interrupts
}

void loop() {
  decode();
}

void decode() {
  if (irrecv.decode(&results)) {
    //Serial.println(results.bits); //The length of the signa
    //Serial.println(results.value);
    int value = results.value;
   if (value >= 0 && value <= 3)
    Serial.println(value);
    //Serial.println(results.value);
   // Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
}

ISR(TIMER0_COMPA_vect){//timer0 interrupt 2kHz toggles pin 8
 //decode();
//generates pulse wave of frequency 2kHz/2 = 1kHz (takes two cycles for full wave- toggle high then toggle low)
 /*if (toggle0){
  //Serial.println(HIGH);
    digitalWrite(10,HIGH);
    toggle0 = 0;
  }
  else{
  //	Serial.println(LOW);
    digitalWrite(10,LOW);
    toggle0 = 1;
  }*/
}

ISR(TIMER1_COMPA_vect){//timer1 interrupt 2Hz toggles pin 13 (LED)
  // decode();

//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
  /*if (toggle1){
  	Serial.println(HIGH);
    digitalWrite(13,HIGH);
    toggle1 = 0;
  }
  else{
  	Serial.println(LOW);
    digitalWrite(13,LOW);
    toggle1 = 1;
  }*/
}
