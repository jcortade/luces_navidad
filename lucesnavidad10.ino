#include "FastLED.h"
#include <arduino-timer.h>


// DEFINICIÓN E/S
// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 6

// PIN 0 Y 1 RESERVADOS PARA COMM. SERIE
#define LED_AZUL 2
#define LED_VERDE 3  
#define BOTON_AZUL 8
#define BOTON_VERDE 9
#define POTENCIOMETRO A0

// CONSTANTES
#define YELLOW 55
#define BLUE 240
#define RED 0
#define TOTAL_SECUENCIAS 7
#define TIEMPO_CAMBIO_AUTO 60000  // En millis

// How many leds in your strip?
#define NUM_LEDS 100


// VARIABLES DE PROGRAMA

// Define the array of leds
CRGB leds[NUM_LEDS];
int hue[NUM_LEDS];


bool boton_azul_pres, boton_verde_pres;
bool boton_azul, boton_verde, boton_azul_ant, boton_verde_ant;
bool bit_aux_azul, bit_aux_verde;
int val_pot, val_pot_esc;
int etapa, etapa_ant;
int i,j,k;
bool flag_inicio, flag_arcoiris, flag_nevada;
bool modo_auto,modo_manu,modo_off;
bool pulse_500ms,flag_500ms,flag_500ms_last;
bool flag_100ms,flag_100ms_last;

// Timers
Timer<1, millis, const char *> timer_nevada; // create a timer with 1 task and microsecond resolution
Timer<1, millis, const char *> timer_arcoiris; // create a timer with 1 task and microsecond resolution
Timer<10, millis, const char *> timer_varios;

void setup() {
  // put your setup code here, to run once:

  pinMode(LED_AZUL, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);

  pinMode(BOTON_AZUL, INPUT_PULLUP);
  pinMode(BOTON_VERDE, INPUT_PULLUP);

  Serial.begin(9600);           //  setup serial

  // Configura los LEDs
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);

  // Ajuste de tiempo efecto nevada en milis
  timer_nevada.every(100, set_flag_nevada);
  timer_arcoiris.every(50,set_flag_arcoiris);
  timer_varios.every(TIEMPO_CAMBIO_AUTO,incrementa_etapa);
  timer_varios.every(500,timer_500ms);
  timer_varios.every(100,timer_100ms);
  etapa=1;  
  modo_auto=true;
  modo_manu=false;
  modo_off=false;
  

}

// FUNCION LECTURA ENTRADAS DIGITALES
void lectura_ED(){


  boton_azul = digitalRead(BOTON_AZUL);  
  boton_verde = digitalRead(BOTON_VERDE);

  // Deteccion botón azul
  if (boton_azul != boton_azul_ant) {
    boton_azul_ant = boton_azul;
    if (boton_azul == LOW) {
      boton_azul_pres = true;
      Serial.println("boton_azul_pres");
    }
  }
  
  // Deteccion botón verde
  if (boton_verde != boton_verde_ant) {
    boton_verde_ant = boton_verde;
    if (boton_verde == LOW) {
      boton_verde_pres = true;
      Serial.println("boton_verde_pres");
    }
  }  
}

// Timer 500ms
void timer_500ms(){
  flag_500ms = true;
  if(pulse_500ms){
    pulse_500ms = false;
  }
  else{
    pulse_500ms = true;
  }
}

// Timer 100ms
void timer_100ms(){
  flag_100ms = true;
}

void incrementa_etapa(){

Serial.println(etapa);
  if (modo_auto){

      etapa=etapa+1;
      if (etapa>TOTAL_SECUENCIAS){
        etapa=1;  
     }
    }
  }
  


// FUNCIÓN APAGADO
void apagado(){
    for(uint16_t l=0; l<NUM_LEDS; l++)
    { 
      leds[l]=CRGB::Black;
    }

    FastLED.show();  
}

void set_flag_arcoiris(){
  flag_arcoiris = true;
}

void set_flag_nevada(){
  flag_nevada = true;
}

            
// FUNCIÓN ARCO IRIS
void arcoiris(bool init){
  
  if(init){

    for(uint16_t n=0; n<NUM_LEDS; n++){
      hue[n] = map(n,0,NUM_LEDS,0,255);
      leds[n] = CHSV( hue[n], 255, 255);
    }
    
  }
  else{
    if (flag_arcoiris){
      for(uint16_t n=0; n<NUM_LEDS; n++){ 
        if (hue[n] == 255){
          hue[n] = 0;  
        }
        else{
          hue[n] = hue[n] + 1;
        }
        leds[n]=CHSV( hue[n], 255, 255);
      }    
    }
  }

  FastLED.show();
  flag_arcoiris = false;
  
}

// ------------------------------------------------------------------------------
// FUNCIÓN SECUENCIA NEVADA
void nevada(bool inicio){

  CRGB aux;


    if(inicio){
    for(uint16_t l=0; l<NUM_LEDS; l++)
    { 
      leds[l]=CRGB::Black;
    }

    for(uint16_t l=0; l<20; l++)
    { 
      leds[NUM_LEDS-1-l]=CHSV( 255, 0, l*12);
    }


              
      
      FastLED.show(); 
    }
    else{
    
      if(flag_nevada){
        aux=leds[0];
        
        for(uint16_t l=1; l<NUM_LEDS; l++)
        { 
          leds[l-1]=leds[l];
        }
  
        leds[NUM_LEDS-1]=aux;

        FastLED.show();
        flag_nevada = false; 
      }

    }
  
     

}


