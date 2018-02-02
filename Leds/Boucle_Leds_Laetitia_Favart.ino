/* Commentaires du programme
Carte utilisée : Arduino Mega ADK*/

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
  /*Communication avec la Raspberry Pi*/
 /* if (Serial.available())  {
    readValue = Serial.read();
    Serial.println(readValue);
    Serial.println(readValue - '0');
    light(readValue - '0');
    }*/
    
  /*Appel individuel des fonctions à effectuer*/
  //cold_rainbow(450);
  //vertical_bar(led,200);
  //horizontal_bar(led,200);
  //horizontal_spiral_lt(led,200);
  //horizontal_spiral_tl(led,200);
  //shift_spiral_lt(50);
  shift_spiral_tl(50);
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

//Fonction affichage
void display(uint16_t wait){
  strip.show();
  delay(wait);
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


/*Déclaration des fonctions pour les comportements*/
//Décradé entre 2 couleurs froides
void cold_rainbow(uint16_t wait) {
  int i, j, r, g, b;
  r = 0;
  g = 150;
  b = 255;
  while (r < 155){
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Color(r,g,b));
      r=r+1; 
    }
    display(wait);
  }
  if(r = 154){
        while(g>31){
          for (i=0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, Color(r,g,b));
            g-=1;
          }
          display(wait);
        }
  }
  else{
       while(b>249){
            strip.setPixelColor(i, Color(r,g,b));
            b-=1;
        }
        display(wait);
      }
}

/*Effets de barres*/
//Tac Tic Tok Tek
void vertical_bar(int led[3][4],uint16_t wait){
  int i,j;
  for (j=0;j<4;j++){
    switch(j){
      case 0 :
        initialize_column(led,0,255,0,0);
        display(wait);
        initialize_column(led,0,0,0,0);
        display(wait);
        break;
      case 1 :
        initialize_column(led,1,0,255,0);
        display(wait);
        initialize_column(led,1,0,0,0);
        display(wait);
        break;
      case 2 :
        initialize_column(led,2,0,0,255);
        display(wait);
        initialize_column(led,2,0,0,0);
        display(wait);
        break;
      case 3 :
        initialize_column(led,3,255,255,0);
        display(wait);
        initialize_column(led,3,0,0,0);
        display(wait);
        break;
    } 
  }
  display(wait);
}

//Pif paf pouf
void horizontal_bar(int led[3][4],uint16_t wait){
  int i,j;
  for (i=0;i<3;i++){
    switch(i){
      case 0 :
        initialize_lign(led,0,255,0,0);
        display(wait);
        initialize_lign(led,0,0,0,0);
        display(wait);
        break;
      case 1 :
        initialize_lign(led,1,0,255,0);
        display(wait);
        initialize_lign(led,1,0,0,0);
        display(wait);
        break;
      case 2 :
        initialize_lign(led,2,0,0,255);
        display(wait);
        initialize_lign(led,2,0,0,0);
        display(wait);
        break;
    } 
  }
  display(wait);
}

/*Modèle de Spirales*/
//Spriral horizontal bas en haut
void horizontal_spiral_lt(int led[3][4],uint16_t wait){
  int h,i,j;
  for (h=0;h<strip.numPixels();h++){
    strip.setPixelColor(h, Color(0,0,0));
  }
  display(wait);
  for (i=0;i<3;i++){
    for (j=0;j<4;j++){
      strip.setPixelColor(led[i][j], Color(7,222,46));
      display(wait);
    }
  }
}

//Spriral horizontal haut en bas
void horizontal_spiral_tl(int led[3][4],uint16_t wait){
  int h,i,j;
  for (h=0;h<strip.numPixels();h++){
    strip.setPixelColor(h, Color(0,0,0));
  }
  display(wait);
  for (i=3;i>0;i--){
    for (j=4;j>0;j--){
      strip.setPixelColor(led[i-1][j-1], Color(7,222,46));
      display(wait);
    }
  }
}

//Spiral décalage bas en haut
void shift_spiral_lt(uint16_t wait){
  int h,i,j;
  int tab[4][3]={{0,4,8},{5,7,9},{6,10,2},{11,1,3}};
  for (h=0;h<strip.numPixels();h++){
    strip.setPixelColor(h, Color(0,0,0));
  }
  display(wait);
  for (i=0;i<4;i++){
    for(j=0;j<3;j++){
      strip.setPixelColor(tab[i][j], Color(7,222,46));
      display(wait);
    }
  }
}

//Spiral décalage haut en bas
void shift_spiral_tl(uint16_t wait){
  int h,i,j;
  int tab[4][3]={{9,7,5},{8,4,0},{3,1,11},{2,10,6}};
  for (h=0;h<strip.numPixels();h++){
    strip.setPixelColor(h, Color(0,0,0));
  }
  display(wait);
  for (i=0;i<4;i++){
    for(j=0;j<3;j++){
      strip.setPixelColor(tab[i][j], Color(7,222,46));
      display(wait);
    }
  }
}



