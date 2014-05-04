#include <VirtualWire.h>

int   startLongWhole = 1234;
float startLongDecimal = 0.567875;
float startLatWhole = 4567.0;
float startLatDecimal = 0.8946233;
int   longCycles = 0;
int   latCycles = 0;
float inchesPerCycle = 52.0;
void setup()
{
 vw_setup(2000); // Bits per sec
}


void loop()
{
 const char *msg = "X 1234.5678910 Y 4567.8946233";
   int i = 0;
   int k;
   unsigned int endLongWhole = 0;
   float endLongDecimal = 0.0;
   unsigned int endLatWhole = 0;
   float endLatDecimal = 0.0;
   
   unsigned int distanceToTravelLongWhole = 0;
   float distanceToTravelLongDecimal = 0.0;
   unsigned int distanceToTravelLatWhole = 0;
   float distanceToTravelLatDecimal = 0.0;
   uint8_t buf[2];
   int decimalPoint = 0;
   for(k = 0 ; k < 2; k++){
     int runningSumWhole = 0;
     float runningSumDecimal = 0;
     while(msg[i] != ' '){
       i++;
     }
     i++;
     boolean endOfFile = false;
     int j = i;
     while(msg[j] != 46 && !endOfFile){
         if(msg[j] == ' ' || msg[j+1] == 0){
           endOfFile = true;
           decimalPoint = j - i;
         }
         if(msg[j+1] == 46){
           decimalPoint = j - i;
         }
         j++;
     }
     endOfFile = false;
     while(msg[i] != ' ' && msg[i] != 0){
         
         if(decimalPoint >= 0){
           runningSumWhole += (msg[i] - '0') * pow(10,decimalPoint);
         }
         else{
           runningSumDecimal += (float)(msg[i] - '0') * (float)pow(10,decimalPoint);
         }
         decimalPoint--;
          
         if(msg[i+1] == '.'){
           i++; 
         }
         i++;
     }
     runningSumWhole += 1;
     if(k == 0){
       endLongWhole = runningSumWhole;
       endLongDecimal = runningSumDecimal;
     }
     else{
       endLatWhole = runningSumWhole;
       endLatDecimal = runningSumDecimal;
     }
     i++;
   }
 distanceToTravelLongWhole = endLongWhole - startLongWhole;
 distanceToTravelLongDecimal = endLongDecimal - startLongDecimal;
 distanceToTravelLatWhole = endLatWhole - startLatWhole;
 distanceToTravelLatDecimal = endLatDecimal - startLatDecimal;
 distanceToTravelLongWhole = distanceToTravelLongWhole * 111.0 * 39370.1; //conversion to inches
 longCycles = distanceToTravelLongWhole/inchesPerCycle;
 distanceToTravelLongDecimal = distanceToTravelLongDecimal * 111.0 * 39370.1; //conversion to inches 
 longCycles += distanceToTravelLongDecimal/inchesPerCycle; //conversion from inches to cycles
 Serial.println(longCycles);
 distanceToTravelLatWhole = distanceToTravelLatWhole * 111.0 * 39370.1; //conversion to inches
 latCycles = distanceToTravelLatWhole/inchesPerCycle;
 distanceToTravelLatDecimal = distanceToTravelLatDecimal * 111.0 * 39370.1; //conversion to inches 
 latCycles += distanceToTravelLatDecimal/inchesPerCycle; //conversion from inches to cycles
 Serial.println(latCycles);
 buf[0] = longCycles;
 buf[1] = latCycles;
 //vw_send((uint8_t *)msg, strlen(msg));
 vw_send(buf , 2);
 delay(400);
}
/*NOtes need to figure how to work with rounding Arduino does
7 digits of precision with mission planner. Maybe do ASCII math?
Should also conisder whether it should be for just this case (eg 
only care about last couple of digits since the testing area should not be bigger than 11 km
Can i assume we will be at bottom left most corner?*/
