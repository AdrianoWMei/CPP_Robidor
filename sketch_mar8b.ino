#include "robidor.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
// #define DEBUG

//bluetooth
SerialPIO mySerial(0, 1);
int state = 0;

//define IO pins (pi pico) //-->> Arduino pro mini
int PWR_RST[2] = {15, 2}; //pi pin 20
int PWR_CLK[2] = {16, 3}; //pi pin 21
int LED_A[2] = {5, 13}; //pi pin 7
int LED_B[2] = {20, 5}; //pi pin 26
int LED_C[2] = {7, 6}; //pi pin 10
int LED_D[2] = {6, 7}; //pi pin 9
int MUX_C1[2] = {9, 8}; //pi pin 14
int MUX_C2[2] = {10, 9}; //pi pin 12
int MUX_S1[2] = {8, 10}; //pi pin 11
int MUX_S2[2] = {19, 11}; //pi pin 25
int MUX_S3[2] = {18, 12}; //pi pin 24
int MUX_S4[2] = {17, 4}; //pi pin 22

//standard state of buttons is digital 0
int but1 = 14; //button 1
int but2 = 22; //button 2
int but3 = 11; //button 3
int but4 = 26; //button 4
int but5 = 27; //button 5
int but6 = 28; //button 6

//define arrays and reading indexes
int GP_Arr[9][9] = {0};   //define game piece location matrix
int GP_READ[9] = {0, 12, 1, 13, 2, 14, 3, 15, 4}; //reading index array
int VWall[9][9] = {0};    //define vertical wall placement matrix
int VW_READ[9][2] = {  {8,0},  {5,0},  {9,0},  {6,0},  {0,1},  {12,1},  {1,1},  {13,1},  {2,1} };//reading index array
int HWall[9][9] = {0};    //define horizontal wall placement matrix
int HW_READ[9] = {14, 3, 15, 4, 8, 5, 9, 6, 0}; //reading index array
int LED_Arr[9][18] = {0}; //define LED matrix

int currCycle = 0;  //define current power cycle number
int newTime = 0;
int debugTime = 0;

int testCycle = 1;
uint16_t portvalue = 0;

int boardtype = 0; //0 for pi pico, 1 for arduino mini

int count = 0; //

void setup() {
  // put your setup code here, to run once:
  pinMode(PWR_RST[boardtype], OUTPUT);
  pinMode(PWR_CLK[boardtype], OUTPUT);
  pinMode(LED_A[boardtype], OUTPUT);
  pinMode(LED_B[boardtype], OUTPUT);
  pinMode(LED_C[boardtype], OUTPUT);
  pinMode(LED_D[boardtype], OUTPUT);
  pinMode(MUX_C1[boardtype], OUTPUT);
  pinMode(MUX_C2[boardtype], OUTPUT);
  
  pinMode(MUX_S1[boardtype], INPUT);
  pinMode(MUX_S2[boardtype], INPUT);
  pinMode(MUX_S3[boardtype], INPUT);
  pinMode(MUX_S4[boardtype], INPUT);

  pinMode(but1, INPUT);
  pinMode(but2, INPUT);
  pinMode(but3, INPUT);
  pinMode(but4, INPUT);
  pinMode(but5, INPUT);
  pinMode(but6, INPUT);

  boardReset();
  mySerial.begin(9600);
  Serial.begin(38400);
  Wire.setSDA(12);
  Wire.setSCL(13);
  Wire.begin();
  lcd.init();
  lcd.backlight();
}

