#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <TimeLib.h>


int programCounter = 0;

void incrementProgramCounter(){
  programCounter++;
  if(programCounter > 4) programCounter = 0;
}


class Button{
  public:int pin;
  public:bool history[2] = {0, 0};
  void updateHistory(){
    this->history[0] = this->history[1];
    this->history[1] = !digitalRead(this->pin);
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
  matrix.setRotation(1);//top left is (0,0)
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
  
  for (int8_t x = 2; x >= -23; x--) {
    buttonInput();
    if(btn3.state()){
      incrementProgramCounter();
      return;
    }
    
    matrix.clear();
    matrix.setCursor(x,0);
    
    String prettyTime = getPrettyTime();
    matrix.print(prettyTime);
    matrix.writeDisplay();
    
    
    
    delay(70);
  }
  for (int8_t x= -23; x <= 2; x++) {
    buttonInput();
    if(btn3.state()){
      incrementProgramCounter();
      return;
    }
    matrix.clear();
    matrix.setCursor(x,0);
    
    String prettyTime = getPrettyTime();
    matrix.print(prettyTime);
    matrix.writeDisplay();
    

    delay(70);
  }
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

    
    if(btn2.history[1]) player.position[0]--;
    if(btn1.history[1]) player.position[0]++;
    if(btn3.state()){
      incrementProgramCounter();
      return;
    }
    player.draw();

    matrix.writeDisplay();
    for(int i = 0; i < 200; i++){
      delay(1);
      buttonInput();
      if(btn3.state()){
        incrementProgramCounter();
        return;
      }
    }
  }
}

void loop(){
  buttonInput();
  if(btn3.state()){
    incrementProgramCounter();
  }
  if(programCounter == 0) binaryTime();
  if(programCounter == 1) progressTime();
  if(programCounter == 2) displayedTime();
  if(programCounter == 3) pong();
  if(programCounter == 4){
    matrix.clear();
    matrix.writeDisplay();  
  };

}
