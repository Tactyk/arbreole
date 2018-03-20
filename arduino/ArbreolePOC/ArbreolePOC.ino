// LED strip
#include "Adafruit_WS2801.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
 #include <avr/power.h>
#endif

uint8_t dataPin  = 11;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = 12;    // Green wire on Adafruit Pixels

Adafruit_WS2801 strip = Adafruit_WS2801(12, dataPin, clockPin);
// end LED strip

// LED STRIP VARIABLES
const byte numColors = 3;
byte ledStripColor[numColors] = {0, 0, 0}; // {R, G, B} 
int ledStripState = LOW; 
unsigned long lastUpdate = 0;
int ledStripBlinkInterval = 0;

// SPOT LIGHT
// Define Pins
#define RED 6
#define GREEN 5
#define BLUE 4

byte spotLightColor[3] = {0, 0, 0}; // {R, G, B}
int spotLightOnTime = 0;
// END SPOT LIGHT


// SHARP SENSORS
const int sensorPin1 = 1;
const int sensorPin2 = 2;
const int sensorPin3 = 3;

unsigned long prevSendSensorsValuemillis = 0;
unsigned long sendSensorsValueInterval = 4000;
// END SHARP SENSORS

// SERIAL VARIABLES
const byte buffSize = 40;
char inputBuffer[buffSize];
const char startMarker = '<';
const char endMarker = '>';
byte bytesRecvd = 0;
boolean readInProgress = false;
boolean newDataFromPC = false;

char messageFromPC[buffSize] = {0};

unsigned long curMillis;

unsigned long prevReplyToPCmillis = 0;
unsigned long replyToPCinterval = 3500;

//=============

void setup() {
  Serial.begin(9600);

  // LED strip
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
    clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
  #endif

  strip.begin();

  // Update LED contents, to start they are all 'off'
  strip.show();
  // end LED strip

  // SPOT
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);
  // END SPOT
  
  // tell the PC we are ready
  Serial.println("<Arduino is ready>");
}

//=============

void loop() {
  curMillis = millis();
  getDataFromPC();
  ledstrip();
  spotLight();
  sendSensorsValues();
//  replyToPC();
  sendToPC();
}

//=============

void getDataFromPC() {

    // receive data from PC and save it into inputBuffer
    
  if(Serial.available() > 0) {

    char x = Serial.read();

      // the order of these IF clauses is significant
      
    if (x == endMarker) {
      readInProgress = false;
      newDataFromPC = true;
      inputBuffer[bytesRecvd] = 0;
      parseData();
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

//=============
 
void parseData() {

    // split the data into its parts
    // assumes the data will be received as (eg) 0,1,35
    
  char * strtokIndx; // this is used by strtok() as an index
  
  strtokIndx = strtok(inputBuffer,","); // get the first part
  strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC

  if (strcmp(messageFromPC, "L") == 0) {
    Serial.println("coucou le led strip");
  
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    ledStripColor[0] = atoi(strtokIndx); //  convert to an integer
    
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    ledStripColor[1] = atoi(strtokIndx);
  
    strtokIndx = strtok(NULL, ","); 
    ledStripColor[2] = atoi(strtokIndx); 
  
    strtokIndx = strtok(NULL, ","); 
    ledStripBlinkInterval = atoi(strtokIndx);
  } else if (strcmp(messageFromPC, "S") == 0) {
    Serial.println("coucou le spot");
  
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    spotLightColor[0] = atoi(strtokIndx); //  convert to an integer
    
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    spotLightColor[1] = atoi(strtokIndx);
  
    strtokIndx = strtok(NULL, ","); 
    spotLightColor[2] = atoi(strtokIndx); 
  }
   
}

//=============

void replyToPC() {

  if (newDataFromPC) {
    newDataFromPC = false;
    Serial.print("<R ");
    Serial.print(ledStripColor[0]);
    Serial.print(" G ");
    Serial.print(ledStripColor[1]);
    Serial.print(" B ");
    Serial.print(ledStripColor[2]);
    Serial.print(" Time ");
    Serial.print(curMillis >> 9); // divide by 512 is approx = half-seconds
    Serial.println(">");
  }
}

//=============

void sendToPC() {
	if (curMillis - prevReplyToPCmillis >= replyToPCinterval) {
		prevReplyToPCmillis += replyToPCinterval;
		int valForPC = curMillis >> 9; // approx half seconds
		Serial.print('<');
		Serial.print(valForPC);
		Serial.print('>');
	}	
}

// LED STRIPS FUNCTIONS

// fill the dots one after the other with said color
// good for testing purposes
void colorAll(uint32_t c) {
  int i;
  
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
  }
  strip.show();
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

void ledstrip() {
  if (ledStripBlinkInterval == 0) {
     colorAll(Color(ledStripColor[0], ledStripColor[1], ledStripColor[2]));
  }
    if((curMillis - lastUpdate) > ledStripBlinkInterval && ledStripBlinkInterval != 0)  // time to update
    {
      lastUpdate = millis();
      if (ledStripState == LOW) {
        ledStripState = HIGH;
        colorAll(Color(ledStripColor[0], ledStripColor[1], ledStripColor[2]));
      } else {
        ledStripState = LOW;
        colorAll(Color(0,0,0));
      }
    }
}


// SPOT LIGHT FUNCTIONS
void spotLight() {
  analogWrite(RED, 255 - spotLightColor[0]);
  analogWrite(GREEN, 255 - spotLightColor[1]);
  analogWrite(BLUE, 255 - spotLightColor[2]); 
}


// SENSORS FUNCTIONS

int sensorValue1;
int sensorValue2;
int sensorValue3;
int distance1;
int distance2;
int distance3;

void sendSensorsValues() {
  if (curMillis - prevSendSensorsValuemillis >= sendSensorsValueInterval) {
    prevSendSensorsValuemillis += sendSensorsValueInterval;

    sensorValue1 = analogRead(sensorPin1);
    distance1 = 28250 / (sensorValue1-229.5);
 
    sensorValue2 = analogRead(sensorPin2);  
    distance2 = 28250 / (sensorValue2-229.5);
 
    sensorValue3 = analogRead(sensorPin3);   
    distance3 = 28250 / (sensorValue3-229.5);


  Serial.print("distance1 : ");
  Serial.print(distance1);
  Serial.print("cm   ");

  Serial.print("distance2 : ");
  Serial.print(distance2);
  Serial.print("cm   ");

  Serial.print("distance3 : ");
  Serial.print(distance3);
  Serial.println("cm   ");
  }
}