void loop() {
  robidor gamestate;
  while(!gamestate.isGameOver())
  {
    lightBoard();
    if(gamestate.playerOneTurn)
    {
      playerMove();
      while(gamestate.playerOneTurn)
      {
        boardRead();
        matchWallLED();
        lightBoard();
        if(digitalRead(but1) == 1)
        {
          delay(500);
          boardRead();
          #ifdef DEBUG
          Serial.println(gamestate.playerOneY);
          Serial.println(gamestate.playerOneX);
          #endif  
          for(int i = 0; i < 9 ; i ++)
          {
            for(int j = 0; j < 9 ; j ++)
            {
                if(GP_Arr[i][j] == 1){
                  #ifdef DEBUG
                  Serial.println(i);
                  Serial.println(j);
                  #endif
                  if(i - gamestate.playerOneY < -2 || i - gamestate.playerOneY > 2 || j - gamestate.playerOneX < -2 || j - gamestate.playerOneX > 2)
                  {
                    Serial.println("invalid move");
                  }
                  //alternatively can use a while loop with this condition if no button!
                  if(i - gamestate.playerOneY == 0 && j - gamestate.playerOneX == 0)
                  {
                    Serial.println("no move detected");
                  }
                  if(gamestate.isWallBetween(gamestate.playerOneX, gamestate.playerOneY, j, i))
                  {
                    Serial.println("wall exist in between");
                  }
                  //ensure its player one
                  else
                  {
                  gamestate.board[gamestate.playerOneY][gamestate.playerOneX].player = 0;
                  gamestate.board[i][j].player = GP_Arr[i][j];
                  gamestate.playerOneX = j;
                  gamestate.playerOneY = i;
                  gamestate.board[gamestate.playerTwoY][gamestate.playerTwoX].player = 2;
                  }
                }
            }
            for(int j = 0; j < 8; j ++)
            {
              if(HWall[i][j] == 1 && HWall[i][j+1] == 1 && gamestate.isWallValid(true, i, j))
              {
                gamestate.buildWall(true, j, i);
              }
              // else if(HWall[i][j] == 0 && HWall[i][j+1] == 0 && gamestate.wall[i][j] == 1 && gamestate.wall[i][j+1] == 1 && gamestate.wall[i][j+2] == 1)
              // {
              //   gamestate.removeWall(false, j, i);
              // }

              if(VWall[i][j] == 1 && VWall[i+1][j] == 1 && gamestate.isWallValid(false, i, j))
              {
                gamestate.buildWall(false, j, i);
              }
              // else if(VWall[i][j] == 0 && VWall[i+1][j] == 0 && gamestate.wall[i][j] == 1 && gamestate.wall[i+1][j] == 1 && gamestate.wall[i+2][j] == 1)
              //   gamestate.removeWall(false, j, i);
              // {
              // }
            }
          }
          printArr(gamestate);
          // gamestate.changeTurn();
          Serial.println("ending p1 turn");
        }
        lightBoard();
        if(digitalRead(but2) == 1)
        {
          delay(500);
          gamestate.changeTurn();
          Serial.println("ending p1 turn");
        }
        if(digitalRead(but3) == 1)
        {
          delay(500);
          boardRead();
          printArr(gamestate);
        }
        if(digitalRead(but6) == 1)
        {
          delay(500);
          gamestate.reset();
          break;
        }
      }
      }
    else
    {
    lightBoard();
    // AImove();
    bool wallBuilt = false;
    // playMove AImove = gamestate.evalBFS();
    // Serial.println(count)
    if(count % 3 == 0)
    {
      if(gamestate.playerOneX < 7 && gamestate.isWallValid(true, gamestate.playerOneX, gamestate.playerOneY-1) && gamestate.playerTwoWall > 5)
      // && !gamestate.board[gamestate.playerOneY][gamestate.playerOneX].wallDown && !gamestate.board[gamestate.playerOneY][gamestate.playerOneX].wallDown)
      {
        // Serial.println();
        lcdPlaceAIWall();
        printArr(gamestate);
        placeWallAI(gamestate.playerOneX, gamestate.playerOneY-1, gamestate.playerOneX + 1, gamestate.playerOneY-1);
        count += 1;
        gamestate.buildWall(true, gamestate.playerOneX, gamestate.playerOneY-1);
        wallBuilt = true;
      }
    }
    else
    {
      AImove();
      lightBoard();
      playMove AImove = gamestate.evalBFS();
      int dir = 0;
      //no x movement
      if(AImove.x - gamestate.playerTwoX == 0 && AImove.y - gamestate.playerTwoY == 1)
      {
        dir = 1;
      }
      else if(AImove.x - gamestate.playerTwoX == 0 && AImove.y - gamestate.playerTwoY == -1)
      {
        dir = 3;
      }
      else if(AImove.x - gamestate.playerTwoX == 1 && AImove.y - gamestate.playerTwoY == 0)
      {
        dir = 2;
      }
      else if(AImove.x - gamestate.playerTwoX == -1 && AImove.y - gamestate.playerTwoY == 0)
      {
        dir = 4;
      }

      Serial.println(dir);
      displayLocation(AImove.x,AImove.y, dir);
      gamestate.move(AImove.x,AImove.y);
      mySerial.write('0');
      switch(dir)
      {
        //forward
        case 1:
          state = 1;
          break;
        case 3:
          state = 3;
          break;
        case 2:
          state = 4;
          break;
        case 4:
          state = 2;
          break;
      }
      // state = 2;
      mySerial.write(state);
      delay(250);
      // state = 0;
      printArr(gamestate);
      while( gamestate.board[AImove.y][AImove.x].player != (GP_Arr[AImove.y][AImove.x] + 1))
      {
        boardRead();
        matchWallLED();
        lightBoard();
        // delay(500);
        // Serial.println("loop");
        if(digitalRead(but1) == 1)
        {
          boardRead();
        }
        if(digitalRead(but3) == 1)
        {
          Serial.println("manually ending p2 turn");
          break;
        }
        if(digitalRead(but6) == 1)
        {
          delay(500);
          gamestate.reset();
          break;
        }
        if(gamestate.board[AImove.y][AImove.x].player == (GP_Arr[AImove.y][AImove.x] + 1))
        {
          count+=1;
          printArr(gamestate);
          Serial.println("ending p2 turn");
          break;
        }
      }
      // if(AImove.x - gamestate.playerTwoX == 0 && AImove.y - gamestate.playerTwoY == 0)
    }
    #ifdef DEBUG
    //   Serial.println(AImove.x);
    //   Serial.println(AImove.y);
      Serial.println("player two coords");
      Serial.println(gamestate.playerTwoX);
      Serial.println(gamestate.playerTwoY);
    //   Serial.println(gamestate.board[AImove.y][AImove.x].player);
    //   Serial.println(gamestate.board[gamestate.playerTwoY][gamestate.playerTwoX].player);
    //   Serial.println((GP_Arr[AImove.y][AImove.x] + 1));
    #endif
    // boardRead();
    // while( gamestate.board[AImove.y][AImove.x].player != (GP_Arr[AImove.y][AImove.x] + 1))
    // {
    //   Serial.println("loop");
    //   if(wallBuilt)
    //   {
    //     break;
    //   }
    //   // Serial.println(counter);
    //   // delay(1000);
    //   //press button 1 to check if the robot moved to its right position
    //   //remove this function once robot can move itself
    //   // if(gamestate.board[AImove.y][AImove.x].player != (GP_Arr[AImove.y][AImove.x] + 1))
    //   // {
    //   //   delay(500);
    //   //   boardRead();
    //   // }
    //   boardRead();
      // if(gamestate.board[AImove.y][AImove.x].player == (GP_Arr[AImove.y][AImove.x] + 1))
      // {
      //   count+=1;
      //   printArr(gamestate);
      //   Serial.println("ending p2 turn");
      //   break;
      // }

    //   // if(digitalRead(but1) == 1)
    //   // {
    //   //   delay(500);
    //   //   // count += 1;
    //   //   boardRead();
    //   //   printArr(gamestate);
    //   //   Serial.println("board check");
    //   // }
    //   // if(digitalRead(but2) == 1)
    //   // {
    //   //   if(gamestate.playerOneX < 7 && gamestate.isWallValid(true, gamestate.playerOneX, gamestate.playerOneY-1))
    //   //   {
    //   //     // Serial.println();
    //   //     printArr(gamestate);
    //   //     placeWallAI(gamestate.playerOneX, gamestate.playerOneY-1, gamestate.playerOneX + 1, gamestate.playerOneY-1);
    //   //     gamestate.buildWall(true, gamestate.playerOneX, gamestate.playerOneY);
    //   //   }
    //   // }
    //   //manual break
      // if(digitalRead(but3) == 1)
      // {
      //   Serial.println("manually ending p2 turn");
      //   break;
      // }
    //   //automatically break if the robot has moved to its right position
    //   // if(gamestate.board[AImove.y][AImove.x].player == (GP_Arr[AImove.y][AImove.x] + 1))
    //   // {
    //   //   Serial.println("ending p2 turn");
    //   //   break;
    //   // }
    // }
    //check robot moved to the AImove x,y coords
    //change turns if robot did
    gamestate.changeTurn();
    }
  }
  // put your main code here, to run repeatedly:
  /*digitalWrite(MUX_C1, LOW);
  digitalWrite(MUX_C2, LOW);
  delay(250);
  digitalWrite(MUX_C1, LOW);
  digitalWrite(MUX_C2, HIGH);
  delay(250);
  digitalWrite(MUX_C1, HIGH);
  digitalWrite(MUX_C2, HIGH);
  delay(250);
  digitalWrite(MUX_C1, HIGH);
  digitalWrite(MUX_C2, LOW);
  delay(250);*/

  // debugTime = millis() + 5000;
  // while(millis()<debugTime){
  //   boardRead();
  //   matchWallLED();
  //   newTime = millis()+1000;
  //   while(millis()<newTime){
  //     lightBoard();
  //   }
  // }
  // placeWallAI(2, 7, 3, 7);
  
  // debugTime = millis() + 5000;
  // while(millis()<debugTime){
  //   boardRead();
  //   matchWallLED();
  //   newTime = millis()+1000;
  //   while(millis()<newTime){
  //     lightBoard();
  //   }
  // }
  
  // placeWallAI(5, 4, 6, 4);
  // debugTime = millis() + 5000;
  // while(millis()<debugTime){
  //   boardRead();
  //   matchWallLED();
  //   newTime = millis()+1000;
  //   while(millis()<newTime){
  //     lightBoard();
  //   }
  // }
  // placeWallAI(6, 7, 7, 7);
  // delay(1000);
  // while(digitalRead(but1) != 1){
  //   boardRead();
  //   matchWallLED();
  //   newTime = millis()+1000;
  //   while(millis()<newTime){
  //     lightBoard();
  //   }
  // }
  
    //allLED();
/*  digitalWrite(MUX_C1, HIGH);
  digitalWrite(MUX_C2, HIGH);
  digitalWrite(PWR_RST, HIGH);
  digitalWrite(PWR_CLK, HIGH);
  gpio_set_dir_out_masked(0b11111000111110111111111111111111);
  gpio_put_all(0b00000000000001000000000000000000);
  delay(1000);*/
  // printArr(); //print to see changes in board
  //cycleLED();
  //debugCycleRead();
}
//print array
void printArr(robidor Gamestate){
  Serial.println();
  Serial.print("GP_Arr");
  Serial.print("\t");
  Serial.print("Vwall");
  Serial.print("\t");
  Serial.print("Hwall");
  Serial.print("\t");
  Serial.println("Board Player");
  Serial.print(123456789);
  Serial.print("\t");
  Serial.print(123456789);
  Serial.print("\t");
  Serial.print(123456789);
  Serial.print("\t");
  Serial.println(123456789);
  for(int i = 0; i<9; i++){
    for(int j = 0; j<9; j++){
      Serial.print(GP_Arr[i][j]);
    }
    Serial.print("\t");
    for(int j = 0; j<9; j++){
      Serial.print(VWall[i][j]);
    }
    Serial.print("\t");
    for(int j = 0; j<9; j++){
      Serial.print(HWall[i][j]);
    }
    Serial.print("\t");
    for(int j = 0; j<9; j++){
      Serial.print(Gamestate.board[i][j].player);
    }
    Serial.print("\t");
    for(int j = 0; j<9; j++){
      Serial.print(Gamestate.board[i][j].wallRight);
    }
    Serial.print("\t");
    for(int j = 0; j<9; j++){
      Serial.print(Gamestate.board[i][j].wallLeft);
    }
    Serial.print("\t");
    for(int j = 0; j<9; j++){
      Serial.print(Gamestate.board[i][j].wallDown);
    }
    Serial.print("\t");
    for(int j = 0; j<9; j++){
      Serial.print(Gamestate.board[i][j].wallUp);
    }
    Serial.println();
  }

//   for(int i = 0 ; i < 10; i ++)
//   {
//     for(int j = 0; j < 10; j ++)
//     {
//       Serial.print(Gamestate.wall[i][j]);
//     }
//     Serial.println();
//   }
}

