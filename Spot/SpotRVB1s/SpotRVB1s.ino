
/*
 * auteur Iynthurisha
 * Code permetant d'allumer le spot en couleur unique au choix (R,V,B)
 * avec une fonction char couleur
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


//fonction
char couleur(char COULEUR){
  return COULEUR;
  }

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  char couleur_uni = couleur('V'); //Choisir [R;V;B]
  tempsd = millis();

  if (couleur_uni=='R'){
  redValue = 0; // it will show red light (the color values are inversed)
  greenValue = 255;
  blueValue = 255;
  }

  if (couleur_uni=='V'){
  redValue = 255; 
  greenValue = 0;
  blueValue = 255;
  }

  if (couleur_uni=='B'){
  redValue = 255; 
  greenValue = 255;
  blueValue = 0;
  }
}

void loop() {
  analogWrite(RED, redValue);
  analogWrite(GREEN, greenValue);
  analogWrite(BLUE, blueValue); 
  
  tempsf=millis();
  if (tempsf-tempsd >1000){
     //spot off
    redValue = 255; 
    greenValue = 255;
    blueValue = 255;

    analogWrite(RED, redValue);
    analogWrite(GREEN, greenValue);
    analogWrite(BLUE, blueValue);
  }
}
