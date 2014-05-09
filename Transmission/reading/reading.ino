//Library needed for Transmission
#include <VirtualWire.h>

//Initials
long   startLongWhole = -118;
long startLongDecimal = 4145790;
float startLatWhole = 33.0;
long startLatDecimal = 9695650;
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
     latLonMsg[bytesReadCount++] = byteRead;
  }
   
 if(bytesReadCount >= 15) {
     //The entire string has been read, convert and send to rover  
        Serial.println(latLonMsg);//Serial Print used for debugging
        long endLatDecimal = 0;
        long endLongDecimal = 0;
		//This converts the ASCII version of the numbers to a numerical value
        for(int i = 0; i < 7; i++) {
           endLatDecimal = endLatDecimal * 10 + (latLonMsg[i]-'0');
           endLongDecimal = endLongDecimal * 10 + (latLonMsg[i+8]-'0');
        }
     
     
     Serial.println("lat and lon:");//Serial Print used for debugging
     Serial.println(endLatDecimal);
     Serial.println(endLongDecimal);
     uint8_t buf[2];//Array that will be transmitted
  
     //Calculations to find the difference between start and finish
     float longDiff = abs(endLongDecimal - startLongDecimal);
     Serial.println(longDiff);//Serial Print used for debugging
     float latDiff = abs(endLatDecimal - startLatDecimal);
     Serial.println(latDiff);//Serial Print used for debugging
     float longInch = longDiff /(scalingDifference);
     Serial.println(longInch);//Serial Print used for debugging
     float latInch = latDiff /(scalingDifference);
     Serial.println(latInch);//Serial Print used for debugging
        
     longCycles = round(longInch/inchesPerCycle);
     Serial.println(longCycles);
     latCycles = round(latInch/inchesPerCycle);
     Serial.println(latCycles);
	 //Special case where rover would run into lamp post
     if (latCycles == 7){
       latCycles++;
     }
     Serial.println(longCycles);
     Serial.println(latCycles);
     //Setting transmission array to hold values of cycles
     buf[0] = latCycles;
     buf[1] = longCycles;
     
     //Transmitts message array
     vw_send(buf , 2);
     delay(400);
  }
}
