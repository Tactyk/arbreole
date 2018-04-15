//Importation des bibliothèques
#include "Adafruit_WS2801.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
#include <avr/power.h>
#endif

//Déclaration des pins des câbles jaune et vert
uint8_t dataPin  = 2;    // Câble jaune
uint8_t clockPin = 3;    // Câble vert
uint32_t d_temps = millis();
int i = 2;
int j = 3;

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
  horizontal_spiral_lt(led,100); 
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

//Initialisation colonne
void initialize_column(int led[3][4],int j,int r, int g, int b){
  int i;
  for (i=0;i<3;i++){
      strip.setPixelColor(led[i][j], Color(r,g,b));
  }
}

//Initialisation ligne
void initialize_lign(int led[3][4],int i,int r, int g, int b){
  int j;
  for (j=0;j<4;j++){
      strip.setPixelColor(led[i][j], Color(r,g,b));
  }
}
//Fonction générale à partir de la commande RasberryPi

          /*Models de Spirales*/
//Spriral horizontal bas en haut
void horizontal_spiral_lt(int led[3][4],uint16_t wait){
  if ( j>-1 && i>-1 && (millis() - d_temps) > wait ) {
    strip.setPixelColor(led[i][j], Color(7,222,46));
    j-=1;
    strip.show();
    d_temps = millis();
  }
  if ( j==-1 && i>-1 && (millis() - d_temps) > wait ) {
    strip.setPixelColor(led[i][j], Color(7,222,46));
    i-=1;
    j=3;
    strip.show();
    d_temps = millis();
  }
  if ( j==3 && i==-1 && (millis() - d_temps) > wait ) {
    int k,l;
    for (k=0;k<3;k++){
      for (l=0;l<4;l++){
        strip.setPixelColor(led[k][l], Color(0,0,0));
      }
    }
    strip.show();
    i=2;
    j=3;
    d_temps = millis();
  }
}
