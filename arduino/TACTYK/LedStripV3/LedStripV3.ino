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

//==================================//
// Class that handles the LED Strip //
//==================================//
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

  // Pulse 2
  int T1;
  int T2;

  // Variables useful in handling each behaviour
  unsigned long previousMillis = 0;   // store last time the strip was updated
  int numberOfSteps;
  int modeStep;

  // Pulse 2
  int step1;
  int step2;
  int n_steps1;
  int n_steps2;
  int phase;
  int step;

  boolean needsUpdate = false; // set to true when the colors change

  // Other variables
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

  void Begin() {
    strip.begin();
    strip.show();
  }

  void Update(unsigned long currentMillis) {
    if (activatedMode == 1) {
      Pulse(currentMillis);
    }

    if (activatedMode == 2) {
      Pulse2(currentMillis);
    }

    if (needsUpdate) {
      strip.show();
      needsUpdate = false;
    }
  }

  void TurnOff() {
    Serial.println("Here in the function TurnOff");
    activatedMode = 0;
    setUniformColor(0);
    needsUpdate = true;
  }

  void ActivatePulse(int mode, byte c1[], byte c2[], int d) {
    Serial.println("Here in the function ActivatePulse");
    activatedMode = mode;

    int i;
    for (i = 0; i < 3; ++i) {
      color1[i] = c1[i];
      color2[i] = c2[i];
    }
    duration = d;

    // Reset variables
    numberOfSteps = duration / updateIntervalTime; // steps from color 1 to 2
    modeStep = 0;
  }

  void ActivatePulse2(int mode, byte c1[], byte c2[], int d1, int d2) {
    Serial.println("Here in the function ActivatePulse 2");
    activatedMode = mode;

    int i;
    for (i = 0; i < 3; ++i) {
      color1[i] = c1[i];
      color2[i] = c2[i];
    }

    T1 = d1;
    T2 = d2;

    // Reset variables
    n_steps1 = T1 / updateIntervalTime; // steps from color 1 to 2
    n_steps2 = T2 / updateIntervalTime; // steps from color 2 to 1
    step = 0;
    phase = 1;
  }

  void Pulse(unsigned long currentMillis) {
    if ((currentMillis - previousMillis) >= updateIntervalTime) {
       previousMillis = millis();
       setPulseColors();
    }
  }

  void setPulseColors() {
    modeStep++;

    int rDiff = color2[0]-color1[0];
    int gDiff = color2[1]-color1[1];
    int bDiff = color2[2]-color1[2];

    float rVariation = (float)rDiff / (float)numberOfSteps;
    float gVariation = (float)gDiff / (float)numberOfSteps;
    float bVariation = (float)bDiff / (float)numberOfSteps;

    byte newRed = color1[0] + modeStep * rVariation;
    byte newGreen = color1[1] + modeStep * gVariation;
    byte newBlue = color1[2] + modeStep * bVariation;

//    byte newExpoRed = color1[0] + sign(rDiff) * (pow (2, (modeStep / R)) - 1);
//    Serial.println(newExpoRed);

    setUniformColor(Color(newRed,newGreen,newBlue));

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

  void Pulse2(unsigned long currentMillis) {
    if ((currentMillis - previousMillis) >= updateIntervalTime) {
       previousMillis = millis();
       setPulseColors2();
    }
  }

  void setPulseColors2() {
    if (phase == 1) {
      step1++;

      int rDiff = color2[0]-color1[0];
      int gDiff = color2[1]-color1[1];
      int bDiff = color2[2]-color1[2];

      float rVariation = (float)rDiff / (float)n_steps1;
      float gVariation = (float)gDiff / (float)n_steps1;
      float bVariation = (float)bDiff / (float)n_steps1;

      byte newRed = color1[0] + step1 * rVariation;
      byte newGreen = color1[1] + step1 * gVariation;
      byte newBlue = color1[2] + step1 * bVariation;

      setUniformColor(Color(newRed,newGreen,newBlue));

      if (step1 >= n_steps1) {
        phase = 2;
        step1 = 0;
      }
    }
    if (phase == 2) {
      step2++;

      int rDiff = color1[0]-color2[0];
      int gDiff = color1[1]-color2[1];
      int bDiff = color1[2]-color2[2];

      float rVariation = (float)rDiff / (float)n_steps2;
      float gVariation = (float)gDiff / (float)n_steps2;
      float bVariation = (float)bDiff / (float)n_steps2;

      byte newRed = color2[0] + step2 * rVariation;
      byte newGreen = color2[1] + step2 * gVariation;
      byte newBlue = color2[2] + step2 * bVariation;

      setUniformColor(Color(newRed,newGreen,newBlue));

      if (step2 >= n_steps2) {
        phase = 1;
        step2 = 0;
      }
    }
  }

  int sign(int x) {
    return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
  }

  // Create a 24 bit color value from R,G,B
  uint32_t Color(byte r, byte g, byte b) {
    uint32_t c;
    c = r;
    c <<= 8;
    c |= g;
    c <<= 8;
    c |= b;
    return c;
  }

  void setUniformColor(uint32_t c) {
    int i;

    for (i=0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
    }
    needsUpdate = true;
  }
};

//=============
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

//
    Serial.print("<");
    Serial.print(inputBuffer);
    Serial.println(">");
//

    char ** arr= NULL;
    arr = explodeDataIntoArray(inputBuffer);

    if (strcmp(arr[0], "L") == 0) {
      Serial.println("Coucou le led strip");
      if (strcmp(arr[1], "0") == 0) {
        Serial.println("Coucou le mode 0");
        ledstrip.TurnOff();
      }

      if (strcmp(arr[1], "1") == 0) {
        Serial.println("Coucou le mode 1");
        byte color1[3] = {atoi(arr[2]),atoi(arr[3]),atoi(arr[4])};
        byte color2[3] = {atoi(arr[5]),atoi(arr[6]),atoi(arr[7])};
        int T = atoi(arr[8]);

        ledstrip.ActivatePulse(atoi(arr[1]), color1, color2, T);
      }

      if (strcmp(arr[1], "2") == 0) {
        Serial.println("Coucou le mode 2");
        byte color1[3] = {atoi(arr[2]),atoi(arr[3]),atoi(arr[4])};
        byte color2[3] = {atoi(arr[5]),atoi(arr[6]),atoi(arr[7])};
        int T1 = atoi(arr[8]);
        int T2 = atoi(arr[9]);
        ledstrip.ActivatePulse2(atoi(arr[1]), color1, color2, T1, T2);
      }
    }
  }
}

char ** explodeDataIntoArray(char string[]) {
    char ** res  = NULL;
    char *  p    = strtok (inputBuffer, ",");
    int arrayLength = 0, i;

    /* split string and append tokens to 'res' */
    while (p) {
      res = realloc (res, sizeof (char*) * ++arrayLength);
    
      if (res == NULL) {
        Serial.println("memory allocation failed");
        return; /* memory allocation failed */
      }

      res[arrayLength-1] = p;
    
      p = strtok (NULL, ",");
    }
    
    /* realloc one extra element for the last NULL */
    res = realloc (res, sizeof (char*) * (arrayLength+1));
    res[arrayLength] = 0;

    return res;
}

int getArrayLength(char ** arr) {
  int arrayLength = 0;
  while(arr[arrayLength]) {
    arrayLength++;
  }

  return arrayLength;
}
