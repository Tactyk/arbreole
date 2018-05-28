#include <StandardCplusplus.h>
#include <serstream>
#include <string>
#include <vector>
#include <iterator>
#include <iostream>
#include <map>

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

/////////////////////////////////////////////

// SENSOR
#include <Arbreole_Sensor.h>
Arbreole_Sensor sensor = Arbreole_Sensor(13, 54, 0); // ledPin = 13; sensorPin = 54 (Analog IN 0 = 54 + 0); calibratingTime = 0

/////////////////////////////////////////////

// SPOT LIGHT
// Define Pins
#define RED 6
#define GREEN 5
#define BLUE 4

byte spotLightColor[3] = {0, 0, 0}; // {R, G, B}
int spotLightOnTime = 0;
boolean spotIsOn = false;
unsigned long previousSpotMillis = 0;
// END SPOT LIGHT

//! @jean Faire une classe pour ces données.
//! @jean genre class Input
// SERIAL VARIABLES
// struct SerialData
//{
//  bufferSize = 40;
//  inputBuffer[buffSize] = nullptr;
//}
// SerialData.bufferSize;
//

const byte buffSize = 40;
char inputBuffer[buffSize];
const char startMarker = '<';
const char endMarker = '>';
byte bytesRecvd = 0;
boolean readInProgress = false;
boolean newDataFromRpi = false;

enum class Mode : int
{
  Off = 0,
  Pulse
};

//==================================//
// Class that handles the LED Strip //
//==================================//
class LedStrip
{
public:

  // numberOfLeds: number of LEDs on Adafruit Pixels
  // dataPin: Yellow wire on Adafruit Pixels
  // clockPin: Green wire on Adafruit Pixels
  // refreshTime: refreshTime in milliseconds
  LedStrip(uint16_t numberOfLeds, uint8_t dataPin, uint8_t clockPin, uint16_t refreshTime);

  void Begin();

  void startMode(std::vector<std::string> const &input);

  void Update(unsigned long currentMillis);

private:
  void TurnOff();

  void setUniformColor(uint32_t c);

  void ActivatePulse();

  void Pulse(unsigned long currentMillis);

  void setPulseColors();

  uint32_t Color(byte r, byte g, byte b);

private:
  std::map<std::string, double> m_params;
  Adafruit_WS2801 m_strip;
  uint16_t m_updateIntervalTime;
  Mode m_activatedMode;
  unsigned long m_previousMillis = 0; // store last time the strip was updated
  bool m_needsUpdate = false;         // set to true when the colors change
};

LedStrip::LedStrip(uint16_t numberOfLeds, uint8_t dataPin, uint8_t clockPin, uint16_t refreshTime)
    : m_params(),
      m_strip(numberOfLeds, dataPin, clockPin),
      m_updateIntervalTime(refreshTime),
      m_activatedMode(Mode::Off),
      m_previousMillis(0),
      m_needsUpdate(false)
{
}

void LedStrip::Begin()
{
  m_strip.begin();
  m_strip.show();
}

void LedStrip::Update(unsigned long currentMillis)
{
  if (m_activatedMode == Mode::Pulse)
  {
    Pulse(currentMillis);
  }

  if (m_needsUpdate)
  {
    m_strip.show();
    m_needsUpdate = false;
  }
}

void LedStrip::TurnOff()
{
  Serial.println("Here in the function TurnOff");
  setUniformColor(0);
  m_needsUpdate = true;
}

void LedStrip::startMode(std::vector<std::string> const &input)
{
  m_params.clear();

  Mode mode = (Mode) atoi(input[1].c_str());

  switch (mode)
  {
  case Mode::Off:
  {
    TurnOff();
    break;
  }
  case Mode::Pulse:
  {
    Serial.println("Coucou le mode pulse");

    if (input.size() >= 10)
    {
      m_params["ncycles"] = atof(input[2].c_str());
      m_params["color1r"] = atof(input[3].c_str());
      m_params["color1g"] = atof(input[4].c_str());
      m_params["color1b"] = atof(input[5].c_str());
      m_params["color2r"] = atof(input[6].c_str());
      m_params["color2g"] = atof(input[7].c_str());
      m_params["color2b"] = atof(input[8].c_str());

      if (input.size() >= 11)
      {
        m_params["t1"] = atof(input[9].c_str());
        m_params["t2"] = atof(input[10].c_str());
      }
      else
      {
        m_params["t1"] = atof(input[9].c_str()) / 2.;
        m_params["t2"] = m_params["t1"];
      }

      ActivatePulse();
    }
    else
    {
      Serial.println("Invalid parameters for Pulse");
    }
    break;
  }
  }

  m_activatedMode = mode;
}

void LedStrip::ActivatePulse()
{
  Serial.println("Here in the function ActivatePulse");
  m_params["count"] = 0;
  m_params["nsteps1"] = m_params["t1"] / m_updateIntervalTime; // steps from color 1 to 2
  m_params["nsteps2"] = m_params["t2"] / m_updateIntervalTime; // steps from color 2 to 1
  m_params["step1"] = 0;
  m_params["step2"] = 0;
  m_params["phase"] = 1;
}

void LedStrip::Pulse(unsigned long currentMillis)
{
  if (m_params["count"] >= m_params["ncycles"])
  {
    Serial.println("<-1>");
    m_params["count"] = 0;
  }

  if ((currentMillis - m_previousMillis) >= m_updateIntervalTime)
  {
    m_previousMillis = millis();
    setPulseColors();
  }
}