//reset the board
void boardReset(){
  GP_Arr[9][9] = {0};
  VWall[9][8] = {0};
  HWall[8][9] = {0};
  LED_Arr[9][18] = {0};
  
  //reset cycle c ounter
  digitalWrite(LED_A[boardtype], LOW);
  digitalWrite(LED_B[boardtype], LOW);
  digitalWrite(LED_C[boardtype], LOW);
  digitalWrite(LED_D[boardtype], LOW);  
  currCycle = 1;
  for(int i = 1; i<=36; i++){
    nextCycle();
  }
}

void boardRead(){ //read board switches and store in the 3 matrices and LED
  //wait for cycle 1
  while(currCycle != 1){
    nextCycle();
  }
  int currRead = currCycle; //save the current cycle number
  //read a full board, cycle 18 cycles
  for(int c = 0; c <= 9; c++){
    //read 1 full column, two power cycles
    int cycleRead[16][2]; //create temporary storage
    for(int i=0; i<2; i++){
      //read 4 bits at a time
      for(int j=0; j<4; j++){
        switch(j){
          //choose mux controls
          case 0:
            digitalWrite(MUX_C1[boardtype], LOW);
            digitalWrite(MUX_C2[boardtype], LOW);
            delayMicroseconds(100);
            break;
          case 1:
            digitalWrite(MUX_C1[boardtype], LOW);
            digitalWrite(MUX_C2[boardtype], HIGH);
            delayMicroseconds(100);
            break;
          case 2:
            digitalWrite(MUX_C1[boardtype], HIGH);
            digitalWrite(MUX_C2[boardtype], HIGH);
            delayMicroseconds(100);
            break;
          case 3:
            digitalWrite(MUX_C1[boardtype], HIGH);
            digitalWrite(MUX_C2[boardtype], LOW);
            delayMicroseconds(100);
            break;
        }
        //read 4 bits
        cycleRead[4*j+0][i] = digitalRead(MUX_S1[boardtype]);
        cycleRead[4*j+1][i] = digitalRead(MUX_S2[boardtype]);
        cycleRead[4*j+2][i] = digitalRead(MUX_S3[boardtype]);
        cycleRead[4*j+3][i] = digitalRead(MUX_S4[boardtype]);
      }
      /*for(int k = 0; k<16; k++){
        Serial.print(cycleRead[k][i]);
      }
      Serial.println();*/
      currRead = currCycle;
      nextCycle();
    }
    //input reading to arrays
    int savePlace = (currRead-1)/2;
    //input to game piece array
    for(int r=0; r<9; r++){
      GP_Arr[r][savePlace] = cycleRead[GP_READ[r]][0];
    }
    //input to vertical walls array
    if(currRead != 1){
      for(int v=0; v<9; v++){
        VWall[v][savePlace] = cycleRead[VW_READ[v][0]][VW_READ[v][1]];
      }
    }
    else{
      for(int v=0; v<9; v++){
        VWall[v][0] = 0;
      }
    }
    //input to horizontal walls array
    for(int h=0; h<8; h++){
      HWall[h][savePlace] = cycleRead[HW_READ[h]][1];
    }
  }
}

