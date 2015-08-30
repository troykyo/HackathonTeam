#include <TimerFreeTone.h>

/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

//pin2 = left motor big wheel
//pin3 = right motor big wheel
//pin4 = IR sensor
//pin5 = head servo motor
//pin6 = speaker
//pin7 = trigger ultrasonic sensor
//pin8 = echo ultrasonic sensor

#include <IRremote.h>
#include <Servo.h> 
#include "pitches.h"
 
Servo myservo_right;  // create servo object to control a servo 
//int pos_right = 0;    // variable to store the servo position 

Servo myservo_left;  // create servo object to control a servo 
//int pos_left = 0;    // variable to store the servo 2 position 

Servo myservo_head;  // create servo object to control a servo 
//int pos_head= 0;    // variable to store the servo 2 position 

int melody[] = {
  NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4};

int noteDurations[] = {200, 200, 200, 200, 200, 200, 500, 200, 200, 200, 200, 200, 200, 500};

//Mario main theme melody
int melody2[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,
 
  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,
 
  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,
 
  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,
 
  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};

int tempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};

int RECV_PIN = 4;
String IRsignal_short;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup() {
  myservo_left.attach(3);  // attaches the servo on pin 9 to the servo object  
  myservo_right.attach(2);  // attaches the servo on pin 9 to the servo object   
  myservo_head.attach(5);  // attaches the servo on pin 9 to the servo object   
  Serial.begin(9600);
  
  irrecv.enableIRIn(); // Start the receiver
  myservo_head.write(120); // current zero position of the head servo (needs to be adjusted in final build)
//  myservo_left.write(90);
//  myservo_right.write(90);

myservo_left.writeMicroseconds(1487); // 1.5 ms stay-still signa
myservo_right.writeMicroseconds(1460); // 1.5 ms stay-still signa

}

void loop() {
  
  if (irrecv.decode(&results)) {
   // Serial.println(results.value, HEX);
    String IRsignal = String(results.value,HEX);
    int num = IRsignal.length();
    IRsignal_short = IRsignal.substring(num-3,num);
    Serial.println(IRsignal_short);
    irrecv.resume(); // Receive the next value
  }
  delay(100);

  if (IRsignal_short == "058") {   //forward
        myservo_left.write(180);              // tell servo to go to position in variable 'pos' 
        myservo_right.write(00);              // tell servo to go to position in variable 'pos'   
    } 

 if (IRsignal_short == "059") { //back
    myservo_left.write(00);              // tell servo to go to position in variable 'pos' 
    myservo_right.write(180);              // tell servo to go to position in variable 'pos'
  }

  if (IRsignal_short == "05a") { //left
     myservo_left.writeMicroseconds(1487); // 1.5 ms stay-still signa
     myservo_right.write(00);              // tell servo to go to position in variable 'pos'
  }

  if (IRsignal_short == "05b") { //right
      myservo_left.write(180);              // tell servo to go to position in variable 'pos' 
      myservo_right.writeMicroseconds(1460);
  } // 1.5 ms stay-st

   if (IRsignal_short == "05c") { //stop
     myservo_left.writeMicroseconds(1487); // 1.5 ms stay-still signa
     myservo_right.writeMicroseconds(1460); // 1.5 ms stay-still signal
   
  }

   if (IRsignal_short == "001") { //head left
     myservo_head.write(180); // 1.5 ms stay-still signa
   }


   if (IRsignal_short == "003") { //head right
     myservo_head.write(60); // 1.5 ms stay-still signa
   }


   if (IRsignal_short == "002") { //head stop
     myservo_head.write(120); // 1.5 ms stay-still signa
   }


   if (IRsignal_short == "005") { //play sound

                     for (int thisNote = 0; thisNote < 14; thisNote++) { // Loop through the notes in the array.
                      TimerFreeTone(6, melody[thisNote], noteDurations[thisNote]); // Play melody[thisNote] for duration[thisNote].
                      delay(50); // Short delay between notes.
                    }
   }

     if (IRsignal_short == "008") { //stop sound

                 for (int thisNote = 0; thisNote < 0; thisNote++) { // Loop through the notes in the array.
                      TimerFreeTone(6, melody[thisNote], noteDurations[thisNote]); // Play melody[thisNote] for duration[thisNote].
                      delay(50); // Short delay between notes.
                    }

    }


    if (IRsignal_short == "006") { //play sound

            int size = sizeof(melody2) / sizeof(int);
                for (int thisNote = 0; thisNote < size; thisNote++) {
             
                  // to calculate the note duration, take one second
                  // divided by the note type.
                  //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
                  int noteDuration = 1000 / tempo[thisNote];
             
                  buzz(6, melody2[thisNote], noteDuration);
             
                  // to distinguish the notes, set a minimum time between them.
                  // the note's duration + 30% seems to work well:
                  int pauseBetweenNotes = noteDuration * 1.30;
                  delay(pauseBetweenNotes);
             
                  // stop the tone playing:
                  buzz(6, 0, noteDuration);
             
                }
           
        }

   if (IRsignal_short == "009") { //stop sound
            buzz(6, 0, 0);
    }


 }


 




void buzz(int targetPin, long frequency, long length) {
  digitalWrite(13, HIGH);
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
  digitalWrite(13, LOW);
 
}