void LedStrip::setPulseColors()
{
  if (m_params["phase"] == 1)
  {
    m_params["step1"]++;

    int rDiff = m_params["color2r"] - m_params["color1r"];
    int gDiff = m_params["color2g"] - m_params["color1g"];
    int bDiff = m_params["color2b"] - m_params["color1b"];

    float rVariation = (float)rDiff / m_params["nsteps1"];
    float gVariation = (float)gDiff / m_params["nsteps1"];
    float bVariation = (float)bDiff / m_params["nsteps1"];

    byte newRed = m_params["color1r"] + m_params["step1"] * rVariation;
    byte newGreen = m_params["color1g"] + m_params["step1"] * gVariation;
    byte newBlue = m_params["color1b"] + m_params["step1"] * bVariation;

    setUniformColor(Color(newRed, newGreen, newBlue));

    if (m_params["step1"] >= m_params["nsteps1"])
    {
      m_params["phase"] = 2;
      m_params["step1"] = 0;
    }
  }
  if (m_params["phase"] == 2)
  {
    m_params["step2"]++;

    int rDiff = m_params["color1r"] - m_params["color2r"];
    int gDiff = m_params["color1g"] - m_params["color2g"];
    int bDiff = m_params["color1b"] - m_params["color2b"];

    float rVariation = (float)rDiff / m_params["nsteps2"];
    float gVariation = (float)gDiff / m_params["nsteps2"];
    float bVariation = (float)bDiff / m_params["nsteps2"];

    byte newRed = m_params["color2r"] + m_params["step2"] * rVariation;
    byte newGreen = m_params["color2g"] + m_params["step2"] * gVariation;
    byte newBlue = m_params["color2b"] + m_params["step2"] * bVariation;

    setUniformColor(Color(newRed, newGreen, newBlue));

    if (m_params["step2"] >= m_params["nsteps2"])
    {
      m_params["phase"] = 1;
      m_params["step2"] = 0;
      m_params["count"]++;
    }
  }
}

// Create a 24 bit color value from R,G,B
uint32_t LedStrip::Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

void LedStrip::setUniformColor(uint32_t c)
{
  for (int i = 0; i < m_strip.numPixels(); i++)
  {
    m_strip.setPixelColor(i, c);
  }
  m_needsUpdate = true;
}

// SPOT LIGHT FUNCTIONS
void updateSpot(unsigned long currentMillis)
{
  if (spotIsOn)
  {
    analogWrite(RED, 255 - spotLightColor[0]);
    analogWrite(GREEN, 255 - spotLightColor[1]);
    analogWrite(BLUE, 255 - spotLightColor[2]);
    if ((currentMillis - previousSpotMillis) > spotLightOnTime)
    {
      Serial.println("SHOULD TURN OFF SPOT");
      spotIsOn = false;
      analogWrite(RED, 255);
      analogWrite(GREEN, 255);
      analogWrite(BLUE, 255);
    }
  }
}

LedStrip ledstrip(12, 11, 12, 20);

void setup()
{
  Serial.begin(57600);
  Serial.println("<Arduino is ready>");

  // LED strip
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  ledstrip.Begin();
  sensor.Begin();

  // SPOT
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);
  // END SPOT
}

//=============
unsigned long curMillis;

//=============
void loop()
{
  curMillis = millis();
  getDataFromRpi();
  handleInputData();
  ledstrip.Update(curMillis);

  updateSpot(millis());

  sensor.Update();
}

//=============
// Receive data from Rpi and save it into inputBuffer
//=============
void getDataFromRpi()
{
  if (Serial.available() > 0)
  {
    char x = Serial.read();

    // the order of these IF clauses is significant
    if (x == endMarker)
    {
      readInProgress = false;
      newDataFromRpi = true;
      inputBuffer[bytesRecvd] = 0;
    }

    if (readInProgress)
    {
      inputBuffer[bytesRecvd] = x;
      bytesRecvd++;
      if (bytesRecvd == buffSize)
      {
        bytesRecvd = buffSize - 1;
      }
    }

    if (x == startMarker)
    {
      bytesRecvd = 0;
      readInProgress = true;
    }
  }
}

void explodeDataIntoArray(std::vector<std::string> &output, char const *const input)
{
  output.clear();

  char *input_cp = (char *)malloc(sizeof(char) * strlen(input));

  strcpy(input_cp, input);

  char *p = strtok(input_cp, ",");
  while (p != 0)
  {
    output.push_back(std::string(p));
    p = strtok(NULL, ",");
  }

  free(input_cp);
}

void handleInputData()
{
  if (newDataFromRpi)
  {
    newDataFromRpi = false;

    //
    Serial.println("");
    Serial.print("<");
    Serial.print(inputBuffer);
    Serial.println(">");
    //

    std::vector<std::string> inputData;
    explodeDataIntoArray(inputData, inputBuffer);

    if (inputData[0] == "L")
    {
      Serial.println("Coucou le led strip");

      ledstrip.startMode(inputData);
    }
    else if (inputData[0] == "S")
    {
      Serial.println("Coucou le SPOT");

      if (inputData[1] == "1")
      {
        Serial.println("Coucou le mode 1 du spot");

        spotLightColor[0] = atoi(inputData[2].c_str());
        spotLightColor[1] = atoi(inputData[3].c_str());
        spotLightColor[2] = atoi(inputData[4].c_str());
        spotLightOnTime = atoi(inputData[5].c_str());
        spotIsOn = true;
        previousSpotMillis = millis();
      }
    }
  }
}

