#include <StandardCplusplus.h>
#include <serstream>
#include <string>
#include <vector>
#include <iterator>

using namespace std;

// <iostream> declares cout/cerr, but the application must define them
// because it's up to you what to do with them.
namespace std
{
ohserialstream cout(Serial);
}
//////////////////////////////////////////////

// LED strip
#include "Adafruit_WS2801.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
#include <avr/power.h>
#endif

//! @jean Faire une classe pour ces données.
//! @jean genre class Input
// SERIAL VARIABLES
const byte buffSize = 40;
char inputBuffer[buffSize];
const char startMarker = '<';
const char endMarker = '>';
byte bytesRecvd = 0;
boolean readInProgress = false;
boolean newDataFromRpi = false;
//

//! @jean Si tu veux vraiment un design clean et ne pas faire grossir ta classe
//! LedStrip à chaque nouveau comportement je ferais un pattern stratégie.
//! Traduction une classe mère de mode Mode et des classes filles genre Pulse1, Pulse2 ect.
//! Ledstrip a un pointeur de Mode et à chaque fois que tu active un nouvea mode,
//! tu fais delete m_mode; et m_mode = new Pulse1() du genre.

// class Mode
// {
// public:
//
//     virtual update(unsigned long currentMillis) = 0;
// }
//
// class Pulse1 : public Mode
// {
// public:
//
//     Pulse1(...);
//
//     udpate(unsigned long currentMillis)
//     {
//         ... équivalent de ta méthode Pulse
//     }
//
//     ~Pulse1();
//
// private:
//
//     //data
// }
//
// class LedStrip
// {
// public:
//
//     LedStrip();
//
//     ~LedStrip()
//     {
//         if (m_mode)
//         {
//             delete m_mode;
//         }
//     }
//
//     activatePulse1(...)
//     {
//         if (m_current_mode != 0)
//         {
//             delete m_mode
//         }
//
//         m_mode = new Pulse1(....);
//     }
//
// private:
//
//     int     m_current_mode; // ou enum
//     Mode *  m_mode;
// }

//==================================//
// Class that handles the LED Strip //
//==================================//
class LedStrip
{

    //! @jean Tu as bien mis le mot clés public mais je mettrais aussi le mot clés
    //! private pour les variables. C'est plus clair même si elles sont par défaut
    //! private.

    //! @jean Par convention on met souvent l'"interface" public d'une classe en
    //! premier car pour la lecture c'est plus simple de mettre ce qu'on va être
    //! susceptible d'appeler.

    //! @jean Souvent en C++ pour distinguer varialbe membre d'une classe def
    //! variable tout cours on met m_ devant les variables membres. Très pratique
    //! et assez classique comme convention.
    //! ex. m_strip, m_udpateIntervalTime.

    //! @jean Un truc du genre.
    // class LedStrip
    // {
    // public:
    //
    //     LedStrip();
    //
    //     LedStrip();
    // private:
    //
    //     adafruite m_strip;
    //     uint16_t m_updateIntervalTime;
    // }

    // Variables initialized in constructor
    Adafruit_WS2801 m_strip;
    uint16_t m_updateIntervalTime;

    //! @jean pour activate mode j'utiliserais un enum plutôt.
    //! en dehors de la classe tu définis la class enum Mode.
    // enum class Mode
    // {
    //     Off,
    //     Ramp1,
    //     Ramp2
    // }
    // et dans la classe tu déclare un mode.
    // Mode activatedMode = Mode::Off;

    //! @jean pour la lecture je ferais des classes de data par mode aussi.

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

    //! @jean Oulala t'initialises rien dans ton constructeur.
    //! en C++ toutes les variables doivent être initialisé parce qu'en gros
    //! les variables peuvent valoirs n'importent quoi sinon. après ton constructeur
    //! step1 peut valoir 4 comme 10000 (dépendement du compilateur)
    //! On utilise souvent une synthaxe initialiser list pour ça. donc

    // LedStrip(uint16_t numberOfLeds, uint8_t dataPin, uint8_t clockPin, uint16_t refreshTime):
    // strip(),
    // m_updateIntervalTime(0),
    // activatedMode(),
    // duration()
    // ect...
    // {
    //
    // }

    LedStrip(uint16_t numberOfLeds, uint8_t dataPin, uint8_t clockPin, uint16_t refreshTime)
    {
      // dataPin is the Yellow wire on Adafruit Pixels
      // clockPin is the Green wire on Adafruit Pixels
      m_strip = Adafruit_WS2801(numberOfLeds, dataPin, clockPin);
      m_updateIntervalTime = refreshTime;

      // Calculate the R variable (only needs to be done once at setup)
      //    R = (numberOfSteps * log10(2))/(log10(255));
    }

    void Begin() {
      m_strip.begin();
      m_strip.show();
    }

