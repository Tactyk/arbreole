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
int etat=1;

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
  vertical_bar(led,200);  
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
          /*Effets de barres*/          
//Tac Tic Tok Tek
void vertical_bar(int led[3][4],uint16_t wait){
  int i,j;
  if( (millis() - d_temps) > wait && (etat==1) ) {
    initialize_column(led,0,255,0,0);
    strip.show();
    etat+=1;
  }
  if( (millis() - d_temps) > 2*wait && (etat==2) ) {
    initialize_column(led,0,0,0,0);
    initialize_column(led,1,0,255,0);
    strip.show();
    etat+=1;
  }
  if( (millis() - d_temps) > 3*wait && (etat==3) ) {
    initialize_column(led,1,0,0,0);
    initialize_column(led,2,0,0,255);
    strip.show();
    etat+=1;
  }
  if( (millis() - d_temps) > 4*wait && (etat==4) ) {
    initialize_column(led,2,0,0,0);
    initialize_column(led,3,255,255,0);
    strip.show();
    etat+=1;
  }
  if( (millis() - d_temps) > 5*wait && (etat==5) ) {
    initialize_column(led,3,0,0,0);
    strip.show();
    etat=1;
    d_temps = millis();
  }
}
