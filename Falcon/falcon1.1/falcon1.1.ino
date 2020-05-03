#include <FastLED.h>

#define LED_PIN     11
#define NUM_LEDS    60
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 2

CRGBPalette16 currentPalette;
TBlendType    currentBlending;
uint8_t       brightness;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

void setup() {
    delay( 3000 ); // power-up safety delay

    // if analog input pin 0 is unconnected, random analog
    // noise will cause the call to randomSeed() to generate
    // different seed numbers each time the sketch runs.
    // randomSeed() will then shuffle the random function.
    randomSeed(analogRead(0));
    brightness = 0;
  
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
    Serial.begin(9600);
}

void loop()
{    
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */

    static uint8_t brightIndex = 5;

    uint8_t secondHand = (millis() / 1000) % 60;
    Serial.println(secondHand);

    /*if(secondHand <= 30)
      CascadeColors( startIndex);
    else if(secondHand <= 60)
      ConstantColorShift(startIndex);
  */

    
    brightness += brightIndex;
    
    if(brightness % 255 == 0)
      brightIndex*= -1;
    
    Serial.print(brightness);
    Serial.println(brightIndex);
    
    Sparkle(brightness);
    FastLED.show();
    FastLED.delay(1000 / 10);
}

void Sparkle( uint8_t brightnessIndex)
{
    //Serial.println("Sparkle");
    //uint8_t brightness = 255;
    uint8_t brightness = 100;
    
    long randNumber = random(0, NUM_LEDS);

    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
    }
    
    leds[0]=ColorFromPalette( currentPalette, 170, brightnessIndex, LINEARBLEND);         
    //leds[1]=ColorFromPalette( currentPalette, 190, 255, LINEARBLEND);         
    //leds[2]=ColorFromPalette( currentPalette, 190, 10, LINEARBLEND);     
}

void CascadeColors( uint8_t colorIndex)
{
    Serial.println("CascadeColors");
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void ConstantColorShift( uint8_t colorIndex)
{
    Serial.println("ConstantColorShift");
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);        
    }
}


// Additional notes on FastLED compact palettes:
//
// Normally, in computer graphics, the palette (or "color lookup table")
// has 256 entries, each containing a specific 24-bit RGB color.  You can then
// index into the color palette using a simple 8-bit (one byte) value.
// A 256-entry color palette takes up 768 bytes of RAM, which on Arduino
// is quite possibly "too many" bytes.
//
// FastLED does offer traditional 256-element palettes, for setups that
// can afford the 768-byte cost in RAM.
//
// However, FastLED also offers a compact alternative.  FastLED offers
// palettes that store 16 distinct entries, but can be accessed AS IF
// they actually have 256 entries; this is accomplished by interpolating
// between the 16 explicit entries to create fifteen intermediate palette
// entries between each pair.
//
// So for example, if you set the first two explicit entries of a compact 
// palette to Green (0,255,0) and Blue (0,0,255), and then retrieved 
// the first sixteen entries from the virtual palette (of 256), you'd get
// Green, followed by a smooth gradient from green-to-blue, and then Blue.