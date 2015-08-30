
#include <NewPing.h>
#include <Adafruit_NeoPixel.h>

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(2, PIN, NEO_GRB + NEO_KHZ800);

#define TRIGGER_PIN  11  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     12  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 256 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

unsigned long startTime = 0;
int color = 0;
int currentColor = 0;
unsigned int uS = 0;

void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
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
  if ((millis() - startTime) > 50) {
    startTime = millis();
    uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
    color = uS / US_ROUNDTRIP_CM;
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

void IR(){
   if (irrecv.decode(&results)) {
   // Serial.println(results.value, HEX);
    String IRsignal = String(results.value,HEX);
    int num = IRsignal.length();
    IRsignal_short = IRsignal.substring(num-3,num);
    Serial.println(IRsignal_short);
    irrecv.resume(); // Receive the next value
  }
 
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
