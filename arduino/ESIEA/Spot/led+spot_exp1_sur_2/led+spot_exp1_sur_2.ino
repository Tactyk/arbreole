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
uint32_t d_temps_rouge = millis();
uint32_t d_temps_vert = millis();
uint32_t d_temps_bleu = millis();
byte r=154;
byte g=32;
byte b=250;
float wait1[68]={2.8902777667310575, 0.3465735902799727, 0.20273255405408186, 0.14384103622589084, 0.11157177565710485, 0.091160778396977, 0.07707533991362947, 0.06676569631226137, 0.058891517828191464, 0.052680257828912946, 0.04765508990216283, 0.04350568849481462, 0.04002135383676819, 0.03705398607686128, 0.03449643574347583, 0.03226926056878554, 0.030312310908216844, 0.028579206919974176, 0.027033610635138317, 0.025646647193775074, 0.02439508208471608, 0.02326000781744675, 0.022225881285416804, 0.021279807209397816, 0.02041099726012785, 0.019610356576640342, 0.018870163991423716, 0.018183822085437562, 0.01754565990563517, 0.01695077583784066, 0.01639491141149474, 0.0158743491572908, 0.0153858293333764, 0.014926481574840444, 0.014493768436626198, 0.014085438483347978, 0.013699487094057439, 0.013334123541080878, 0.012987743201629876, 0.012658903992145198, 0.012346306295185627, 0.012048775789530453, 0.011765248705096987, 0.011494759112349762, 0.01123642792602908, 0.010989453359387724, 0.010753102610481236, 0.01052670459891658, 0.010309643601368101, 0.010101353658759749, 0.009901313648089882, 0.00970904292855046, 0.00952409748534766, 0.009346066506076056, 0.009174569334097882, 0.00900925275133968, 0.008849788549699866, 0.008695871355935303, 0.008547216679649416, 0.008403559158191243, 0.008264650975604582, 0.008130260435890158, 0.008000170673220452, 0.007874178484070349, 0.0077520932679826515, 0.007633736065393748, 0.007518938682270715};
float wait2[187]={2.3844331393294405, 0.3465735902799727, 0.2027325540540823, 0.1438410362258904, 0.11157177565710485, 0.09116077839697745, 0.07707533991362903, 0.06676569631226137, 0.05889151782819191, 0.052680257828912946, 0.047655089902162384, 0.043505688494815065, 0.04002135383676819, 0.037053986076860834, 0.03449643574347583, 0.03226926056878554, 0.030312310908217288, 0.028579206919974176, 0.027033610635138317, 0.025646647193775074, 0.02439508208471608, 0.023260007817446304, 0.022225881285416804, 0.02127980720939826, 0.020410997260127406, 0.019610356576640786, 0.018870163991423716, 0.018183822085436674, 0.01754565990563517, 0.01695077583784066, 0.016394911411495627, 0.015874349157289913, 0.015385829333377288, 0.014926481574840444, 0.014493768436626198, 0.014085438483347978, 0.013699487094057439, 0.013334123541080878, 0.012987743201629876, 0.012658903992145198, 0.012346306295185627, 0.012048775789530453, 0.011765248705096987, 0.011494759112348873, 0.011236427926029968, 0.010989453359387724, 0.010753102610481236, 0.01052670459891658, 0.010309643601368101, 0.01010135365875886, 0.009901313648089882, 0.009709042928551348, 0.009524097485346772, 0.009346066506076944, 0.009174569334097882, 0.009009252751338792, 0.008849788549700754, 0.008695871355934415, 0.008547216679650305, 0.008403559158190355, 0.00826465097560547, 0.008130260435890158, 0.008000170673220452, 0.00787417848406946, 0.0077520932679826515, 0.007633736065394636, 0.007518938682269827, 0.007407542892570618, 0.00729939971057636, 0.007194368726049838, 0.007092317495978051, 0.006993120987369927, 0.006896661066168264, 0.006802826027889175, 0.006711510166070411, 0.0066226133750104665, 0.006536040783675645, 0.00645170241795423, 0.006369512888714901, 0.006289391103430297, 0.006211259999278518, 0.0061350462959071095, 0.006060680266172014, 0.005988095523358439, 0.005917228823500764, 0.0058480198815962225, 0.0057804112005381825, 0.005714347911810691, 0.005649777626967278, 0.00558665029906269, 0.005524918093292364, 0.0054645352660953606, 0.005405458052107015, 0.0053476445583742205, 0.005291054665268469, 0.005235649933648112, 0.005181393517773358, 0.005128250083593855, 0.005076185732009186, 0.005025167926750562, 0.0049751654265843825, 0.0049261482215055, 0.004878087472683035, 0.004830955455868313, 0.00478472550807485, 0.004739371977271922, 0.00469487017491943, 0.004651196331156626, 0.004608327552462654, 0.004566241781636116, 0.004524917759958669, 0.004484334991380123, 0.004444473708622887, 0.0044053148410778675, 0.004366839984377258, 0.004329031371557157, 0.004291871845696349, 0.004255344833953956, 0.004219434322932258, 0.004184124835258096, 0.004149401407348208, 0.004115249568257262, 0.0040816553195801575, 0.00404860511631, 0.004016085848632223, 0.00398408482458823, 0.0039525897535570564, 0.003921588730512404, 0.003891070221027526, 0.0038610230469551254, 0.0038314363727849354, 0.003802299692609701, 0.003773602817691568, 0.0037453358645782586, 0.0037174892437592533, 0.0036900536488113644, 0.0036630200460363582, 0.0036363796645400015, 0.003610123986743652, 0.003584244739306186, 0.0035587338844322858, 0.0035335836115457653, 0.003508786329323499, 0.0034843346580464285, 0.0034602214222871908, 0.003436439643881073, 0.0034129825351998377, 0.0033898434926893373, 0.003367016090671804, 0.0033444940753986074, 0.0033222713593339392, 0.0033003420156765273, 0.0032787002730794157, 0.0032573405105962294, 0.0032362572528095157, 0.0032154451651447147, 0.0031948990493857465, 0.0031746138393291545, 0.003154584596632759, 0.0031348065067975384, 0.0031152748753182635, 0.0030959851239602543, 0.003076932787188902, 0.0030581135087182076, 0.0030395230381907723, 0.0030211572279812415, 0.0030030120301063334, 0.0029850834932521053, 0.0029673677599069137, 0.0029498610635938505, 0.002932559726199102, 0.0029154601553971204, 0.002898558842162835, 0.0028818523583753475, 0.0028653373544917926, 0.0028490105573188984, 0.0028328687678387254, 0.002816908859128553, 0.002801127774334944, 0.002785522524727746, 0.0027700901878073836, 0.00275482790548498, 0.002739732882312218, 0.0027248023837831425, 0.0027100337346688264, 0.0026954243174381887};
float wait3[243]={0.0020618585919311982, 0.00207039633301509, 0.0020790050743322297, 0.0020876857052405384, 0.0020964391300175578, 0.002105266268172201, 0.002114168054760057, 0.002123145440725338, 0.002132199393229506, 0.0021413308960003263, 0.0021505409496951344, 0.0021598305722578814, 0.0021692007992992757, 0.002178652684477811, 0.0021881872998994467, 0.002197805736519065, 0.0022075091045588024, 0.002217298533932599, 0.002227175174690288, 0.0022371401974607963, 0.002247194793919327, 0.0022573401772634227, 0.002267577582695246, 0.0022779082679305063, 0.0022883335137056093, 0.0022988546243150054, 0.002309472928147649, 0.0023201897782509917, 0.0023310065529056345, 0.002341924656213301, 0.0023529455187061288, 0.002364070597972834, 0.0023753013792990885, 0.0023866393763283256, 0.002398086131746524, 0.0024096432179749883, 0.002421312237893325, 0.0024330948255872897, 0.002444992647095745, 0.0024570074012144616, 0.002469140820291038, 0.00248139467106423, 0.0024937707555201527, 0.00250627091177158, 0.0025188970149789824, 0.0025316509782733476, 0.0025445347537358387, 0.0025575503333845617, 0.0025706997502101814, 0.002583985079221307, 0.002597408438551163, 0.0026109719905758055, 0.0026246779430723066, 0.0026385285504222367, 0.002652526114846232, 0.0026666729876811957, 0.002680971570693025, 0.0026954243174381887, 0.0027100337346697145, 0.0027248023837822544, 0.002739732882312218, 0.00275482790548498, 0.0027700901878073836, 0.002785522524727746, 0.002801127774334944, 0.002816908859128553, 0.0028328687678387254, 0.0028490105573188984, 0.0028653373544917926, 0.0028818523583753475, 0.002898558842162835, 0.0029154601553971204, 0.002932559726199102, 0.0029498610635938505, 0.0029673677599069137, 0.0029850834932521053, 0.0030030120301063334, 0.0030211572279812415, 0.0030395230381907723, 0.0030581135087182076, 0.003076932787188902, 0.0030959851239611424, 0.0031152748753173753, 0.0031348065067975384, 0.003154584596632759, 0.0031746138393291545, 0.0031948990493857465, 0.003215445165145603, 0.0032362572528086275, 0.0032573405105962294, 0.0032787002730794157, 0.0033003420156765273, 0.0033222713593339392, 0.0033444940753986074, 0.003367016090671804, 0.0033898434926893373, 0.0034129825351998377, 0.003436439643881073, 0.0034602214222871908, 0.0034843346580464285, 0.003508786329323499, 0.0035335836115457653, 0.0035587338844322858, 0.003584244739306186, 0.003610123986743652, 0.0036363796645400015, 0.0036630200460363582, 0.0036900536488113644, 0.0037174892437592533, 0.0037453358645782586, 0.003773602817691568, 0.003802299692609701, 0.0038314363727849354, 0.0038610230469551254, 0.003891070221027526, 0.003921588730512404, 0.0039525897535570564, 0.00398408482458823, 0.004016085848632223, 0.00404860511631, 0.0040816553195801575, 0.00411524956825815, 0.0041494014073473195, 0.004184124835258096, 0.004219434322932258, 0.004255344833954844, 0.004291871845695461, 0.004329031371557157, 0.004366839984377258, 0.0044053148410778675, 0.004444473708622887, 0.004484334991380123, 0.004524917759958669, 0.004566241781636116, 0.004608327552462654, 0.004651196331156626, 0.00469487017491943, 0.004739371977271922, 0.00478472550807485, 0.004830955455868313, 0.004878087472683035, 0.0049261482215055, 0.0049751654265843825, 0.005025167926750562, 0.005076185732009186, 0.005128250083593855, 0.005181393517773358, 0.005235649933648112, 0.005291054665268469, 0.0053476445583742205, 0.005405458052107015, 0.0054645352660953606, 0.005524918093292364, 0.00558665029906269, 0.005649777626967278, 0.005714347911810691, 0.0057804112005381825, 0.0058480198815962225, 0.005917228823500764, 0.005988095523358439, 0.006060680266172014, 0.0061350462959071095, 0.006211259999278518, 0.006289391103430297, 0.006369512888714901, 0.00645170241795423, 0.006536040783675645, 0.0066226133750104665, 0.006711510166070411, 0.006802826027889175, 0.006896661066168264, 0.006993120987369927, 0.007092317495978051, 0.007194368726049838, 0.00729939971057636, 0.007407542892570618, 0.007518938682269827, 0.007633736065394636, 0.0077520932679826515, 0.00787417848406946, 0.008000170673220452, 0.008130260435890158, 0.00826465097560547, 0.008403559158190355, 0.008547216679650305, 0.008695871355934415, 0.008849788549700754, 0.009009252751338792, 0.00917456933409877, 0.009346066506076056, 0.009524097485346772, 0.009709042928551348, 0.009901313648089882, 0.010101353658759749, 0.010309643601367213, 0.01052670459891658, 0.010753102610481236, 0.010989453359387724, 0.011236427926029968, 0.011494759112348873, 0.011765248705096987, 0.012048775789530453, 0.012346306295185627, 0.012658903992145198, 0.012987743201629876, 0.013334123541080878, 0.013699487094057439, 0.014085438483347978, 0.014493768436626198, 0.014926481574840444, 0.015385829333377288, 0.015874349157289913, 0.016394911411495627, 0.01695077583784066, 0.01754565990563517, 0.018183822085436674, 0.018870163991423716, 0.01961035657664123, 0.020410997260126962, 0.021279807209397816, 0.022225881285417692, 0.02326000781744586, 0.02439508208471608, 0.025646647193775074, 0.027033610635138317, 0.028579206919974176, 0.030312310908217732, 0.03226926056878554, 0.03449643574347583, 0.03705398607686039, 0.04002135383676819, 0.04350568849481462, 0.04765508990216283, 0.05268025782891339, 0.05889151782819191, 0.06676569631226137, 0.07707533991362858, 0.09116077839697745, 0.1115717756571053, 0.14384103622588995, 0.20273255405408186, 0.3465735902799727};
int i1 = 0;
int i2 = 0;
int i3 = 0;



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
}