    void Update(unsigned long currentMillis) {
      if (activatedMode == 1) {
        Pulse(currentMillis);
      }

      if (activatedMode == 2) {
        Pulse2(currentMillis);
      }

      if (needsUpdate) {
        m_strip.show();
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
      numberOfSteps = duration / m_updateIntervalTime; // steps from color 1 to 2
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
      n_steps1 = T1 / m_updateIntervalTime; // steps from color 1 to 2
      n_steps2 = T2 / m_updateIntervalTime; // steps from color 2 to 1
      step = 0;
      phase = 1;
    }

    void Pulse(unsigned long currentMillis) {
      if ((currentMillis - previousMillis) >= m_updateIntervalTime) {
        previousMillis = millis();
        setPulseColors();
      }
    }

    void setPulseColors() {
      modeStep++;

      int rDiff = color2[0] - color1[0];
      int gDiff = color2[1] - color1[1];
      int bDiff = color2[2] - color1[2];

      float rVariation = (float)rDiff / (float)numberOfSteps;
      float gVariation = (float)gDiff / (float)numberOfSteps;
      float bVariation = (float)bDiff / (float)numberOfSteps;

      byte newRed = color1[0] + modeStep * rVariation;
      byte newGreen = color1[1] + modeStep * gVariation;
      byte newBlue = color1[2] + modeStep * bVariation;

      //    byte newExpoRed = color1[0] + sign(rDiff) * (pow (2, (modeStep / R)) - 1);
      //    Serial.println(newExpoRed);

      setUniformColor(Color(newRed, newGreen, newBlue));

      if (modeStep >= numberOfSteps) {
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
      if ((currentMillis - previousMillis) >= m_updateIntervalTime) {
        previousMillis = millis();
        setPulseColors2();
      }
    }

    void setPulseColors2() {
      if (phase == 1) {
        step1++;

        int rDiff = color2[0] - color1[0];
        int gDiff = color2[1] - color1[1];
        int bDiff = color2[2] - color1[2];

        float rVariation = (float)rDiff / (float)n_steps1;
        float gVariation = (float)gDiff / (float)n_steps1;
        float bVariation = (float)bDiff / (float)n_steps1;

        byte newRed = color1[0] + step1 * rVariation;
        byte newGreen = color1[1] + step1 * gVariation;
        byte newBlue = color1[2] + step1 * bVariation;

        setUniformColor(Color(newRed, newGreen, newBlue));

        if (step1 >= n_steps1) {
          phase = 2;
          step1 = 0;
        }
      }
      if (phase == 2) {
        step2++;

        int rDiff = color1[0] - color2[0];
        int gDiff = color1[1] - color2[1];
        int bDiff = color1[2] - color2[2];

        float rVariation = (float)rDiff / (float)n_steps2;
        float gVariation = (float)gDiff / (float)n_steps2;
        float bVariation = (float)bDiff / (float)n_steps2;

        byte newRed = color2[0] + step2 * rVariation;
        byte newGreen = color2[1] + step2 * gVariation;
        byte newBlue = color2[2] + step2 * bVariation;

        setUniformColor(Color(newRed, newGreen, newBlue));

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

      for (i = 0; i < m_strip.numPixels(); i++) {
        m_strip.setPixelColor(i, c);
      }
      needsUpdate = true;
    }
};

//! @jean Pas util de déclarer les variables numOfLeds, dataPin ect...
//! Elles sont gardé en mémoire cache après et accessibles après.
//! LedStrip ledstrip(12, 11, 12, 20) la doc du constructeur suffis à comprendre

//=============
uint16_t numberOfLeds = 12; // Number of LEDs on Adafruit Pixels
uint8_t dataPin  = 11;      // Yellow wire on Adafruit Pixels
uint8_t clockPin = 12;      // Green wire on Adafruit Pixels
uint16_t refreshTime = 20;  // refreshTime in milliseconds

LedStrip ledstrip(numberOfLeds, dataPin, clockPin, refreshTime);
//=============

void setup() {
  Serial.begin(57600);

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
  handleInputData();
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

    if (readInProgress) {
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
void handleInputData() {
  if (newDataFromRpi) {
    newDataFromRpi = false;

    //
    Serial.println("");
    Serial.print("<");
    Serial.print(inputBuffer);
    Serial.println(">");
    //

    std::vector<std::string> inputData;
    explodeDataIntoArray(inputData, inputBuffer);

    if (inputData[0] == "L") {
      Serial.println("Coucou le led strip");

      if (inputData[1] == "0") {
        Serial.println("Coucou le mode 0");

        ledstrip.TurnOff();
      } else if (inputData[1] == "1") {
        Serial.println("Coucou le mode 1");

        byte color1[3] = {atoi(inputData[2].c_str()), atoi(inputData[3].c_str()), atoi(inputData[4].c_str())};
        byte color2[3] = {atoi(inputData[5].c_str()), atoi(inputData[6].c_str()), atoi(inputData[7].c_str())};
        int T = atoi(inputData[8].c_str());
        ledstrip.ActivatePulse(atoi(inputData[1].c_str()), color1, color2, T);

      } else if (inputData[1] == "2") {
        Serial.println("Coucou le mode 2");

        byte color1[3] = {atoi(inputData[2].c_str()), atoi(inputData[3].c_str()), atoi(inputData[4].c_str())};
        byte color2[3] = {atoi(inputData[5].c_str()), atoi(inputData[6].c_str()), atoi(inputData[7].c_str())};
        int T1 = atoi(inputData[8].c_str());
        int T2 = atoi(inputData[9].c_str());
        ledstrip.ActivatePulse2(atoi(inputData[1].c_str()), color1, color2, T1, T2);
      }
    }
  }
}

void explodeDataIntoArray(std::vector<std::string> & output, char const * const input)
{
  output.clear();

  char * p = strtok(input, ",");
  while (p != 0)
  {
    output.push_back(std::string(p));
    p = strtok (NULL, ",");
  }
}