/*
//only for debugging
void debugCycleRead(){
  LED_SEL(0);
  while(currCycle != 1){
    nextCycle();
  }
  while(currCycle != testCycle){
    nextCycle();
  }
  LED_SEL(1);
  if(digitalRead(but3) == 1){
    nextCycle();
    nextCycle();
    testCycle = currCycle;
  }
  delay(500);
  
  Serial.println("0123456789012345");
  int cycleRead[16][2]; //create temporary storage
  for(int c = 0; c <= 9; c++){
    //read 1 full column, two power cycles
    int cycleRead[16][2]; //create temporary storage
    for(int i=0; i<2; i++){
      //read 4 bits at a time
      for(int j=0; j<4; j++){
        switch(j){
          //choose mux controls
          case 0:
            digitalWrite(MUX_C1[boardtype], LOW);
            digitalWrite(MUX_C2[boardtype], LOW);
            delayMicroseconds(100);
            break;
          case 1:
            digitalWrite(MUX_C1[boardtype], LOW);
            digitalWrite(MUX_C2[boardtype], HIGH);
            delayMicroseconds(100);
            break;
          case 2:
            digitalWrite(MUX_C1[boardtype], HIGH);
            digitalWrite(MUX_C2[boardtype], HIGH);
            delayMicroseconds(100);
            break;
          case 3:
            digitalWrite(MUX_C1[boardtype], HIGH);
            digitalWrite(MUX_C2[boardtype], LOW);
            delayMicroseconds(100);
            break;
        }
        //read 4 bits
        cycleRead[4*j+0][i] = digitalRead(MUX_S1[boardtype]);
        cycleRead[4*j+1][i] = digitalRead(MUX_S2[boardtype]);
        cycleRead[4*j+2][i] = digitalRead(MUX_S3[boardtype]);
        cycleRead[4*j+3][i] = digitalRead(MUX_S4[boardtype]);
      }
      for(int k = 0; k<16; k++){
        Serial.print(cycleRead[k][i]);
      }
      Serial.println();
      nextCycle();
    }
    Serial.println();
  }
}
*/
void getPieceLocations(){
//  return GP_Arr;
}