//Définition du tableau des leds (disposition)
int leds[3][4] = { // tableau de char à 2 dimensions
  {2,3,8,9},
  {1,4,7,10},
  {0,5,6,11}
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
  rainbow_cold_to_hot_LED(wait1,wait2,wait3,i1,i2,i3); //rainbow_cold_to_hot(147,53,41)
  rainbow_cold_to_hot_SPOT(wait1,wait2,wait3,i1,i2,i3); //rainbow_cold_to_hot(147,53,41)
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

//Fonction générale à partir de la commande RasberryPi


/*Déclaration des fonctions*/
void rainbow_cold_to_hot_LED(float *wait1, float *wait2, float *wait3,int i1,int i2,int i3) {
  //r = 154;
  //g = 32;
  //b = 250;
  int i;
  if( (millis() - d_temps_rouge > (wait1[i1])*20) && (r < 223) ) {
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Color(r,g,b));
     }
     strip.show();
     r=r+1;
     d_temps_rouge = millis();
     i1=i1+1;
  }
  if( (millis() - d_temps_vert > (wait2[i2])*20) && (g < 220) ) {
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Color(r,g,b));
    }
    strip.show();
    g=g+1;
    d_temps_vert = millis();
    i2=i2+1;
  }
  if( (millis() - d_temps_bleu > (wait3[i3])*20) && (b > 10) ) {
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Color(r,g,b));
    }
    strip.show();
    b=b-1;
    d_temps_bleu = millis();
    i3=i3+1;
  }
  //strip.show();
  if( (r >= 222) && (g >= 219) && (b >= 10) ) {
    r = 154;
    g = 32;
    b = 250;
  }
}

