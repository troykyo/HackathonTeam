
#include <SoftwareSerial.h>

// Define the pins on Arduino for XBee comminication
uint8_t pinRx = 3 , pinTx = 2; // the pin on Arduino
long BaudRate = 9600 , sysTick = 0;
char GotChar;

// Initialize NewSoftSerial

SoftwareSerial mySerial( pinRx , pinTx );

const int buttonPin = 5;     // the number of the pushbutton pin

int recByte = -1;
int writByte = -1;
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 25;    // the debounce time; increase if the output flickers


void setup()  { 
  
  Serial.begin(9600);
 // Serial1.begin(9600);
  Serial.println("XBee Communication Test Start !");
  Serial.print("BaudRate:");
  Serial.println(BaudRate);
  Serial.print("SoftwareSerial Rx Pin#");
  Serial.println(pinRx,DEC);
  Serial.print("SoftwareSerial Tx Pin#");
  Serial.println(pinTx,DEC);

  // This part is the NewSoftSerial for talking to XBee
  mySerial.begin(BaudRate);
  mySerial.println("Powered by SoftwareSerial !");
    
  pinMode(buttonPin, INPUT); 
  digitalWrite(buttonPin,HIGH);
} 

void loop() { 
   
    
      Serial.write(writByte);
      delay(100);

   
     int reading = digitalRead(buttonPin);
   
     if (reading != lastButtonState) {
      // reset the debouncing timer
        lastDebounceTime = millis();
       } 
 
 // transmitter  
   if ((millis() - lastDebounceTime) > debounceDelay) {
         buttonState = reading;
         if(buttonState == HIGH){
              writByte = 'a';
          }
         else{
              writByte = 'b';
          }
       }
   
   delay(100);
   lastButtonState = reading;
}
