//pin2 = left motor big wheel
//pin3 = right motor big wheel
//pin4 = IR sensor
//pin5 = head servo motor
//pin6 = Neopixels
//pin7 = trigger ultrasonic sensor
//pin8 = echo ultrasonic sensor


#include <Adafruit_NeoPixel.h>
#include <IRremote.h>
#include <Servo.h>

// XBee comminication
#include <SoftwareSerial.h> 
const uint8_t pinRx = 10, pinTx = 11; // XBee comminication SoftSerial pin on Arduino
const long BaudRate = 9600;           // XBee comminication SoftSerial Baud Rate
SoftwareSerial mySerial( pinRx , pinTx );// Initialize the SoftSerial
byte xbByte = 0;  //from 0 to 255. 

#define PIN 6

int RECV_PIN = 4;
String IRsignal_short;

IRrecv irrecv(RECV_PIN);

decode_results results;



Servo myservo_right;  // create servo object to control a servo
//int pos_right = 0;    // variable to store the servo position

Servo myservo_left;  // create servo object to control a servo
//int pos_left = 0;    // variable to store the servo 2 position

Servo myservo_head;  // create servo object to control a servo
//int pos_head= 0;    // variable to store the servo 2 position
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(2, PIN, NEO_GRB + NEO_KHZ800);

#define TRIGGER_PIN  7  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     8  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 256 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
//NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

unsigned long startTime = 0;
int color = 0;
int currentColor = 0;
unsigned int uS = 0;

void setup() {
  // XBee comminication setup
  mySerial.begin(BaudRate);// This part is the NewSoftSerial for talking to XBee
  mySerial.println("Powered by SoftwareSerial 2 !");
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  myservo_left.attach(3);  // attaches the servo on pin 9 to the servo object
  myservo_right.attach(2);  // attaches the servo on pin 9 to the servo object
  myservo_head.attach(5);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);

  irrecv.enableIRIn(); // Start the receiver
  myservo_head.write(120); // current zero position of the head servo (needs to be adjusted in final build)
  myservo_left.write(95);
  myservo_right.write(95);

  //myservo_left.writeMicroseconds(1487); // 1.5 ms stay-still signa
  //myservo_right.writeMicroseconds(1460); // 1.5 ms stay-still signa
}

void loop() {
  // XBee comminication receive data
  if (mySerial.available()) xbByte = mySerial.read();
   else                     xbByte = 0;
  if(xbByte >0) processXBee_data(xbByte);
  //IR();
  
  if ((millis() - startTime) > 1000) {
    startTime = millis();
    color = random(1, 255);
    //uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
    //color = uS / US_ROUNDTRIP_CM;
  }
  /*Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_CM); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
  Serial.println("cm");*/
  if (color > currentColor) {
    currentColor++;
  }
  if (color < currentColor) {
    currentColor--;
  }
  strip.setPixelColor(0, Wheel(currentColor));
  strip.setPixelColor(1, Wheel(255 - currentColor));
  strip.show();
}
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void IR() {
  if (irrecv.decode(&results)) {
    // Serial.println(results.value, HEX);
    String IRsignal = String(results.value, HEX);
    int num = IRsignal.length();
    IRsignal_short = IRsignal.substring(num - 3, num);
    Serial.println(IRsignal_short);
    irrecv.resume(); // Receive the next value
  }
delay (100);
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

}

void processXBee_data(byte command) {

    //note
    // XBee network we have set is Broadcasting network.
    // it means each robot can send message each other,
    // it also means PC can send message to every robot at the same time.

    Serial.print("xb:");
    Serial.println(command);
    
    if (130 == command) { //forward "X", 0x58
      myservo_left.write(180);              // tell servo to go to position in variable 'pos'
      myservo_right.write(00);              // tell servo to go to position in variable 'pos'
    }
  
    if (131 == command) { //back  "Y", 0x59
      myservo_left.write(00);              // tell servo to go to position in variable 'pos'
      myservo_right.write(180);              // tell servo to go to position in variable 'pos'
    }
  
    if (132 == command) { //left  "Z", 0x5A
      myservo_left.writeMicroseconds(1487); // 1.5 ms stay-still signa
      myservo_right.write(00);              // tell servo to go to position in variable 'pos'
    }
  
    if (133 == command) { //right  "[", 0x5B
      myservo_left.write(180);              // tell servo to go to position in variable 'pos'
      myservo_right.writeMicroseconds(1460);
    } // 1.5 ms stay-st
  
    if (134 == command) { //stop  "\", 0x5B
      myservo_left.writeMicroseconds(1487); // 1.5 ms stay-still signa
      myservo_right.writeMicroseconds(1460); // 1.5 ms stay-still signal  
    }
  
    if (1 == command) { //head left  0x01
      myservo_head.write(180); // 1.5 ms stay-still signa
    }    
    if (2 == command) { //head stop    0x02
      myservo_head.write(120); // 1.5 ms stay-still signa
    }
    if (3 == command) { //head right  0x03
      myservo_head.write(60); // 1.5 ms stay-still signa
    }  

}