void getHWall(){ //return horizontaal wall positions
//  return HWall;
}

void getVWall(){ //return vertical wall positions
//  return VWall;
}

void lightBoard(){ //light up the board based on the current LED Array
  //wait for cycle 1
  LED_SEL(0);
  while(currCycle != 1){
    nextCycle();
  }
  for(int i=0; i<18; i++){
    for(int j=0; j<9; j++){
      if(LED_Arr[j][i] == 1){
        LED_SEL(j+1);
        delayMicroseconds(100);
      }
      else{
        LED_SEL(0);
      }
    }
    LED_SEL(0);
    nextCycle();
  }
}

void matchWallLED(){ //transfer wall arrays into LED array
  for(int w=0; w<18; w++){
    if(w%2 == 0){
      for(int t=0; t<9; t++){
        LED_Arr[t][w] = VWall[t][w/2];
      }
    }
    if(w%2 == 1){
      for(int t=0; t<9; t++){
        LED_Arr[t][w] = HWall[t][w/2];
      }
    }
  }
}

void placeWallAI(int x1, int y1, int x2, int y2){//inputs for wall placement from AI? cell coordinates and direction placed in respect
  //add new temporary VWall or HWall depending on whether inputs are determined to be horizontal or vertical
  bool VH_SEL = false; //false = new HWall is used, true = new VWall is used
  int tempWall[9][9] = {0};
  
  //add wall to the correct array, VWall or HWall
  if(y1 == y2){
    //use temporary HWall
    VH_SEL = false;
    //set temporary wall array to HWall
    for(int i=0; i<9; i++){
      for(int j=0; j<9; j++){
        tempWall[i][j] = HWall[i][j];
      }
    }
  }
  else if(x1 == x2){
    //use temporary VWall
    VH_SEL = true;
    //set temporary wall array to VWall
    for(int i=0; i<9; i++){
      for(int j=0; j<9; j++){
        tempWall[i][j] = VWall[i][j];
      }
    }
  }
  //set new wall spots
  tempWall[y1][x1] = 1;
  tempWall[y2][x2] = 1;
  
  //make new temporary LED array with new temporary VWall or HWall
  int tempLED[9][18];
  for(int w=0; w<18; w++){
    // Serial.println();
    if(w%2 == 0){
      for(int t=0; t<9; t++){
        if(VH_SEL){  //use new VWall
          tempLED[t][w] = tempWall[t][w/2];
        }
        else{
          tempLED[t][w] = VWall[t][w/2];
        }
        // Serial.print(tempLED[t][w]);
      }
    }
    if(w%2 == 1){
      for(int t=0; t<9; t++){
        if(!VH_SEL){ //use new HWall
          tempLED[t][w] = tempWall[t][w/2];
        }
        else{
          tempLED[t][w] = HWall[t][w/2];
        }
        // Serial.print(tempLED[t][w]);
      }
    }
  }
  
  //placing AI wall, wait for button to be pushed
  //wait for cycle 1
  bool wallPlaced = false;
  while(!wallPlaced){
    LED_SEL(0);
    while(currCycle != 1){
      nextCycle();
    }
    //light temporary LED array
    for(int i=0; i<18; i++){
      for(int j=0; j<9; j++){
        if(tempLED[j][i] == 1){
          LED_SEL(j+1);
          delayMicroseconds(100);
        }
        else{
          LED_SEL(0);
        }
      }
      LED_SEL(0);
      nextCycle();
    }
    //check the board every 0.5 seconds
    newTime = millis()+500;
    if(millis() < newTime){
      //read board and check actual LED array and new LED array
      boardRead();
      matchWallLED();
      //printArr();
      
      //if they match, system can exit
      wallPlaced = true;
      // Serial.println();
      for(int i=0; i<18; i++){
        // Serial.println();
        for(int j=0; j<9; j++){
          if(tempLED[j][i] != LED_Arr[j][i]){
            wallPlaced = false;
            //Serial.print(j);
            //Serial.print(i);
          }
          // Serial.print(LED_Arr[j][i]);
        }
      }
    }
  }
}

