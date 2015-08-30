#include <TimerFreeTone.h>

/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>
#include <Servo.h> 
#include "pitches.h"
#include <Adafruit_NeoPixel.h>
#define PIN 6
 
Servo myservo_right;  // create servo object to control a servo 
Servo myservo_left;  // create servo object to control a servo 
Servo myservo_head;  // create servo object to control a servo 

int RECV_PIN = 10;
String IRsignal_short;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
IRrecv irrecv(RECV_PIN);
decode_results results;


void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
    
  myservo_left.attach(9);  // attaches the servo on pin 9 to the servo object  
  myservo_right.attach(8);  // attaches the servo on pin 9 to the servo object   
  myservo_head.attach(11);  // attaches the servo on pin 9 to the servo object   
  
  myservo_head.write(70); // current zero position of the head servo (needs to be adjusted in final build)
  myservo_left.write(95);
  myservo_right.write(95);

  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {

//  colorWipe(strip.Color(255, 0, 0), 50); // Red
//  colorWipe(strip.Color(0, 255, 0), 50); // Green
//  colorWipe(strip.Color(0, 0, 255), 50); // Blue
//  rainbow(20);
//  rainbowCycle(20);


 //   strip.setPixelColor(1, 0, 255, 0);
 //   strip.show();
  
  
  if (irrecv.decode(&results)) {
    String IRsignal = String(results.value,HEX);
    int num = IRsignal.length();
    IRsignal_short = IRsignal.substring(num-2,num);
    Serial.println(IRsignal_short);
    Serial.println(IRsignal);

    irrecv.resume(); // Receive the next value

    
 
  }

  if (IRsignal_short == "58") {   //forward
        myservo_left.write(60);              // tell servo to go to position in variable 'pos' 
        myservo_right.write(120);              // tell servo to go to position in variable 'pos'   
     //   irrecv.resume(); // Receive the next value
      
    } 

     if (IRsignal_short == "59") { //back
         myservo_left.write(120);              // tell servo to go to position in variable 'pos' 
         myservo_right.write(60);              // tell servo to go to position in variable 'pos'
     //    irrecv.resume(); // Receive the next value
         
    }

   // if (IRsignal_short == "5A") { //left
   //     myservo_left.write(95); // 1.5 ms stay-still signa
   //     myservo_right.write(120);              // tell servo to go to position in variable 'pos'
   //     irrecv.resume(); // Receive the next value
 // }

 // if (IRsignal_short == "5B") { //right
   //   myservo_left.write(60);              // tell servo to go to position in variable 'pos' 
    //  myservo_right.write(95);
   //   irrecv.resume(); // Receive the next value
 // } // 1.5 ms stay-st


   // if (IRsignal_short == "5C") { //stop
   //   myservo_left.write(95);              // tell servo to go to position in variable 'pos' 
   //   myservo_right.write(95);
  //} // 1.5 ms stay-st

   //   if (IRsignal_short == "01") { //head left
   //       myservo_head.write(160);              // tell servo to go to position in variable 'pos' 
   // } // 1.5 ms stay-st


    //  if (IRsignal_short== "03") { //head right
    //      myservo_head.write(0);              // tell servo to go to position in variable 'pos' 
   //  } // 1.5 ms stay-st


    //  if (IRsignal_short == "02") { //head middle
    //      myservo_head.write(70);              // tell servo to go to position in variable 'pos' 
    // } // 1.
  
 // delay(10);

 
 
 //irrecv.resume(); // Receive the next value

 }


 


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

