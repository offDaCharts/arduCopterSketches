#include <VirtualWire.h>
boolean messageReceived = false;
boolean cyclesStarted = false;
int cyclesX;
int cyclesY;
void setup()
{
 Serial.begin(9600);
 Serial.println("setup");
 vw_setup(2000); // Bits per sec
 vw_rx_start(); // Start the receiver PLL running
}
void loop()
{
  if(!cyclesStarted){
   receivingCode();
  }
 if(messageReceived && !cyclesStarted){
    Serial.print(cyclesX);
    Serial.println(cyclesY);
    cyclesStarted = true;
    
 }
 
 
}

void receivingCode(){ 
   uint8_t buf[VW_MAX_MESSAGE_LEN];
   uint8_t buflen = VW_MAX_MESSAGE_LEN;
   if (vw_get_message(buf, &buflen) && !messageReceived) // Non-blocking
   {
    int i;
// Message with a good checksum received, dump HEX
Serial.print("Got: ");
for (i = 0; i < buflen; i++){
 Serial.print(buf[i], HEX);
 Serial.print(" ");
}
Serial.println("");
messageReceived = true;
cyclesX = buf[0];
 cyclesY = buf[1];
 }
}