// ------------------------------------------------------------------------------
// FUNCIÓN TEATRO
void teatro(bool inicio){

  CRGB aux;


    if(inicio){
    for(uint16_t l=0; l<NUM_LEDS; l++)
    { 
      leds[l]=CRGB::Red;
    }


    leds[0]=CRGB::Green;
    leds[20]=CRGB::Green;
    leds[40]=CRGB::Green;
    leds[60]=CRGB::Green;
    leds[80]=CRGB::Green;   
      
    FastLED.show(); 
    
    }
    else{
    
      if(flag_500ms & !flag_500ms_last){
        aux=leds[0];
        
        for(uint16_t l=1; l<NUM_LEDS; l++)
        { 
          leds[l-1]=leds[l];
        }
  
        leds[NUM_LEDS-1]=aux;

        FastLED.show();

      }
      flag_500ms_last = flag_500ms;
      flag_500ms = false;
    }
  
     

}


// ------------------------------------------------------------------------------
// FUNCIÓN AZUL/BLANCO
void teatro2(bool inicio){

  CRGB aux;

    if(inicio){
    for(uint16_t l=0; l<NUM_LEDS; l++)
    { 
      leds[l]=CRGB::DarkTurquoise;
    }

    leds[0]=CRGB::White;
    leds[20]=CRGB::White;
    leds[40]=CRGB::White;
    leds[60]=CRGB::White;
    leds[80]=CRGB::White;

    FastLED.show(); 
    }
    else{
    
      if(flag_500ms & !flag_500ms_last){
        aux=leds[0];
        
        for(uint16_t l=1; l<NUM_LEDS; l++)
        { 
          leds[l-1]=leds[l];
        }
  
        leds[NUM_LEDS-1]=aux;

        FastLED.show();

      }
      flag_500ms_last = flag_500ms;
      flag_500ms = false;
    }
  

}


// ------------------------------------------------------------------------------
// FUNCIÓN CAMBIO GRADUAL
void cambio(){

    
    Serial.println(j);
    if (j < 255){
      j = j+1;
    }
    else{
      j=0;
    }
    
    FastLED.showColor(CHSV(j, 255, 255));
    flag_100ms = false;
  

}

// ------------------------------------------------------------------------------
// FUNCIÓN ENCENDIDO ALEATORIO
void aleatorio(bool inicio){

  long randNumber,randColor;
    
  if(inicio){
    for(uint16_t l=0; l<NUM_LEDS; l++)
    { 
      leds[l]=CRGB::Black;
    }
    FastLED.show();
  }
  else{ 
    if(flag_100ms & !flag_100ms_last){   
      randNumber = random(NUM_LEDS);
      randColor = random(256);      
      leds[int(randNumber)]=CHSV(int(randColor), 255, 255);
    }
    FastLED.show();
  }

  flag_100ms_last = flag_100ms;
  flag_100ms = false;
    
  
}


// ------------------------------------------------------------------------------
// -------------------  MAIN ----------------------------------------------------
// ------------------------------------------------------------------------------

void loop() { 

  timer_varios.tick();
  timer_nevada.tick();

  // LECTURA ENTRADAS DIGITALES
  lectura_ED(); 



  if (boton_azul_pres){   
    if (modo_manu){ 
      etapa = etapa+1;
        if (etapa >TOTAL_SECUENCIAS){
          etapa = 1;        
      }    
    }
  } 

  if (boton_verde_pres){
    if(modo_auto){
      modo_auto=false;
      modo_off=true;
      etapa=0;
    }else if(modo_manu){
      modo_manu=false;
      modo_auto=true;     
    }else if(modo_off){
      modo_off=false;
      modo_manu=true;
      etapa=1;
    }

  } 

  if (modo_auto){
    digitalWrite(LED_VERDE,true); 
       
  }
  else if (modo_manu){
    digitalWrite(LED_VERDE,pulse_500ms);
    
  }
  else{
    digitalWrite(LED_VERDE,false);
  }


  //flag cambio etapa
  if (etapa!=etapa_ant) {
    flag_inicio = true;
    etapa_ant = etapa;   
  }
  else{
    flag_inicio = false;
  }

  digitalWrite(LED_AZUL,false);


 // SI SE AÑADEN MÁS ETAPAS ACTUALIZAR CONSTANTE
  switch (etapa) {
  
        case 0: // APAGADO
          apagado();  
          break;
  
        case 1: // ARCO IRIS
        
          timer_arcoiris.tick();
          arcoiris(flag_inicio); 
          break;

        case 2:
          timer_nevada.tick();
          nevada(flag_inicio);  
          break;

        case 3:
          teatro(flag_inicio);   
          break;

        case 4:
          if(flag_100ms){ 
            cambio();
          }
          break;

        case 5:
          teatro2(flag_inicio);
          break;
          
        case 6:
          aleatorio(flag_inicio);
          break;
          
        case 7:
          digitalWrite(LED_AZUL,true);
          val_pot = analogRead(POTENCIOMETRO);  // read the input pin
          val_pot_esc = map(val_pot, 0, 1023, 0, 255); // Escalado de analógica
          FastLED.showColor(CHSV(val_pot_esc, 255, 255));   
          break;   
        
        default:  
          // statements
          break;
  
      }


 
  
  boton_azul_pres = false;
  boton_verde_pres = false;


}
