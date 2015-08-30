//////////////////////////////////////////////////////////////////
//©2011 bildr
//Released under the MIT License - Please reuse change and share
//Turning the pot on one Arduino, dims the LED on all the others on the network
//////////////////////////////////////////////////////////////////

//#define potPin 3
#include <SoftwareSerial.h>

#define ledPin 12
//const int buttonPin = 9;     // the number of the pushbutton pin

int inByte = 'z';
int LastinByte = 'z';


// Define the pins on Arduino for XBee comminication
uint8_t pinRx = 10, pinTx = 11; // the pin on Arduino
long BaudRate = 9600 , sysTick = 0;
char GotChar;
// Initialize NewSoftSerial
SoftwareSerial mySerial( pinRx , pinTx );

void setup()  { 
  pinMode(ledPin, OUTPUT); 
  digitalWrite(ledPin,LOW);
  
  Serial.begin(9600);
  Serial.println("XBee Communication Test Start 2 !");
  Serial.print("BaudRate 2:");
  Serial.println(BaudRate);
  Serial.print("SoftwareSerial Rx Pin# D");
  Serial.println(pinRx,DEC);
  Serial.print("SoftwareSerial Tx Pin# D");
  Serial.println(pinTx,DEC);
   
  // This part is the NewSoftSerial for talking to XBee
  mySerial.begin(BaudRate);
  mySerial.println("Powered by SoftwareSerial 2 !");

} 

void loop() { 
  //digitalWrite(ledPin,HIGH);
  
  //Serial.print("Stored value...");
  //inByte = mySerial.read();
  //Serial.write(inByte);
  //Serial.println("waiting for input...");  
  delay(100);
  
  if (mySerial.available()){ 

    inByte = mySerial.read();
    
    if (inByte == 97){ // 'a'
      digitalWrite(ledPin,HIGH);
    }
    else{
      if (inByte == 98){ // 'b'
        digitalWrite(ledPin,LOW);
      }
    }
    if(LastinByte != inByte ) Serial.println(" ");
    Serial.write(inByte);
    LastinByte = inByte;

   }
  delay(100);
}
