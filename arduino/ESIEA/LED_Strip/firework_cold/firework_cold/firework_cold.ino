//Importation des bibliothèques
#include "Adafruit_WS2801.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
#include <avr/power.h>
#endif

//Déclaration des pins des câbles jaune et vert
uint8_t dataPin  = 2;    // Câble jaune
uint8_t clockPin = 3;    // Câble vert

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
}

//Définition du tableau des leds (disposition)
int led[3][4] = { // tableau de char à 2 dimensions
  {0,5,6,11},
  {1,4,7,10},
  {2,3,8,9}
};

void loop() {
  //cold_rainbow(450);
  //vertical_bar(led,200);
  //horizontal_bar(led,200);
  //horizontal_spiral_lt(led,100);
  //horizontal_spiral_tl(led,200);
  //shift_spiral_lt(125);
  //shift_spiral_tl(125);
  firework_cold(50);
  //firework_hot(50);
  //degree_spiral(led,100);  
}

/*Déclaration des fonctions de base*/
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


//Feu d'artifice froid
void firework_cold(uint16_t timer){
  int i = random (0,12);
  int j = random (0,2);
  for (int h=0;h<timer;h++){
    switch(j){
      case 0 : 
        strip.setPixelColor(i, Color(102,178,255));
        break;
      case 1 : 
        strip.setPixelColor(i, Color(102,102,255));
        break;
      default :
        strip.setPixelColor(i, Color(178,102,255));
        break;
    }
    strip.show();
    strip.setPixelColor(i, Color(0,0,0));
    strip.show();
  }
}
