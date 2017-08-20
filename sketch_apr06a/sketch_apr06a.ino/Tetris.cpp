
void reset2dArray(bool arr[8][8]){
  for(int i = 0; i < 8; i++)
    for(int j = 0; j < 8; j++)
      arr[i][j] = 0;
}


void tetris(){
  bool screen[8][8] = {{0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0}};

  bool static background[8][8] = {{0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0}};
 
  const static bool iBlock[2][3] = {{0,0,0},
                                    {1,1,1}};  
  const static bool jBlock[2][3] = {{1,0,0},
                                    {1,1,1}};
  const static bool lBlock[2][3] = {{0,0,1},
                                    {1,1,1}};
  const static bool oBlock[2][3] = {{0,1,1},
                                    {0,1,1}};
                                                 
  const static bool tBlock[2][3] = {{0,1,0},
                                    {1,1,1}};
  
  const static bool zBlock[2][3] = {{1,1,0},
                                    {0,1,1}};

  const static bool sBlock[2][3] = {{0,1,1},
                                    {1,1,0}};

  class Block{
    public:bool shape[2][3];
    public:int position[2];
    public:int skipCount = 0;
    
    Block(bool shape[2][3]){
      this->position[1] = 0;
      this->position[0] = random(3, 4);
      for(int x = 0; x < 3; x++){
        for(int y = 0; y < 2; y++){
          this->shape[y][x] = shape[y][x];
        }  
      }
    }
    void moveLeft(){
      if(this->position[0] > 0 || (this->position[0]>-1 && (!this->shape[0][0] && !this->shape[1][0]))){
        this->position[0]--;
      }
      
    }
    void moveRight(){
      if(this->position[0] < 5){
          this->position[0]++;
      }
    }
    void draw(bool surface[8][8], bool &screenFilled){
      bool spaceBelow = true;
      
      for(int x = 0; x < 3; x++){
        for(int y = 0; y < 2; y++){//write shape
          surface[this->position[1]+y][this->position[0]+x] = this->shape[y][x] || background[this->position[1]+y][this->position[0]+x];
        }  
      }

      for(int x = 0; x < 3; x++){//check space below
        if(background[this->position[1]+2][this->position[0]+x] && this->shape[1][x]){
          spaceBelow = false;
        }
        if(background[this->position[1]+1][this->position[0]+x] && this->shape[0][x]){
          spaceBelow = false;
          if(this->position[1] == -1){
            screenFilled = true;
            return;
          }
        }
      }


      if(this->position[1] < 6 && spaceBelow){
        if(this->skipCount > 4){
          this->position[1]+= 1;
          this->skipCount = 0;
        }else{
          skipCount++;
        }
      }else{
        int shape_num = random(0, 6);
        for(int x = 0; x < 3; x++){
          for(int y = 0; y < 2; y++){
            background[this->position[1]+y][this->position[0]+x] = this->shape[y][x] || background[this->position[1]+y][this->position[0]+x];
            if(shape_num == 0) this->shape[y][x] = iBlock[y][x];
            else if(shape_num == 1) this->shape[y][x] = jBlock[y][x];
            else if(shape_num == 2) this->shape[y][x] = lBlock[y][x];
            else if(shape_num == 3) this->shape[y][x] = oBlock[y][x];
            else if(shape_num == 4) this->shape[y][x] = sBlock[y][x];
            else if(shape_num == 5) this->shape[y][x] = tBlock[y][x];
            else this->shape[y][x] = zBlock[y][x];
          }  
        }
        
        this->position[1] = -1;
        this->position[0] = 2;
      }
    }
      
  };

  Block block(zBlock);
  while(true){
    buttonInput();
    for(int x = 0; x < 8; x++){
      for(int y = 0; y < 8; y++){
          screen[y][x] = background[y][x];
      }  
    }
    bool screenFilled = false;

    if(btn2.history[1]) block.moveLeft();
    if(btn1.history[1]) block.moveRight();

    
    block.draw(screen, screenFilled);
   
    matrix.clear();
    for(int x = 0; x < 8; x++){
      for(int y = 0; y < 8; y++){
          matrix.drawPixel(x, y, screen[y][x]);
      }  
    }
    if(screenFilled){
      matrix.blinkRate(1);  
      delay(4000);
      reset2dArray(background);
      reset2dArray(screen);
      matrix.blinkRate(0);
    }
    
    matrix.writeDisplay();
    for(int i = 0; i < 100; i++){//delay
      delay(1);
      buttonInput();
      if(btn3.state()){
        incrementProgramCounter();
        return;
      }
    }
  }
}
