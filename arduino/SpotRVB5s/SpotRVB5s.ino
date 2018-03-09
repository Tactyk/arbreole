/*
 *
 *- Le spot light fonctionne comme 3 LEDs R,G,B controllable via 3 sorties digitales PWM de l'Arduino.
 *- Afin de ne pas bruler le composant : brancher des résistances (1000 Ohm) en série avec les LEDs
 *
 *==============
 *- Led RED : white wire
 *- Led GREEN: purple wire
 *- Led BLUE: grey wire
 *
 *- +5V : red wire
 *- GND : brown wire
 */

/*
 * autuer Iynthurisha
 * Code Arduino permettant d'allumer le spot douche 5secondes en blue puis 5s en vert et enfin 5s en rouge 
 * 
 */


// Define Pins
#define GREEN 12
#define BLUE 11
#define RED 10

// define variables
int redValue;
int greenValue;
int blueValue;
int tempsd; //temps t=0
int tempsf; // temps à l'instant t

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  tempsd = millis();
  
  redValue = 255;
  greenValue = 255;
  blueValue = 255;
  
 }
void loop() {
  analogWrite(RED, redValue);
  analogWrite(GREEN, greenValue);
  analogWrite(BLUE, blueValue); 
  
  tempsf=millis();
  if (tempsf-tempsd <5000){
     //do 5s spot in blue
    redValue = 255; 
    greenValue = 255;
    blueValue = 0; // it will show blue light (the color values are inversed)

    analogWrite(RED, redValue);
    analogWrite(GREEN, greenValue);
    analogWrite(BLUE, blueValue);
    tempsf=millis();
  }

    else if (tempsf-tempsd < 10000){
     //do 5s spot in green
    redValue = 255; 
    greenValue = 0;
    blueValue = 255;

    analogWrite(RED, redValue);
    analogWrite(GREEN, greenValue);
    analogWrite(BLUE, blueValue);
    tempsf=millis();
    }
      else if (tempsf-tempsd < 15000){
     //do 5s spot in red
    redValue = 0; 
    greenValue = 255;
    blueValue = 255;

    analogWrite(RED, redValue);
    analogWrite(GREEN, greenValue);
    analogWrite(BLUE, blueValue);
      }
    else { //then off
    redValue = 255; 
    greenValue = 255;
    blueValue = 255;

    analogWrite(RED, redValue);
    analogWrite(GREEN, greenValue);
    analogWrite(BLUE, blueValue);
      }

    }

