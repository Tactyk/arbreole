//Importation des bibliothèques
#include "Adafruit_WS2801.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
#include <avr/power.h>
#endif

//Déclaration des pins des câbles jaune et vert  ****LED****
uint8_t dataPin  = 2;    // Câble jaune
uint8_t clockPin = 3;    // Câble vert
uint32_t d_temps_rouge = millis(); //32 bit au lieu de 16 bit pour éviter le bug a 1min6s
uint32_t d_temps_vert = millis();
uint32_t d_temps_bleu = millis();
byte r=0;
byte g=150;
byte b=255;


//Déclaration des pins des câbles jaune et vert  ****SPOT****
// Define Pins
#define GREEN 12 // Câble violet
#define BLUE 11 // Câble gris
#define RED 10 // Câble blanc



//Déclaration du nombre de leds sur la guirlande
Adafruit_WS2801 strip = Adafruit_WS2801(12, dataPin, clockPin);

//Définition de la fréquence de communication et mise en place des leds en mode "on"
void setup() {
  Serial.begin(9600);
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
    clock_prescale_set(clock_div_1); 
  #endif
  strip.begin();
  strip.show();


  //*************spott****************//

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  analogWrite(RED, r);
  analogWrite(GREEN, g);
  analogWrite(BLUE, b);
}

void loop() {
  cold_rainbow_LED(65,85,2000);
  cold_rainbow_SPOT(65,85,2000);
}

//Création de la couleur à partir du code RGB
uint32_t Color(byte r, byte g, byte b){
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

/*Déclaration des fonctions*/
void cold_rainbow_LED(uint16_t wait1, uint16_t wait2, uint16_t wait3) {
  //r = 0;
  //g = 150;
  //b = 255;
  int i;
  if( (millis() - d_temps_rouge > wait1) && (r < 155) ) {
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Color(r,g,b));
     }
     strip.show();
     r=r+1;
     d_temps_rouge = millis();
  }
  if( (millis() - d_temps_vert > wait2) && (g > 31) ) {
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Color(r,g,b));
    }
    strip.show();
    g=g-1;
    d_temps_vert = millis();
  }
  if( (millis() - d_temps_bleu > wait3) && (b > 249) ) {
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Color(r,g,b));
    }
    strip.show();
    b=b-1;
    Serial.print(b);
    Serial.print("\n");
    d_temps_bleu = millis();
  }
  //Pour bouclé le comportement (sans transition)
  if( (r >= 154) && (g <= 32) && (b <= 249) ) {
    r = 0;
    g = 150;
    b = 255;
    strip.show();
  }

}
  //----------------------SPOT------------------//

  void cold_rainbow_SPOT(uint16_t wait1, uint16_t wait2, uint16_t wait3) {
  //r = 0;
  //g = 150;
  //b = 255;
  int i;
  if( (millis() - d_temps_rouge > wait1) && (r < 155) ) {
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Color(r,g,b));
     }
    
    analogWrite(RED, 255-r);
    analogWrite(GREEN, 255-g);
    analogWrite(BLUE, 255-b);
     r=r+1;
     d_temps_rouge = millis();
     

  }
  if( (millis() - d_temps_vert > wait2) && (g > 31) ) {
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Color(r,g,b));
    }
  
    analogWrite(RED, 255-r);
    analogWrite(GREEN, 255-g);
    analogWrite(BLUE, 255-b);
    g=g-1;
    d_temps_vert = millis();
  }
  if( (millis() - d_temps_bleu > wait3) && (b > 249) ) {
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Color(r,g,b));
    }
   
    analogWrite(RED, 255-r);
    analogWrite(GREEN, 255-g);
    analogWrite(BLUE, 255-b);
    b=b-1;
    Serial.print(b);
    Serial.print("\n");
    d_temps_bleu = millis();
  }
  //Pour bouclé le comportement (sans transition)
  if( (r >= 154) && (g <= 32) && (b <= 249) ) {
    r = 0;
    g = 150;
    b = 255;
    analogWrite(RED, 255-r);
    analogWrite(GREEN, 255-g);
    analogWrite(BLUE, 255-b);

  }
}
