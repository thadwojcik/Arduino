#include <FastLED.h>
 
#define LED_PIN 5 //digital pin 5
#define ANALOG_PIN 1 //for generating random numbers
#define NUM_LEDS 579
#define BRIGHTNESS  255
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
CRGB colorsArray[NUM_LEDS];
#define FRAMES_PER_SECOND 50 //it takes ~20ms to push all the data to all 579 LEDs so the hard limit is ~50fps
 
const int size = NUM_LEDS / 3; //size of the rainbow, given 3 rows
int cycleCount = 0; //counts cycles to reset at the correct time
 
void setup() {
  delay(50); //because life sucks
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  //Serial.begin(9600);
 
  //initial bias is arbitary and then normalized to the rainbow size. More bias will make the respective section of the rainbow larger
  int BRsize = 100;
  int RGsize = 100;
  int GBsize = 80;
  int biasSum = BRsize + RGsize + GBsize;
  BRsize = size * (float(BRsize) / float(biasSum));
  RGsize = size * (float(RGsize) / float(biasSum));
  GBsize = size * (float(GBsize) / float(biasSum));
  biasSum = BRsize + RGsize + GBsize;
  //makes the sum of the bias the same size as the rainbow
  if (biasSum < size) {
    BRsize ++;
    biasSum ++;
    if (biasSum < size) {
      RGsize ++;
      biasSum ++;
      if (biasSum < size) {
        GBsize ++;
        biasSum ++;
      }
    }
  }
  else if (biasSum > size) {
    BRsize --;
    biasSum --;
    if (biasSum > size) {
      RGsize --;
      biasSum --;
      if (biasSum > size) {
        GBsize --;
        biasSum --;
      }
    }
  }
 
  for (int i = 0; i < BRsize; i++) {
                           colorsArray[i] = CRGB( int(round(255 * float(i) / float(BRsize))) ,
                           0 ,
                           int(round(255 - 255 * float(i) / (BRsize) )));
                           
                           
   // Serial.println(int(round(255 - float(255) * pow(float(i), float(2)) / (float(BRsize), float(2)) )));
   // Serial.println
   // Serial.println(int(round(255 - 255 * float(i) / float(BRsize))));
  }
  for (int i = 0; i < RGsize; i++) {
    colorsArray[i + BRsize] = CRGB(int(round(255 - 255 * float(i) / float(RGsize))) , int(round(255 * (float(i) / float(RGsize)))) , 0 );
 
  }
  for (int i = 0; i < GBsize; i++) {
    colorsArray[i + BRsize + RGsize] = CRGB(0, int(round(255 - 255 * float(i) / float(GBsize))) , int(round(255 * (float(i) / float(GBsize)))));
 
  }
 
  //colorsArray[0] = CRGB(255,255,255);
 
  //copy first row onto next two rows
  for (int i = 0; i < NUM_LEDS / 3; i++) {
    colorsArray[i + NUM_LEDS / 3] = colorsArray[i];
    colorsArray[i + 2 * NUM_LEDS / 3] = colorsArray[i];
  }
}
 
void loop() {
  unsigned long start = millis(); //start timer
  FastLED.show();
  int speed = 40; //speed of the rainbow
  speed = speed * size / 300; //makes the speed not dependent on the size of the rainbow
 
  int ledOffset = cycleCount * speed / FRAMES_PER_SECOND ;  //calculates how far the generated table should be offset from the starting position for the given cycle count
 
  //writes the led array based on the pre-generated table
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = colorsArray[(i + ledOffset) % NUM_LEDS];
  }
 
  cycleCount ++;
  //resets cycle count after one full cycle
  if (cycleCount == int( size * FRAMES_PER_SECOND / speed)) {
    cycleCount = 0;
  }
  unsigned long delta = millis() - start; //end of timer
 
  if ( 1000 / FRAMES_PER_SECOND > delta) { //adds pause to ensure that fps is at or below the defined fps
    FastLED.delay(1000 / FRAMES_PER_SECOND - delta);
  }
}
