// LED strip
#include "Adafruit_WS2801.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
 #include <avr/power.h>
#endif

// SERIAL VARIABLES
const byte buffSize = 40;
char inputBuffer[buffSize];
const char startMarker = '<';
const char endMarker = '>';
byte bytesRecvd = 0;
boolean readInProgress = false;
boolean newDataFromRpi = false;
//



class LedStrip
{
  // Variables initialized in constructor
  Adafruit_WS2801 strip;
  uint16_t updateIntervalTime;

  // State variables that are updated after reading Raspberry Pi message
  int activatedMode;
  int duration;
  byte color1[3];
  byte color2[3];

  // Variables useful in handling each behaviour
  unsigned long previousMillis = 0;   // store last time the strip was updated
  int numberOfSteps;
  int modeStep;

  boolean needsUpdate = false; // set to true when the colors change

  // Others
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

      // Calculate the R variable (only needs to be done once at setup)
//    R = (numberOfSteps * log10(2))/(log10(255));
  }

  void Begin()
  {
    strip.begin();
    strip.show();
  }

  void TurnOff() {
    setUniformColor(0);
    needsUpdate = true;
  }

  void UpdateState(int mode, byte c1[], byte c2[], int d) {
    activatedMode = mode;

    int i;
    for (i = 0; i < 3; ++i) {
      color1[i] = c1[i];
      color2[i] = c2[i];
    }
    duration = d;

    // Reset variables
    numberOfSteps = duration / updateIntervalTime;
    modeStep = 0;
  }

  void Update(unsigned long currentMillis)
  {
    if (activatedMode == 0) {
      TurnOff();
    }

    if (activatedMode == 1) {
      Pulse(currentMillis);
    }

    if (needsUpdate) {
      strip.show();
      needsUpdate = false;
    }
  }

  void setUniformColor(uint32_t c)
  {
    int i;
    
    for (i=0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
    }
  }

  void Pulse(unsigned long currentMillis)
  {
    if ((currentMillis - previousMillis) >= updateIntervalTime) {
       previousMillis = millis();
       setPulsate();
    }
  }

  void setPulsate()
  {
//    Serial.println(R);
    modeStep++;

    int rDiff = color2[0]-color1[0];
    int gDiff = color2[1]-color1[1];
    int bDiff = color2[2]-color1[2];

//        Serial.println(rDiff);

    float rVariation = (float)rDiff / (float)numberOfSteps;
    float gVariation = (float)gDiff / (float)numberOfSteps;
    float bVariation = (float)bDiff / (float)numberOfSteps;

//    Serial.println(rVariation);
//    Serial.println(rVariation);
    byte newRed = color1[0] + modeStep * rVariation;
    byte newGreen = color1[1] + modeStep * gVariation;
    byte newBlue = color1[2] + modeStep * bVariation;

//    byte newExpoRed = color1[0] + sign(rDiff) * (pow (2, (modeStep / R)) - 1);
//    Serial.println(newExpoRed);

//    Serial.println(sign(rDiff));
//        Serial.println(newRed);


    setUniformColor(Color(newRed,newGreen,newBlue));
    needsUpdate = true;
    if(modeStep >= numberOfSteps) {
      modeStep = 0;
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

uint16_t numberOfLeds = 12; // Number of LEDs on Adafruit Pixels
uint8_t dataPin  = 11;      // Yellow wire on Adafruit Pixels
uint8_t clockPin = 12;      // Green wire on Adafruit Pixels
uint16_t refreshTime = 20;  // refreshTime in milliseconds

LedStrip ledstrip(numberOfLeds, dataPin, clockPin, refreshTime);

//=============

void setup() {
  Serial.begin(9600);

  // LED strip
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
    clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
  #endif

  ledstrip.Begin();
  
  // tell the PC we are ready
  Serial.println("<Arduino is ready>");

  byte color1[3] = {255,0,255};
  byte color2[3] = {0,255,0};
  int duration = 1000;
  ledstrip.UpdateState(1, color1, color2, duration);
}


//=============
unsigned long curMillis;

//=============
void loop() {
  curMillis = millis();
  getDataFromRpi();
  setStateFromData();
  ledstrip.Update(curMillis);
}

//=============
// Receive data from Rpi and save it into inputBuffer
//=============
void getDataFromRpi() {
  if (Serial.available() > 0) {
    char x = Serial.read();

    // the order of these IF clauses is significant  
    if (x == endMarker) {
      readInProgress = false;
      newDataFromRpi = true;
      inputBuffer[bytesRecvd] = 0;
    }
    
    if(readInProgress) {
      inputBuffer[bytesRecvd] = x;
      bytesRecvd ++;
      if (bytesRecvd == buffSize) {
        bytesRecvd = buffSize - 1;
      }
    }

    if (x == startMarker) { 
      bytesRecvd = 0; 
      readInProgress = true;
    }
  }
}

// Data = L,M,args
void setStateFromData() {
  if (newDataFromRpi) {
    newDataFromRpi = false;

    char ** array = NULL;
    array = explodeDataIntoArray(inputBuffer);
    Serial.println(array[0]);
    Serial.println(array[1]);
  }
}

char ** explodeDataIntoArray(char string[]) {
    char ** res  = NULL;
    char *  p    = strtok (inputBuffer, ",");
    int n_spaces = 0, i;

    /* split string and append tokens to 'res' */
    while (p) {
      res = realloc (res, sizeof (char*) * ++n_spaces);
    
      if (res == NULL) {
        Serial.println("memory allocation failed");
        return; /* memory allocation failed */
      }

      res[n_spaces-1] = p;
    
      p = strtok (NULL, ",");
    }
    
    /* realloc one extra element for the last NULL */
    res = realloc (res, sizeof (char*) * (n_spaces+1));
    res[n_spaces] = 0;

    /* print the result */

    for (i = 0; i < (n_spaces+1); ++i) {
      Serial.println(res[i]);
    }

    return res;
}

