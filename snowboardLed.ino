#include "FastLED.h"
#include <MPU6050.h>

#define LED_NUM 82
#define DATA_PIN 5
#define BRIGHTNESS 80

CRGB leds[LED_NUM];

static int red = 0;
static int green = 0;
static int blue = 255;

extern float gyrAngleY, gyrAngleX, gyrAngleZ;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  configSensor();
  calibrateAcc();
  calibrateGyro();

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, LED_NUM);   
  FastLED.setBrightness(BRIGHTNESS);

  currentTime  = millis();

}

void loop() {
  getAngle();
  
  if (gyrAngleX <= -70){
    sinelon();
  }

  else if (gyrAngleX >= 70){
    pacman();
  }
  
  else if (realAccX >= 0.50 ){
    runningMode();
    delay(1000);
  }
  
  else {
      /*Standard animation */
  //  snake();
  //  sunrise();
    bpm();
  }



  
//  EVERY_N_MILLISECONDS
  
}

void getAngle(){
  // set up time for integration
  prevTime = currentTime;
  currentTime = millis();
  elapsedTime = (currentTime - prevTime) / 1000; // time-step in s
  
  getRawData();
  
  calculateTemperature();  
  calculateGyro();
  calculateAcc(); 
  calculateAccAngle();
  calculateGyrAngle();

//  displayValues();
 
}

void tiltY(){
  if (gyrAngleY < 0){
    red = 0;
    green = map(gyrAngleY, 0, -20, 0,255);
    if (green >=255){
       green = 255;
    }else if(green <=0){
      green = 0;
    }
  
    blue = map(gyrAngleY, 0, -20, 255,0);
    if (blue >=255){
      blue = 255;
    }else if(blue <=0){
      blue = 0;
    }
  }else if(gyrAngleY > 0){
    red = map(gyrAngleY, 0, 15, 0,255);
    if (red >=255){
      red = 255;
    }else if(red <=0){
      red = 0;
    }
    blue = map(gyrAngleY, 0, 20, 255,0);
    if (blue >=255){
      blue = 255;
    }else if(blue <=0){
      blue = 0;
    }
  }
  for (int i = 0; i < LED_NUM; i++){
    leds[i] = CRGB( red, green, blue);
  }
  FastLED.show();
  
}

void pacman(){
  for (int i = 0; i <= LED_NUM; i++){
    leds[i] = CRGB(0xFF, 0x00,0xCC);
    leds[(i+1)] = CRGB( 0xFF, 0x95, 0x00);
    leds[(i+2)] = CRGB::Cyan;
    leds[(i+3)] = CRGB::Red;
    leds[(i+4)] = CRGB::Black;
    leds[(i+5)] = CRGB::Yellow;
    FastLED.show();
    delay(200);

    leds[i] = CHSV(255,10,100);
    FastLED.show();
  }
  

  for(int i = LED_NUM; i > 0;i--){
    leds[i] = CRGB::Yellow;
    leds[(i-1)] = CRGB::Black;

    leds[(i-2)] = CRGB::Blue;
    leds[(i-3)] = CRGB::Blue;
    leds[(i-4)] = CRGB::Blue;
    leds[(i-5)] = CRGB::Blue;
    FastLED.show();

    delay(100);

    leds[i] = CHSV(255,10,100);

    FastLED.show();
  }

  
}

void bpm()
{
  uint8_t gHue = 255; // rotating "base color" used by many of the patterns
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < LED_NUM; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
  FastLED.show();
}

void sinelon()
{
    FastLED.clear();
    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy( leds, LED_NUM, 2);
    int pos = beatsin16( 25, 0, LED_NUM-1 );
    leds[pos] += CHSV( 100, 255, 255);
  
    FastLED.show();
}

void runningMode(){

  while (gyrAngleY <= 70){
    getAngle();
    tiltY();

    /* if jaump */
    if (realAccZ >= 0.50){
      explosion();
    }
  }
}

void explosion(){
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, LED_NUM, 150, 7);
  FastLED.show();
  delay(2000);
}

void snake(){
  int snakeSize = 1;

  for(int i = 0; i < LED_NUM; i++){

    for(int k = 0; k < LED_NUM; k++){
      leds[k] =  CHSV(255,10,150);
    }
    FastLED.show();
    
    for(int j = 0; j < LED_NUM; j++){
      leds[j] =  CRGB::Green;
      leds[j-snakeSize] = CHSV(255,10,150);
      leds[LED_NUM - 1] = CRGB::Red;
      delay(15);
      FastLED.show();
    }
 
    snakeSize++;
  }
}
