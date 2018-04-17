// LED strip
#include "Adafruit_WS2801.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
 #include <avr/power.h>
#endif

class LedStrip
{
  // Class Member Variables
  // These are initialized at startup
  Adafruit_WS2801 strip;
  uint16_t updateIntervalTime;

  // These maintain the current state
  unsigned long pulsatePreviousMillis = 0;   // will store last time the strip was updated
  unsigned long lastUpdate = 0;   // will store last time the strip was updated

  int pulsateCycles = 0;
  int duration = 5000;
  byte color1[3] = {0,0,0};
  byte color2[3] = {255,0,0};
  int numberOfSteps;

  float R;


  // Constructor - creates a LedStrip 
  // and initializes the member variables and state
  public:
  LedStrip(uint16_t numberOfLeds, uint8_t dataPin, uint8_t clockPin, uint16_t refreshTime)
  {
    // dataPin is the Yellow wire on Adafruit Pixels
    // clockPin is the Green wire on Adafruit Pixels
    strip = Adafruit_WS2801(numberOfLeds, dataPin, clockPin);

    updateIntervalTime = refreshTime;
    numberOfSteps = duration / updateIntervalTime;

      // Calculate the R variable (only needs to be done once at setup)
    R = (numberOfSteps * log10(2))/(log10(255));
  }

  void Begin()
  {
    strip.begin();
    strip.show();
  }

  void Update(unsigned long currentMillis)
  {
    if (currentMillis - lastUpdate >= updateIntervalTime) {
//      Serial.println(lastUpdate);
      lastUpdate = currentMillis;
      strip.show();
    }
  }

  void setUniformColor(uint32_t c)
  {
    int i;
    
    for (i=0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
    }
//    strip.show();
  }

  void pulsate(unsigned long currentMillis, byte * c1, byte * c2, uint16_t duration)
  {
    if ((unsigned long)(millis() - pulsatePreviousMillis) >= updateIntervalTime) {
       pulsatePreviousMillis = millis();
       setPulsate();
    }
  }

  void setPulsate()
  {
//    Serial.println(R);
    pulsateCycles++;

    int rDiff = color2[0]-color1[0];
    int gDiff = color2[1]-color1[1];
    int bDiff = color2[2]-color1[2];

//        Serial.println(rDiff);

    float rVariation = (float)rDiff / (float)numberOfSteps;
    float gVariation = (float)gDiff / (float)numberOfSteps;
    float bVariation = (float)bDiff / (float)numberOfSteps;

//    Serial.println(rVariation);
//    Serial.println(rVariation);
    byte newRed = color1[0] + pulsateCycles * rVariation;
    byte newGreen = color1[1] + pulsateCycles * gVariation;
    byte newBlue = color1[2] + pulsateCycles * bVariation;

//    byte newExpoRed = color1[0] + sign(rDiff) * (pow (2, (pulsateCycles / R)) - 1);
//    Serial.println(newExpoRed);

//    Serial.println(sign(rDiff));
//        Serial.println(newRed);


    setUniformColor(Color(newRed,newGreen,newBlue));
    if(pulsateCycles >= numberOfSteps) {
      pulsateCycles = 0; 
      byte bufferColorR = color1[0];
      byte bufferColorG = color1[1];
      byte bufferColorB = color1[2];

      color1[0] = color2[0];
      color1[1] = color2[1];
      color1[2] = color2[2];

      color2[0] = bufferColorR;
      color2[1] = bufferColorG;
      color2[2] = bufferColorB;
    }
  }

  int sign(int x) {
    return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
  }

  // Create a 24 bit color value from R,G,B
  uint32_t Color(byte r, byte g, byte b)
  {
    uint32_t c;
    c = r;
    c <<= 8;
    c |= g;
    c <<= 8;
    c |= b;
    return c;
  }
};

uint8_t dataPin  = 11;     // Yellow wire on Adafruit Pixels
uint8_t clockPin = 12;     // Green wire on Adafruit Pixels
uint16_t refreshTime = 200; // refreshTime in milliseconds

LedStrip ledstrip(12, dataPin, clockPin, refreshTime);


unsigned long curMillis;



//=============

void setup() {
  Serial.begin(9600);

  // LED strip
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
    clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
  #endif

uint8_t dataPin  = 11;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = 12;    // Green wire on Adafruit Pixels


  ledstrip.Begin();
  
  // tell the PC we are ready
  Serial.println("<Arduino is ready>");
}


byte color1[3] = {20,0,0};
byte color2[3] = {255,0,0};
uint16_t transitionTime = 10000; //2 seconds

//=============

void loop() {
  curMillis = millis();
//  ledstrip.setUniformColor(Color(255,0,0));

//Serial.println(color1[2]);
  ledstrip.pulsate(curMillis, color1, color2, transitionTime);
  ledstrip.Update(curMillis);
}

// LED STRIPS FUNCTIONS



