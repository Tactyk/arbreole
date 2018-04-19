/******************************DECLARATIONS ET IMPORTATIONS DE TOUS LES ELEMENTS******************************/
//Importation 
#include "Adafruit_WS2801.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
#include <avr/power.h>
#endif

//Declaration of LED strip
uint8_t dataPin  = 11;                                          // Yellow wire on Adafruit Pixels
uint8_t clockPin = 12;                                          // Green wire on Adafruit Pixels
Adafruit_WS2801 strip = Adafruit_WS2801(12, dataPin, clockPin); // Number of LEDs
byte r,g,b,R,G,B,behaviour,choose;                              // Declaration of colors
char * category;
uint32_t c_time,d_time_red, d_time_blue, d_time_green;
uint32_t d_time = millis();
byte led[3][4] = {{0,5,6,11},{1,4,7,10},{2,3,8,9}};
byte statement=0;

//Déclaration des variables pour la communication avec le serveur
const byte buffSize = 40;
char inputBuffer[buffSize];
const char startMarker = '<';
const char endMarker = '>';
byte bytesRecvd = 0;
boolean readInProgress = false;
boolean newDataFromPC = false;


//Définition de la fréquence de communication et mise en place des leds en mode "on"
void setup() {
  Serial.begin(9600);
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
    clock_prescale_set(clock_div_1); 
  #endif
  strip.begin();
  strip.show();
}

/******************************LOOP******************************/
void loop() {
  getDataFromPC();
  parseData();
}

/******************************BASIC FUNCTIONS******************************/
//Creation of color from RGB code
uint32_t Color(byte r, byte g, byte b){
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}
//Initialization column
void initialize_column(byte led[3][4],byte j,byte r, byte g, byte b){
  byte i;
  for (i=0;i<3;i++){
      strip.setPixelColor(led[i][j], Color(r,g,b));
  }
}
//Initialization lign
void initialize_lign(byte led[3][4],byte i,byte r, byte g, byte b){
  byte j;
  for (j=0;j<4;j++){
      strip.setPixelColor(led[i][j], Color(r,g,b));
  }
}

//Show and change statement
void change_statement(){
  strip.show();
  statement+=1;
}
/******************************COMMUNICATION WITH SERVER******************************/
// receive data from PC and save it into inputBuffer
void getDataFromPC() {
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

//Recover parameters
void parseData() {
  char * strtokIndx;                    // this is used by strtok() as an index
  strtokIndx = strtok(inputBuffer,","); // get the first part
  strcpy(category, strtokIndx);         // copy it to messageFromPC
  byte i;
  for(i=1;i<10;i++){
    strtokIndx = strtok(NULL, ",");
    switch(i){
      case 1 : behaviour = atoi(strtokIndx);
        break;
      case 2 : choose = atoi(strtokIndx);
        break;
      case 3 :r = atoi(strtokIndx);
        break;
      case 4 : g = atoi(strtokIndx);
        break;
      case 5 : b = atoi(strtokIndx);
        break;
      case 6 : R = atoi(strtokIndx);
        break;
      case 7 : G = atoi(strtokIndx);
        break;
      case 8 : B = atoi(strtokIndx);
        break;
      case 9 : c_time = atoi(strtokIndx);
        break;
      default : break;
    }
  }
}
//Choose behavior thanks to variable "category"
void choose_behavior(){
  if (strcmp(category, "V") == 0) {
    vertical_bar(led,choose,r,g,b,R,G,B,c_time);
  }
  else if(strcmp(category, "H")==0){
    //Mettre les horizontales bar
  }
  else if(strcmp(category, "R")==0){
    //Mettre les arc-en-ciel
  }
  else if(strcmp(category, "F")==0){
    //Mettre les firework
  }
  else if(strcmp(category, "E")==0){
    //Mettre les exponentielles
  }
  else if(strcmp(category, "P")==0){
    //Mettre le pulse
  }
  else if(strcmp(category, "SP")==0){
    //Mettre les codes spot
  }
  else if(strcmp(category, "H")==0){
    //Mettre les codes spirales horizontales
  }
  else if(strcmp(category, "SS")==0){
    //Mettre les horizontales bar
  }
  else{}
}

/******************************BEHAVIOURS******************************/
//vertical_bar and vertical_bar_2
void vertical_bar(byte led[3][4],byte choose,byte r,byte g, byte b,byte R,byte G,byte B,uint16_t c_time){
  uint16_t wait = c_time/5;
  byte i;
  if(choose==5){
    if( (millis() - d_time_red > c_time/abs(r-R)) && (r!=R)) {
      for (i=0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, Color(r,g,b));}
      if(r>=R) {
        r-=1;}
      else {
        r+=1;}
      }
     d_time_red = millis();
    }
  if( (millis() - d_time_green > c_time/abs(g-G)) && (g!=G) ) {
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Color(r,g,b));}
    if(g>=G) {
      g-=1;}
    else {
      g+=1;}
    d_time_green = millis();
  }
  if( (millis() - d_time_blue > c_time/abs(b-B)) && (b!=B) ) {
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Color(r,g,b));}
    if(b>=B) {
      b-=1;}
    else {
      b+=1;}
    d_time_blue = millis();
  }
  else{
    if(statement==0) {
      for(i=0;i<4;i++) {
          if(choose==3||choose==4){
            initialize_column(led,i,r,g,b);
          }
          else {
            initialize_column(led,i,0,0,0);
          }   
      }
      change_statement();
    }
  }
  if( (millis() - d_time) > wait && (statement==1) ) {
    initialize_column(led,0,R,G,B);
    change_statement();
  }
  if( (millis() - d_time) > 2*wait && (statement==2) ) {
    if (choose==1||choose==3){
      initialize_column(led,0,0,0,0);
    }
    else if(choose==4||choose==5){
      initialize_column(led,0,r,g,b);
    }
    initialize_column(led,1,R,G,B);
    change_statement();
  }
  if( (millis() - d_time) > 3*wait && (statement==3) ) {
    if (choose==1||choose==3||choose==5){
      initialize_column(led,1,0,0,0);
    }
    else if(choose==4){
      initialize_column(led,1,r,g,b);
    }
    initialize_column(led,2,R,G,B);
    change_statement();
  }
  if( (millis() - d_time) > 4*wait && (statement==4) ) {
    if(choose==1||choose==3||choose==5){
      initialize_column(led,2,0,0,0);
    }
    else if(choose==4){
      initialize_column(led,2,r,g,b);
    }
    initialize_column(led,3,R,G,B);
    change_statement();
  }
  if( (millis() - d_time) > 5*wait && (statement==5) ) {
    if (choose==4){
      initialize_column(led,3,r,g,b);
    }
    else{
      initialize_column(led,3,0,0,0);
    }
    strip.show();
    statement=0;
    d_time = millis();
  }
}