//select an LED to turn on
void LED_SEL(int sel){
  //turn into binary selection
  if(sel != 0){
    //ones bit
    if(sel%2 == 1){
      digitalWrite(LED_A[boardtype], HIGH);
    }
    else{
      digitalWrite(LED_A[boardtype], LOW);
    }
    //twos bit
    if(sel/2 == 1 || sel/2 == 3){
      digitalWrite(LED_B[boardtype], HIGH);
    }
    else{
      digitalWrite(LED_B[boardtype], LOW);
    }
    //fours bit
    if(sel/4 == 1){
      digitalWrite(LED_C[boardtype], HIGH);
    }
    else{
      digitalWrite(LED_C[boardtype], LOW);
    }
    //eights bit
    if(sel/8 == 1){
      digitalWrite(LED_D[boardtype], HIGH);
    }
    else{
      digitalWrite(LED_D[boardtype], LOW);
    }
  }
  else{
    digitalWrite(LED_A[boardtype], LOW);
    digitalWrite(LED_B[boardtype], LOW);
    digitalWrite(LED_C[boardtype], LOW);
    digitalWrite(LED_D[boardtype], LOW);
  }
}

//turn on all LEDs
void allLED(){
  for(int j=1; j<18; j++){
    nextCycle();
    for(int i = 1; i<9; i++){
      LED_SEL(i);
      delayMicroseconds(100);
    }
  }
}

