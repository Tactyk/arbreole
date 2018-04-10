/******************************************************************************
PIR_Motion_Detector_Example.ino
Example sketch for SparkFun's PIR Motion Detector
  (https://www.sparkfun.com/products/13285)
Jim Lindblom @ SparkFun Electronics
May 2, 2016

The PIR motion sensor has a three-pin JST connector terminating it. Connect
the wire colors like this:
- Black: D2 - signal output (pulled up internally)
- White: GND
- Red: 5V

Connect an LED to pin 13 (if your Arduino doesn't already have an LED there).

Whenever the PIR sensor detects movement, it'll write the alarm pin LOW.

Development environment specifics:
Arduino 1.6.7
******************************************************************************/
const int MOTION_PIN_SF = 2; // Pin connected to Sparkfun motion detector
const int LED_PIN_SF = 13; // LED pin - active-high
int stateSF = LOW;
int proximitySF = 0;

const int MOTION_PIN_ADA = 3;               // choose the input pin (for Adafruit PIR sensor)
const int LED_PIN_ADA = 12; // LED pin - active-high
int stateADA = LOW;
int proximityADA = 0;

void setup() 
{
  Serial.begin(9600);
  // The PIR sensor's output signal is an open-collector, 
  // so a pull-up resistor is required:
  pinMode(MOTION_PIN_SF, INPUT_PULLUP);
  pinMode(LED_PIN_SF, OUTPUT);

  pinMode(MOTION_PIN_ADA, INPUT);     // declare ada sensor as input
  pinMode(LED_PIN_ADA, OUTPUT);
}

void loop() 
{
  proximitySF = digitalRead(MOTION_PIN_SF);
  if (proximitySF == LOW) // If the sensor's output goes low, motion is detected
  {
    digitalWrite(LED_PIN_SF, HIGH);
    if (stateSF == LOW) {
      Serial.println("SF Motion detected!");
      stateSF = HIGH;
    }
  } else {
    digitalWrite(LED_PIN_SF, LOW);
    if (stateSF == HIGH) {
      Serial.println("SF Motion ended!");
      stateSF = LOW;
    }
  }

  proximityADA = digitalRead(MOTION_PIN_ADA);
  if (proximityADA == HIGH) // If the sensor's output goes HIGH, motion is detected
  {
    digitalWrite(LED_PIN_ADA, HIGH);
    if (stateADA == LOW) {
      Serial.println("ADA Motion detected!");
      stateADA = HIGH;
    }
  } else {
    digitalWrite(LED_PIN_ADA, LOW);
    if (stateADA == HIGH) {
      Serial.println("ADA Motion ended!");
      stateADA = LOW;
    }
  }
}

