//////////////////////////////////////////////////////////////////
//©2011 bildr
//Released under the MIT License - Please reuse change and share
//Turning the pot on one Arduino, dims the LED on all the others on the network
//////////////////////////////////////////////////////////////////

//#define potPin 3
#include <SoftwareSerial.h>

#define ledPin 5
//const int buttonPin = 9;     // the number of the pushbutton pin

int inByte = 'b';


// Define the pins on Arduino for XBee comminication
uint8_t pinRx = 6 , pinTx = 7; // the pin on Arduino
long BaudRate = 9600 , sysTick = 0;
char GotChar;
// Initialize NewSoftSerial
SoftwareSerial mySerial( pinRx , pinTx );

void setup()  { 
  pinMode(ledPin, OUTPUT); 
  
  Serial.begin(9600);
  Serial.println("XBee Communication Test Start 2 !");
  Serial.print("BaudRate 2:");
  Serial.println(BaudRate);
  Serial.print("SoftwareSerial Rx Pin# ");
  Serial.println(pinRx,DEC);
  Serial.print("SoftwareSerial Tx Pin#");
  Serial.println(pinTx,DEC);
   
  // This part is the NewSoftSerial for talking to XBee
  mySerial.begin(BaudRate);
  mySerial.println("Powered by SoftwareSerial 2 !");

} 

void loop() { 
  digitalWrite(ledPin,HIGH);
  
  Serial.print("Stored value...");
  inByte = mySerial.read();
  Serial.println(inByte);
  Serial.println("waiting for input...");  
  delay(5000);
  
  if (mySerial.available()){ 

    inByte = mySerial.read();
 
    Serial.println(inByte);
          
         if (inByte == 'a'){
           digitalWrite(ledPin,HIGH);
         }
         else{
           if (inByte == 'b'){
            digitalWrite(ledPin,LOW);
           }
         }
        
   }
  delay(100);
}
