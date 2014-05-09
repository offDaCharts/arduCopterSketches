#include <VirtualWire.h>//Library needed for transmission and receiving
volatile int rotaryCount = 0;//Used for interupt

#define DIRECTIONC 12//Defining Motor C Direction Pin 12

#define DIRECTIOND 2//Defining Motor D Direction Pin 2

//Used to define variables and pins for interrupt
#define PINA 8 
#define PINB 9
#define INTERRUPT 0  

#define DIRECTIONA 4//Defining Motor A Direction Pin 4
#define MOTORA 5//Defining Motor A Power Pin 5 also power for motor D; connect pin 5 to both power pins 

#define DIRECTIONB 7//Defining Motor B Direction Pin 7
#define MOTORB 6//Defining Motor B Power Pin 6 also power for motor C; connect pin 6 to both power pins

#define TIME_FORWARDS 3000 //Time to move forward 3 seconds
#define TIME_TURN 650 //Time needed to turn 90 degress

//Gloabal variables to be used in program
boolean messageReceived = false;
boolean cyclesStarted = false;
boolean finished = false;
boolean latBoolean = true;
int i = 0;
int j = 0;
int k = 0;
int l = 0;
int cyclesLat;
int cyclesLong;

//part of interrupt 
void isr ()
{
  boolean up;

  if (digitalRead (PINA))
    up = digitalRead (PINB);
  else
    up = !digitalRead (PINB);

  if (up)
    rotaryCount++;
  else
    rotaryCount--;
}  // end of isr

//Setting up rover pins and receiving code
void setup ()
{
  attachInterrupt (INTERRUPT, isr, CHANGE);   // interrupt 0 is pin 2, interrupt 1 is pin 3
  pinMode (MOTORA, OUTPUT);
  pinMode (DIRECTIONA, OUTPUT);
  pinMode (MOTORB, OUTPUT);
  pinMode (DIRECTIONB, OUTPUT);
  pinMode (DIRECTIONC, OUTPUT);
  pinMode (DIRECTIOND, OUTPUT);
  Serial.begin(9600);
 Serial.println("setup");
 vw_setup(2000); // Bits per sec
 vw_rx_start(); // Start the receiver PLL running

}  // end of setup

byte phase;
unsigned long start;
int time_to_go;

//Main Rover Function
//Receiving of Latitude and Longitude cycles
void loop ()
{
  if(!cyclesStarted){
     receivingCode();
  }
 if(messageReceived && !cyclesStarted){
    Serial.print(cyclesLat);
    Serial.println(cyclesLong);
    cyclesStarted = true;
 
 //Motor Powers for the latitude movement of the rover 
 //Hard Coded for cement on Pereria Quad
 }
 if(cyclesStarted && !finished){
   if(latBoolean){
      if(i >= 15){
        analogWrite (MOTORA, 230);
        analogWrite (MOTORB, 200);
        start = millis ();
      }
      else if (i >= 12){
        analogWrite (MOTORA, 225);
        analogWrite (MOTORB, 200);
        start = millis ();
      }
      else if (i >= 8){
        analogWrite (MOTORA, 215);
        analogWrite (MOTORB, 185);
        start = millis ();
      }
      else{
        analogWrite (MOTORA, 195);
        analogWrite (MOTORB, 175);
        start = millis ();
      }
   }
   
   //Motor Powers for the longitute movement of the rover
   //Hard coded for grass of Pereria Quad
   //Not always straight with different levels of battery charge. 
   //Impliment a Control System on Rover
   else{
       if(j >= 9){
         analogWrite (MOTORA, 210);
         analogWrite (MOTORB, 195);
         start = millis ();
       }
       else if (j >= 4){
         analogWrite (MOTORA, 210);
         analogWrite (MOTORB, 195);
         start = millis ();
       }
       else if (j >= 2){
         analogWrite (MOTORA, 200);
         analogWrite (MOTORB, 195);
         start = millis ();
       }
       else{
         analogWrite (MOTORA, 200);
         analogWrite (MOTORB, 190);
         start = millis ();
       }
    }
  
  // check current drain
  while (millis () - start < time_to_go)
    {
    if (analogRead (0) > 500)  // > 1.46 amps
      break;    
    }
    //Turn on all motors one direction to move straight
    if ( i < cyclesLat )   {
      digitalWrite (DIRECTIONA, 1); 
      digitalWrite (DIRECTIONB, 1); 
      digitalWrite (DIRECTIONC, 1); 
      digitalWrite (DIRECTIOND, 1); 
      time_to_go = TIME_FORWARDS;
      i++;
    }
    //Turn on two motors to move forward, two to move backwards in order to turn
    else if ( i == cyclesLat ) {     
      digitalWrite (DIRECTIONA, 0); 
      digitalWrite (DIRECTIONB, 1);
      digitalWrite (DIRECTIONC, 1); 
      digitalWrite (DIRECTIOND, 0);
      latBoolean = false;
      time_to_go = TIME_TURN;
      i++;
     }
     //Forward again
     else  if ( j < cyclesLong ) {
      digitalWrite (DIRECTIONA, 1); 
      digitalWrite (DIRECTIONB, 1);
      digitalWrite (DIRECTIONC, 1); 
      digitalWrite (DIRECTIOND, 1); 
      time_to_go = TIME_FORWARDS;
      j++;
    } 
    //Makeing sure the Rover stops after all cycles complete
     else if(j == cyclesLong){
      j++;
      finished = true;
     }
    else if ( i == cyclesLat + 1, k == cyclesLat + 1, j == cyclesLong + 1, l == cyclesLong + 1) {
      analogWrite (MOTORA, 0);
      analogWrite (MOTORB, 0);
      finished = true;
    }
    else{
      analogWrite (MOTORA, 0);
      analogWrite (MOTORB, 0);
      finished = true;
    }
    analogWrite (MOTORA, 0);
    analogWrite (MOTORB, 0);
    delay (1000);
 }
}  // end of loop
//Code for reveiving coordinates
void receivingCode(){ 
   uint8_t buf[VW_MAX_MESSAGE_LEN];
   uint8_t buflen = VW_MAX_MESSAGE_LEN;
   if (vw_get_message(buf, &buflen) && !messageReceived) // Non-blocking
   {
    int i;
Serial.print("Got: ");
for (i = 0; i < buflen; i++){
 Serial.print(buf[i], HEX);
 Serial.print(" ");
}
Serial.println("");
messageReceived = true;
  cyclesLat = buf[0];
  cyclesLong = buf[1];
 }
}
