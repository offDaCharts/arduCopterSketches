#include <VirtualWire.h>

//Initials
long   startLongWhole = -118;
long startLongDecimal = 4145688;
float startLatWhole = 33.0;
long startLatDecimal = 9695502;
long   longCycles = 0;
long   latCycles = 0;
float inchesPerCycle = 56.0;
float scalingDifference = 2.2; //* pow(10,-7);

//Buffer for message to read from file
char latLonMsg[15];
long bytesReadCount = 0;

void setup() { 
  // Turn the Serial Protocol ON
  vw_setup(2000); // Bits per sec
  pinMode(2, OUTPUT);
   Serial.begin(9600);
}
void loop(){
  byte byteRead;
  if (Serial.available()) {
   
   /* read the most recent byte */
   byteRead = Serial.read();
   //byteRead=byteRead-'0';
   
   latLonMsg[bytesReadCount++] = byteRead;
  }
   
 if(bytesReadCount >= 15) {
     //The entire string has been read, convert and send to rover  
     
          Serial.println(latLonMsg);
          
  //     if(targetLocInHexCord[3] == '1') {
  //       digitalWrite(2, HIGH); // set the LED on
  //     }
     
  
     long endLatDecimal = 0;
     long endLongDecimal = 0;
     for(int i = 0; i < 7; i++) {
       endLatDecimal = endLatDecimal * 10 + (latLonMsg[i]-'0');
       endLongDecimal = endLongDecimal * 10 + (latLonMsg[i+8]-'0');
     }
     
     
     Serial.println("lat and lon:");
     Serial.println(endLatDecimal);
     Serial.println(endLongDecimal);
     uint8_t buf[2];
  
  
     float longDiff = abs(endLongDecimal - startLongDecimal);
          Serial.println(longDiff);
  
     float latDiff = abs(endLatDecimal - startLatDecimal);
     Serial.println(latDiff);
     float longInch = longDiff /(scalingDifference);
     Serial.println(longInch);
     float latInch = latDiff /(scalingDifference);
     Serial.println(latInch);
     longCycles = floor(longInch/inchesPerCycle);
     Serial.println(longCycles);
     latCycles = floor(latInch/inchesPerCycle);
     Serial.println(latCycles);
     if (latCycles == 7){
       latCycles++;
     }
     Serial.println(longCycles);
     Serial.println(latCycles);
     
     //uint8_t buf[2];
     //longCycles = 24;
     //latCycles = 14;
     
     
     buf[0] = longCycles;
     buf[1] = latCycles;
     //vw_send((uint8_t *)msg, strlen(msg));
     vw_send(buf , 2);
     delay(400);
     
  //   if(byteRead==0){
  //     //Turn off all LEDS
  //     digitalWrite(2, LOW);
  //   }
  //   if(byteRead>0){
  //     digitalWrite((byteRead+1), HIGH); // set the LED on
  //   }
   
  }
}
