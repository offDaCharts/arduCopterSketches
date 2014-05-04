// Dagu 5 Chassis example.
// Author: Nick Gammon
// Date:   11th December 2011
#include <VirtualWire.h>
volatile int rotaryCount = 0;

#define DIRECTIONC 12

#define DIRECTIOND 2

#define PINA 8
#define PINB 9
#define INTERRUPT 0  // that is, pin 2

#define DIRECTIONA 4
#define MOTORA 5

#define DIRECTIONB 7
#define MOTORB 6

#define TIME_FORWARDS 3000
#define TIME_BACKWARDS 1000
#define TIME_TURN 700
#define TIME_TURN2 1400
#define TIME_TURN3 2100
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

void loop ()
{
  if(!cyclesStarted){
   receivingCode();
  }
 if(messageReceived && !cyclesStarted){
    Serial.print(cyclesLat);
    Serial.println(cyclesLong);
    cyclesStarted = true;
    
 }
 if(cyclesStarted && !finished){
   if(latBoolean){
      if(i > 12){
        analogWrite (MOTORA, 185);//220
        analogWrite (MOTORB, 220);
        start = millis ();
      }
      else{
        analogWrite (MOTORA, 185);//220
        analogWrite (MOTORB, 185);
        start = millis ();
      }
   }
   /*CYCLE 4*/
//   else{   
//      if(j >= 12){
//        analogWrite (MOTORA, 220);
//        analogWrite (MOTORB, 185);
//        start = millis ();
//      }
//      else if (j > 4){
//        analogWrite (MOTORA, 210);
//        analogWrite (MOTORB, 185);
//        start = millis ();
//      }
//      else{
//        analogWrite (MOTORA, 195);
//        analogWrite (MOTORB, 185);
//        start = millis ();
//      }
//    }
    
//    /*CYCLE 5*/
//    else{
//    if(j >= 9){
//      analogWrite (MOTORA, 225);
//      analogWrite (MOTORB, 190);
//      Serial.println("hey");
//      start = millis ();
//    }
//    else if (j > 5){
//      analogWrite (MOTORA, 215);
//      analogWrite (MOTORB, 180);
//      Serial.println("you");
//      start = millis ();
//    }
//    else{
//      analogWrite (MOTORA, 205);
//      analogWrite (MOTORB, 185);
//      start = millis ();
//    }
// }
//    /*CYCLE 6*/
//    else{
//    if(j >= 9){
//      analogWrite (MOTORA, 215);
//      analogWrite (MOTORB, 190);
//      Serial.println("hey");
//      start = millis ();
//    }
//    else if (j > 3){
//      analogWrite (MOTORA, 215);
//      analogWrite (MOTORB, 180);
//      Serial.println("you");
//      start = millis ();
//    }
//    else{
//      analogWrite (MOTORA, 210);
//      analogWrite (MOTORB, 185);
//      start = millis ();
//    }
// }
  /*CYCLE 7*/
    else{
    if(j >= 9){
      analogWrite (MOTORA, 215);
      analogWrite (MOTORB, 190);
      Serial.println("hey");
      start = millis ();
    }
     else if (j >= 4){
      analogWrite (MOTORA, 200);
      analogWrite (MOTORB, 195);
      Serial.println("you");
      start = millis ();
    }
    else if (j >= 2){
      analogWrite (MOTORA, 200);
      analogWrite (MOTORB, 185);
      Serial.println("you");
      start = millis ();
    }
    else{
      analogWrite (MOTORA, 210);
      analogWrite (MOTORB, 185);
      start = millis ();
    }
 }
  
  // check current drain
  while (millis () - start < time_to_go)
    {
    if (analogRead (0) > 500)  // > 1.46 amps
      break;    
    }
    if ( i < cyclesLat )   {
      digitalWrite (DIRECTIONA, 1); 
      digitalWrite (DIRECTIONB, 1); 
      digitalWrite (DIRECTIONC, 1); 
      digitalWrite (DIRECTIOND, 1); 
      time_to_go = TIME_FORWARDS;
      i++;
    }
    else if ( i == cyclesLat ) {     
      digitalWrite (DIRECTIONA, 0); 
      digitalWrite (DIRECTIONB, 1);
      digitalWrite (DIRECTIONC, 1); 
      digitalWrite (DIRECTIOND, 0);
      latBoolean = false;
      time_to_go = TIME_TURN;
      i++;
     }
     else  if ( j < cyclesLong ) {
      digitalWrite (DIRECTIONA, 1); 
      digitalWrite (DIRECTIONB, 1);
      digitalWrite (DIRECTIONC, 1); 
      digitalWrite (DIRECTIOND, 1); 
      time_to_go = TIME_FORWARDS;
      j++;
    } 
     else if(j == cyclesLong){
      digitalWrite (DIRECTIONA, 0); 
      digitalWrite (DIRECTIONB, 1);
      digitalWrite (DIRECTIONC, 1); 
      digitalWrite (DIRECTIOND, 0);
      time_to_go = TIME_TURN2;
      j++;
     }
     else  if ( l < cyclesLong ) {
      digitalWrite (DIRECTIONA, 1); 
      digitalWrite (DIRECTIONB, 1);
      digitalWrite (DIRECTIONC, 1); 
      digitalWrite (DIRECTIOND, 1); 
      time_to_go = TIME_FORWARDS;
      l++;
    } 
    else if ( l == cyclesLat ) {     
      digitalWrite (DIRECTIONA, 0); 
      digitalWrite (DIRECTIONB, 1);
      digitalWrite (DIRECTIONC, 1); 
      digitalWrite (DIRECTIOND, 0);
      time_to_go = TIME_TURN3;
      latBoolean = true;
      l++;
     }
    else if ( k < cyclesLat )   {
      digitalWrite (DIRECTIONA, 1); 
      digitalWrite (DIRECTIONB, 1); 
      digitalWrite (DIRECTIONC, 1); 
      digitalWrite (DIRECTIOND, 1); 
      time_to_go = TIME_FORWARDS;
      k++;
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
