#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <TimeLib.h>
bool gameMode = false;
bool laserState = false;

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();
String days[7] = {
  "Sunday",
  "Monday",
  "Tuesday",
  "Wednesday",
  "Thursday",
  "Friday",
  "Saturday"
};
uint8_t charSet[39][5] = {
    {2, 5, 7, 5, 5},//A
    {6, 5, 7, 5, 7},//B
    {7, 4, 4, 4, 7},//C
    {6, 5, 5, 5, 6},//D
    {7, 4, 7, 4, 7},//E
    {7, 4, 6, 4, 4},//F
    {15, 8, 11, 9, 15},//G
    {5, 5, 7, 5, 5},//H
    {7, 2, 2, 2, 7},//I
    {7, 2, 2, 2, 6},//J
    {5, 5, 6, 5, 5},//K
    {4, 4, 4, 4, 7},//L
    {17, 27, 21, 17, 17},//M
    {17, 25, 21, 19, 17},//N
    {7, 5, 5, 5, 7},//O
    {7, 5, 7, 4, 4},//P
    {30, 18, 18, 22, 31},//Q
    {7, 5, 7, 6, 5},//R
    {7, 4, 7, 1, 7},//S
    {7, 2, 2, 2, 2},//T
    {5, 5, 5, 5, 7},//U
    {5, 5, 5, 5, 2},//V
    {17, 17, 21, 27, 17},//W
    {5, 2, 2, 2, 5},//X
    {5, 5, 2, 2, 2},//Y
    {7, 1, 2, 4, 7},//Z
    {7, 5, 5, 5, 7},//O
    {2, 6, 2, 2, 7},//1
    {7, 1, 7, 4, 7},//2
    {7, 1, 3, 1, 7},//3
    {8, 10, 15, 2, 2},//4
    {7, 4, 7, 1, 7},//5
    {7, 4, 7, 5, 7},//6
    {14, 2, 7, 2, 2},//7
    {7, 5, 7, 5, 7},//8
    {7, 5, 7, 1, 1},//9
    {0, 2, 0, 2, 0},//:
    {0, 1, 2, 4, 0},///
    {0, 0, 0, 0, 0}//[whitespace]
};



int getWidthString(String inputStr){
  int x = 0;
  int inputInts[inputStr.length()];
  for (int i = 0; i < inputStr.length(); i++){
    if(((int)inputStr[i] >= 97) && ((int)inputStr[i] <= 122)){
      inputInts[i] = ((int)tolower(inputStr[i]))-97;
    }
    else if(((int)inputStr[i] >= 48) && ((int)inputStr[i] <= 57)){
      inputInts[i] = ((int)tolower(inputStr[i]))-22;     
    }
    else if((int)inputStr[i] == 58){
      inputInts[i] = 36;
    }
    else if((int)inputStr[i] == 47){
      inputInts[i] = 37;
    }
    else if((int)inputStr[i] == 32){
      inputInts[i] = 38;
    }
    else{
      inputInts[i] = 23;
    }
  
  }
    
  for(int j = 0; j < inputStr.length(); j++){
    int width = 3;
    int largest = 0;
    for(int i = 0; i < 5; i++){
      if(charSet[inputInts[j]][i] > largest){
        largest = charSet[inputInts[j]][i];  
      }
    }
    if(largest >= 8){
      if(largest < 16){
        width = 4;  
      }
      else{
        width = 5;  
      }
    }
    x += width + 1;
  }
  return x;
}

void writeString(String inputStr, int x, int y, bool color){
  int inputInts[inputStr.length()];
  for (int i = 0; i < inputStr.length(); i++){
    if(((int)tolower(inputStr[i])>= 97) && ((int)tolower(inputStr[i]) <= 122)){
      inputInts[i] = ((int)tolower(inputStr[i]))-97;
    }
    else if(((int)tolower(inputStr[i]) >= 48) && ((int)tolower(inputStr[i]) <= 57)){
      inputInts[i] = ((int)tolower(inputStr[i]))-22;     
    }
    else if((int)inputStr[i] == 58){
      inputInts[i] = 36;
    }
    else if((int)inputStr[i] == 47){
      inputInts[i] = 37;
    }
    else if((int)inputStr[i] == 32){
      inputInts[i] = 38;
    }
    else{
      inputInts[i] = 23;
    }
  
  }  
  
  for(int j = 0;j < inputStr.length(); j++){
    int width = 3;
    int largest = 0;
    for(int i = 0; i < 5; i++){
      if(charSet[inputInts[j]][i] > largest){
        largest = charSet[inputInts[j]][i];  
      }
    }
    if(largest >= 8){
      if(largest < 16){
        width = 4;  
      }
      else{
        width = 5;  
      }
    }
    matrix.drawBitmap(x-8+width, y, charSet[inputInts[j]], 8, 5, LED_ON);
    x += width + 1;
  }
}

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


