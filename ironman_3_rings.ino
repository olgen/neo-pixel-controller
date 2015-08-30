// Low power NeoPixel goggles example.  Makes a nice blinky display
// with just a few LEDs on at any time.

#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
#include <avr/power.h>
#endif

#define PIN 0
#define SWITCH 2
#define PIX 47

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIX, PIN);

uint8_t fade = 50;

// layout:
uint8_t innerFrom = 40,
        innerTo = 47,
        middleFrom = 24,
        middleTo = 39,
        outerFrom = 0,
        outerTo = 23;

int program = 0;
int programs = 5;

void setup() {

#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif

  pixels.begin();
  pixels.setBrightness(50); // 1/3 brightness
  //pixels.setBrightness(240); // 1/3 brightness
  randomSeed(analogRead(0));
  //program = random(programs);
  
  // initialize the SWITCH pin as an input.
  pinMode(SWITCH, INPUT);
  // ...with a pullup
  digitalWrite(SWITCH, HIGH);  
}

uint32_t onColor = pixels.Color(250, 120, 60);
uint32_t black = pixels.Color(0, 0, 0);

int frame = 5; //ms


uint32_t loops = 0;
int maxLoops = 100;

void loop() {
  //randomDot();
  //randMatrix();
   return heartBeat();
  // rotatingRings();
  // return clock();
  
  if (!digitalRead(SWITCH)) {
    program = (program + 1) % programs;
  }
  
  
  switch ( program ) {
    case 0:
      randomDot();
      break;
    case 1:
      randomMatrix();
      break;
    case 2:
      heartBeat();
      break;
    case 3:
      rotatingRings();
      break;
    case 4:
      clock();
      break;
  }
}

int rotInner = 0,
    rotMiddle = 0,
    rotOuter = 0,
    lenInner = 2,
    lenMiddle = 8,
    lenOuter = 12;

void clock() {
  rotateOuter();
  
  if(rotOuter == 0) {
    rotateMiddle();
  }
  if(rotMiddle == 0){
    rotateInner();     
  }
  delay(100);
}

void rotatingRings() {
  rotateOuter();
  rotateMiddle();
  rotateInner();
  delay(100);
}

void rotateOuter() {
  int outerTotal = outerTo - outerFrom;
  setColor(outerFrom, outerTo, black);
  setRingColor(outerFrom, outerTotal, rotOuter, lenOuter, onColor);
  rotOuter = (rotOuter + 1) % outerTotal;
}

void rotateMiddle() {
  int middleTotal = middleTo - middleFrom;
  setColor(middleFrom, middleTo, black);
  setRingColor(middleFrom, middleTotal, middleTotal - rotMiddle, lenMiddle, onColor);
  rotMiddle = (rotMiddle + 1) % middleTotal;
}

void rotateInner() {
  int innerTotal = innerTo - (innerFrom + 1);
  setColor(innerFrom, innerTo, black);
  setRingColor((innerFrom + 1), innerTotal, rotInner, lenInner, onColor);
  rotInner = (rotInner + 1) % innerTotal;
}

void setRingColor(int ringStart, int ringTotal, int stripeStart, int stripeLength, uint32_t color) {
  uint8_t  i;
  for (i = stripeStart; i <= (stripeStart + stripeLength); i++) {
    pixels.setPixelColor(ringStart + i % (ringTotal+1), color);
  }
  pixels.show();
}

void randomDot() {
  int pixel1 = random(47);
  int pixel2 = random(47);
  pixels.setPixelColor(pixel1, randomColor());
  pixels.setPixelColor(pixel2, randomColor());
  pixels.show();
  delay(frame);
  pixels.setPixelColor(pixel1, black);
  pixels.setPixelColor(pixel2, black);
  pixels.show();
  delay(frame);
}

void randomMatrix() {
  uint8_t  i;
  for (i = 0; i <= PIX; i++) {
    pixels.setPixelColor(i, randomColor());
  }
  pixels.show();
  delay(frame);
}

uint32_t randomColor() {
  return pixels.Color(random(255), random(255), random(255));
}

int pulse = 1100; //ms

void heartBeat() {
  frame = 35;
  beat(frame);
  delay(2 * frame);
  beat(frame);
  delay(pulse - frame * 16);
}

void beat(int frame) {
  setColor(innerFrom, innerTo, onColor);
  delay(frame);
  setColor(middleFrom, middleTo, onColor);
  delay(frame);
  setColor(outerFrom, outerTo, onColor);
  delay(2 * frame);

  setColor(outerFrom, outerTo, black);
  delay(frame);
  setColor(middleFrom, middleTo, black);
  delay(frame);
  setColor(innerFrom, innerTo, black);
}

void allBlack() {
  setColor(innerFrom, innerTo, black);
  setColor(middleFrom, middleTo, black);
  setColor(outerFrom, outerTo, black);
  pixels.show();
}

void setColor(int from, int to, uint32_t color) {
  uint8_t  i;
  for (i = from; i < to + 1; i++) {
    pixels.setPixelColor(i, color);
  }
  pixels.show();
}