//**************SPOT****************//
  void rainbow_cold_to_hot_SPOT(float *wait1, float *wait2, float *wait3,int i1,int i2,int i3) {
  //r = 154;
  //g = 32;
  //b = 250;
  int i;
  if( (millis() - d_temps_rouge > (wait1[i1])*20) && (r < 223) ) {
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Color(r,g,b));
     }
     analogWrite(RED, 255-r);
    analogWrite(GREEN, 255-g);
    analogWrite(BLUE, 255-b);
     r=r+1;
     d_temps_rouge = millis();
     i1=i1+1;
  }
  if( (millis() - d_temps_vert > (wait2[i2])*20) && (g < 220) ) {
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Color(r,g,b));
    }

        analogWrite(RED, 255-r);
    analogWrite(GREEN, 255-g);
    analogWrite(BLUE, 255-b);
    g=g+1;
    d_temps_vert = millis();
    i2=i2+1;
  }
  if( (millis() - d_temps_bleu > (wait3[i3])*20) && (b > 10) ) {
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Color(r,g,b));
    }

    analogWrite(RED, 255-r);
    analogWrite(GREEN, 255-g);
    analogWrite(BLUE, 255-b);
    b=b-1;
    d_temps_bleu = millis();
    i3=i3+1;
  }
 
  if( (r >= 222) && (g >= 219) && (b >= 10) ) {
    r = 154;
    g = 32;
    b = 250;
  }
}