void cycleLED(){
  while(currCycle != 1){
    nextCycle();
  }
  //odd cycles
  for(int j=1; j<=18; j++){
    if(currCycle%2 == 1){
      for(int i = 1; i<=9; i++){
        LED_SEL(i);
        delay(100);
      }
    }
    nextCycle();
  }
  //even cycles
  for(int i = 1; i<=9; i++){
    LED_SEL(i);
    for(int j=1; j<=18; j++){
      if(currCycle%2 == 0){
        delay(100);
        if(currCycle == 18){
          delay(2500);
        }
        nextCycle();
      }
      else{
        nextCycle();
      }
    }
  }
}

//go to the next power cycle

void nextCycle(){
  currCycle++;
  if(currCycle > 18){
    currCycle = 1;
    digitalWrite(PWR_RST[boardtype], LOW);
  }
  digitalWrite(PWR_CLK[boardtype], HIGH);
  digitalWrite(PWR_RST[boardtype], HIGH);
  digitalWrite(PWR_CLK[boardtype], LOW);
}

void testAll()
{
  // testing all functions
  invalidMove();
  delay(2000);
  playerMove();
  delay(2000);
  AImove();
  delay(2000);
  numWallsLeftPlayer(10);
  delay(2000);
  numWallsLeftAI(10);
  delay(2000);
  lcdPlaceAIWall();
  delay(2000);
  movePieceOutOfWay();
  delay(2000);
  displayLocation(3, 4, 3);
  delay(2000);
}

void invalidMove()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Invalid move!");
  lcd.setCursor(0, 1);
  lcd.print("Try again");
  delay(1000);
}

void playerMove()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Your move...");
  delay(1000);
}
void AImove()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("AI's move...");
  delay(1000);
}

void numWallsLeftPlayer(int num)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("You have ");
  lcd.print(num);
  lcd.setCursor(0, 1);
  lcd.print("walls left");
  delay(1000);
}

void numWallsLeftAI(int num)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("The AI has ");
  lcd.print(num);
  lcd.setCursor(0, 1);
  lcd.print("walls left");
  delay(1000);
}

void lcdPlaceAIWall()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Please place a");
  lcd.setCursor(0, 1);
  lcd.print("wall for the AI");
  delay(1000);
}

void movePieceOutOfWay()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Please remove");
  lcd.setCursor(0, 1);
  lcd.print("piece(s) for AI");
  delay(1000);
}

void displayLocation(int row, int col, int dir)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Row: ");
  lcd.print(row);
  lcd.print(" Col: ");
  lcd.print(col);
  lcd.setCursor(0, 1);
  lcd.print("Facing ");
  String direction = "";
  switch (dir)
  {
    case 1:
      direction = "North";
      break;
    case 2:
      direction = "East";
      break;
    case 3:
      direction = "South";
      break;
    case 4:
      direction = "West";
      break;
  }
  lcd.print(direction);
  delay(1000);
}