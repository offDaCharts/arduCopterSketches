//Libraries are imported and global variables initialized
import processing.serial.*;
import java.io.*;
int mySwitch=0;
int counter=0;
String [] subtext;
Serial myPort;


void setup(){
   //Create a switch that will control the frequency of text file reads.
   //When mySwitch=1, the program is setup to read the text file.
   //This is turned off when mySwitch = 0
   mySwitch=1;
 
   //The com port needs to be set here. Be sure to check the com port of the Arduino
   myPort = new Serial(this, "COM5", 9600);
   myPort.bufferUntil('\n');
}

void draw() {
   if (mySwitch>0){
      /*The readData function can be found later in the code.
        The address is where the file is located. */
      readData("C:\\Users\\Student\\Desktop\\arduCopterSketches\\eastbay-mavlink\\demos\\target_found");
 
       /*The following switch prevents continuous reading of the text file, until
       we are ready to read the file again. */
      mySwitch=0;
    }
   /*Only send new data. This IF statement will allow new data to be sent to
   the arduino. */
 
   if(subtext!=null && counter<subtext.length){
      /* Write the next number to the Serial port and send it to the Arduino */
      myPort.write(subtext[counter]);
      delay(500);
      myPort.write('0');
      delay(100);
      //Increment the counter so that the next number is sent to the arduino.
      counter++;
    } else{
      //If the text file has run out of numbers, then read the text file again in 5 seconds.
      delay(5000);
      mySwitch=1;
    }
} 


/* The following function will read from a TXT file */
void readData(String myFileName){
 
   File file=new File(myFileName);
   BufferedReader br=null;
   /*Try used for error testing to catch potential errors and output a message.*/
   try{
      br=new BufferedReader(new FileReader(file));
      String text=null;
 
      /* keep reading each line until you get to the end of the file */
      while((text=br.readLine())!=null){
          /* Spilt each line up into bits and pieces*/
          subtext = splitTokens(text,"");
        }
    }catch(FileNotFoundException e){
        e.printStackTrace();
    }catch(IOException e){
        e.printStackTrace();
    }finally{
        try {
           if (br != null){
           br.close();
           }
        } catch (IOException e) {
           e.printStackTrace();
        }
    }
}