void pong(){
  matrix.setRotation(1);
  class Paddle;
  class Ball{
    public:float velocity[2] = {1, -1};
    public:float position[2] = {4, 3};
    
    void movePosition(float enemyPosition[2], float playerPosition[2]){       
      if(this->position[0] >= 7){//screen protection
         this->position[0] = 7;
         this->velocity[0] = -this->velocity[0];
      }else if(this->position[0] <= 0){
         this->position[0] = 0;
         this->velocity[0] = -this->velocity[0];
      }
       
      if(this->position[1] > 8){//screen protection
         this->position[0] = random(1,6);
         this->velocity[1] = -this->velocity[1];
         this->position[1] = 3;
      }else if(this->position[1] < -1){
         this->position[0] = random(1,6);
         this->position[1] = 3;
      }

       
      if(this->position[1] == 1 && this->velocity[1] < 0){
        if(this->position[0] == enemyPosition[0] || this->position[0] == enemyPosition[0]+1){
           this->velocity[1] = -this->velocity[1];
        }
        else if(this->position[0] == enemyPosition[0]-1){
          if(this->velocity[0] > 0){
            this->velocity[1] = -this->velocity[1];
          }
        }
        else if(this->position[0] == enemyPosition[0]+2){
          if(this->velocity[0] < 0){
            this->velocity[1] = -this->velocity[1];
          }
        }
      }

      if(this->position[1] == 6 && this->velocity[1] > 0){
        if(this->position[0] == playerPosition[0] || this->position[0] == playerPosition[0]+1){
           this->velocity[1] = -this->velocity[1];
        }
        else if(this->position[0] == playerPosition[0]-1){
          if(this->velocity[0] > 0){
            this->velocity[1] = -this->velocity[1];
          }
        }
        else if(this->position[0] == playerPosition[0]+2){
          if(this->velocity[0] < 0){
            this->velocity[1] = -this->velocity[1];
          }
        }
      }

      
      this->position[0] += this->velocity[0];
      this->position[1] += this->velocity[1];
      
    }
    void draw(){
      matrix.drawPixel(this->position[0], this->position[1], 1);
    }
  };


 
  class Paddle{
    public:float position[2] = {3, 0};//leftmost of paddle ***
    
    void ai(Ball &ball){
      bool outBounds = false;
       if(ball.velocity[1] < 0){
        if(ball.position[0] < this->position[0]){
          this->position[0]--;
          outBounds = true;
        }
        else if(ball.position[0] > this->position[0]+1){
          this->position[0]++;
          outBounds = true;
        }
      }
      
   }
   void draw(){
      matrix.drawPixel(this->position[0], this->position[1], 1);
      matrix.drawPixel(this->position[0]+1, this->position[1], 1);
   }
  };
  
  
  Ball ball;
  Paddle enemy;
  Paddle player;
  player.position[1] = 7;
  
  while(true){
    buttonInput();
    matrix.clear();
    ball.movePosition(enemy.position, player.position);
    ball.draw();
    
    enemy.ai(ball);
    enemy.draw();
    
    if(btn2.pressed){
      if(player.position[0] > 0) player.position[0]--;
    }
    if(btn1.pressed){
      if(player.position[0] < 6) player.position[0]++;
    }
    player.draw();

    matrix.writeDisplay();
    for(int i = 0; i < 200; i++){
      delay(1);
      if(btn3.state()){
        if(btn3.clickOrHold()){
          gameMode = false;
          return;
        }
        else{
          return;  
        }
    }
    }
  }
}






void binaryTime(){
  matrix.clear();
  uint8_t screen[] =
  { month(),
    0,
    hour(),
    minute(),
    second() };

  matrix.drawBitmap(0, 3, screen, 8, 5, LED_ON);
  matrix.writeDisplay();
}



void displayDate(){
  String stringDate = String(day())+"/" + String(month());
  for (int8_t x = 7; x >= -getWidthString(stringDate); x--) {
    matrix.clear();
    matrix.setCursor(x,0);
    writeString(stringDate, x, 1, 1);
    matrix.writeDisplay();
    delay(70);
  }
}
void displayTime(){
  String stringTime = String(hour())+":";
  if(minute()<10){
    stringTime += "0";
  }
  stringTime += String(minute());
  
  for (int8_t x = -getWidthString(stringTime); x <= 7; x++) {
    matrix.clear();
    writeString(stringTime, x, 1, 1);
    matrix.writeDisplay();
    delay(70);
  }
}

void displayWeekday(){
  for (int x = 7; x >= -getWidthString(days[weekday()]); x--) {
    matrix.clear();
    
    writeString(days[weekday()], x, 1, 1);
    matrix.writeDisplay();
    delay(70);
  }
}





void setup() {
  matrix.begin(0x70);
  matrix.setBrightness(16);
  matrix.setRotation(1);
  matrix.setTextSize(1);
  matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);
  
  setTime(100000);
  
  btn1.init(12);
  btn2.init(9);
  btn3.init(11);


  pinMode(8, OUTPUT);
}



void loop(){
  buttonInput();
  
  if(btn3.state()){//determine click or hold
    if(btn3.clickOrHold()){
      gameMode = true;
    }
    else{
      displayTime();
    }
  }
  if(btn2.state()){
    if(btn2.clickOrHold()){
      laserState = !laserState;
      digitalWrite(8, laserState);
    }
    else{
      displayDate();  
    }
  }
  if(btn1.state()){
    displayWeekday();  
  }

  while(gameMode){
    pong();
  }
  
  matrix.clear();
  matrix.writeDisplay();
}
