#include <OneWire.h>
#define reed A2
#include <dht.h>
#define dht_dpin A1

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

dht DHT;

//temperatura
OneWire  tempPin(5);

//velocidade
int reedVal;
long tempo;// tempo de uma volta completa em ms
float kph;
float radio = 25;
float circunferencia;
int maxReedContador = 100;//tempo minimo de uma rotação
int reedContador;

//tela
const int BD = 3;
const int BE = 2; 
int BD_st = 0; 
int BE_st = 0;
int tela = 0;

void setup(){  
  pinMode(BD, INPUT);
  pinMode(BE, INPUT);  
  Serial.begin(9600);

  tela1();

  //VELOCIDADE 

    reedContador = maxReedContador;
  circunferencia = 2*3.14*radio;
  pinMode(reed, INPUT);
  
  // TIMER SETUP- the timer interrupt allows precise timed measurements of the reed switch
  //for more info about configuration of arduino timers see http://arduino.cc/playground/Code/Timer1
  cli();

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 1999;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS11);   
  TIMSK1 |= (1 << OCIE1A);
  sei();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Serial.begin(9600);
}


ISR(TIMER1_COMPA_vect) {
  reedVal = digitalRead(reed);
  if (reedVal){
    if (reedContador == 0){
      kph = (56.8*float(circunferencia))/float(tempo);
      tempo = 0;
      reedContador = maxReedContador;
    }
    else{
      if (reedContador > 0){
        reedContador -= 1;
      }
    }
  }
  else{
    if (reedContador > 0){
      reedContador -= 1;
    }
  }
  if (tempo > 2000){
    kph = 0;
  }
  else{
    tempo += 1;
  } 
}

void loop() {
  BD_st = digitalRead(BD);
  BE_st = digitalRead(BE);
  if (BD_st == HIGH){
     switch(tela){
       case 1:
         tela2();
         delay(500);
         break;
       case 2:
         tela3();
         delay(500);
         break;
       case 3:
         tela1();
         delay(500);
         break;
     }
  }
   if (BE_st == HIGH){
     switch(tela){
       case 1:
         tela3();
         delay(500);
         break;
       case 2:
         tela1();
         delay(500);
         break;
       case 3:
         tela2();
         delay(500);
         break;
     }
  }
  switch(tela){
      case 1:
         tela1();
         break;
      case 2:
         tela2();
         break;
      case 3:
         tela3();
         break;
  }
}

void tela1(){

  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
  
  if ( !tempPin.search(addr)) {
    tempPin.reset_search();
    delay(250);
    return;
  }
  
  for( i = 0; i < 8; i++) {
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
      return;
  }
 
  switch (addr[0]) {
    case 0x10:
      type_s = 1;
      break;
    case 0x28:
      type_s = 0;
      break;
    case 0x22:
      type_s = 0;
      break;
    default:
      return;
  } 

  tempPin.reset();
  tempPin.select(addr);
  tempPin.write(0x44, 1);
  
  delay(1000);
    
  present = tempPin.reset();
  tempPin.select(addr);    
  tempPin.write(0xBE);

  for ( i = 0; i < 9; i++) {
    data[i] = tempPin.read();
  }
.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; 
    if (data[7] == 0x10) {
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw & ~7;
    else if (cfg == 0x20) raw = raw & ~3;
    else if (cfg == 0x40) raw = raw & ~1;
  }
  celsius = (float)raw / 16.0;

  Serial.print(celsius);
  Serial.println(" Celsius");  

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(33,0);
  display.println("TEMP.");
  display.setCursor(23,35);
  display.print(celsius);
  display.println(" C"); 
  display.display();
  
  tela = 1;
}
void tela2(){
  DHT.read11(dht_dpin);
  Serial.print("Umidade = ");
  Serial.print(DHT.humidity);
  Serial.println(" %  ");   

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(20,0);
  display.println("UMIDADE");
  display.setCursor(27,35);
  display.print(DHT.humidity);
  display.println("%"); 
  display.display();
  delay(1000);
  
  tela = 2;
}
void tela3(){
  Serial.print(kph * 1.609344);
  Serial.println(" KM/H");
  delay(1000);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(3,0);
  display.println("VELOCIDADE");
  display.setCursor(3,35);
  display.print(kph * 1.609344);
  display.println(" KM/H"); 
  display.display();
  delay(1000);
  
  tela = 3; 
}

