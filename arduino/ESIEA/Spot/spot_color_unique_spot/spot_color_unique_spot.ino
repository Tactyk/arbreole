
// Define Pins
#define GREEN 12 // Câble violet
#define BLUE 11 // Câble gris
#define RED 10 // Câble blanc

// define variables
int r;
int g;
int b;
uint32_t d_temps = millis();

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  }



void loop() {
  color_unique_spot(5000,7,222,46);
}


void color_unique_spot(uint32_t wait,int r,int g,int b){
  
 if (wait - d_temps >0){ 

  d_temps = millis();
  analogWrite(RED, 255-r);
  analogWrite(GREEN, 255-g);
  analogWrite(BLUE, 255-b);
  }


  else{
  analogWrite(RED, 255);
  analogWrite(GREEN, 255);
  analogWrite(BLUE, 255);
}
}

