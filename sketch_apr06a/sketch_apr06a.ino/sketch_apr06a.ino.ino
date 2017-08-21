#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <TimeLib.h>

class Button{
  public:int pin;
  public:bool history[2] = {0, 0};
  public:bool pressed = false;
  
  void updateHistory(){
    this->history[0] = this->history[1];
    this->history[1] = !digitalRead(this->pin);
    this->pressed = this->history[1];
  }  
  bool state(){
    if((this->history[1]) && (!this->history[0])) return true;
    else return false;
  }
  void init(int pinToSetTo){
    this->pin = pinToSetTo;
    pinMode(this->pin, INPUT);
    digitalWrite(this->pin, HIGH);
  }
  bool clickOrHold(){ //0 for click, 1 for hold
    int startTime = second();
    int duration = 0;
    while(this->pressed){
      duration = second()-startTime;
      if(duration > 1) break;
      this->updateHistory();
    }
    if(duration > 1){
      return true;
    }
    else{
      return false;
    }
  }
  
};

Button btn1;
Button btn2;
Button btn3;


void buttonInput(){
  btn1.updateHistory();
  btn2.updateHistory();
  btn3.updateHistory();
}

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();
void setup() {
  matrix.begin(0x70);
  matrix.setBrightness(16);
  setTime(1491678866);
  
  btn1.init(12);
  btn2.init(9);
  btn3.init(11);
}

void progressTime(){
  matrix.setRotation(3);//bottom left is (0,0)
  matrix.clear();
  for(int i = 0; i < second()%8; i++){
    matrix.drawPixel(i, 0, 1);  
  }
  for(int i = 0; i < floor(second()/8); i++){
    matrix.drawPixel(i, 1, 1);
  }
  for(int i = 0; i < minute()%8; i++){
    matrix.drawPixel(i, 3, 1);  
  }
  for(int i = 0; i < floor(minute()/8); i++){
    matrix.drawPixel(i, 4, 1);
  }
  
  for(int i = 0; i < hour()%8; i++){
    matrix.drawPixel(i, 6, 1);
  }
  for(int i = 0; i < floor(hour()/8); i++){
    matrix.drawPixel(i, 7, 1);
  }
  matrix.writeDisplay();
}

void binaryTime(){
  matrix.clear();
  matrix.setRotation(1);//so top left is (0,0)
  uint8_t screen[] =
  { month(),
    0,
    hour(),
    minute(),
    second() };

  matrix.drawBitmap(0, 3, screen, 8, 5, LED_ON);
  matrix.writeDisplay();
}

String getPrettyTime(){
  String prettyTime;
  if(hour()<10){
    prettyTime += "0";
  }
  prettyTime += String(hour())+":";
  if(minute()<10){
    prettyTime += "0";  
  }
  prettyTime += String(minute());

  return prettyTime;
}

void displayedTime(){
  matrix.setRotation(1);
  matrix.setTextSize(1);
  matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);
  for (int8_t x = 7; x >= -28; x--) {
    buttonInput();
    if(btn3.state()){
      return;
    }
    matrix.clear();
    
    String prettyTime = getPrettyTime();
    matrix.setCursor(x,0);
    matrix.print(prettyTime);
    matrix.writeDisplay();
    delay(60);
  }
}


void loop(){
  bool gameMode = false;
  buttonInput();
  
  if(btn3.state()){//determine click or hold
    if(btn3.clickOrHold()){
      gameMode = true;
    }
    else{
      displayedTime();
    }
  }

  while(gameMode){
    buttonInput();
    if(btn3.state()){
      if(btn3.clickOrHold()) gameMode = false;  
    }
    
    matrix.clear();
    matrix.drawPixel(0, 0, second()%2);
    matrix.writeDisplay();

    
  }

  
  
  matrix.clear();
  matrix.writeDisplay();
}
