#ifndef ROBIDOR_H
#define ROBIDOR_H
#include <utility>
#include <vector>

struct cell{
  int player;
  bool wallUp;
  bool wallDown;
  bool wallLeft;
  bool wallRight;
};

struct WallPlacement{
  bool horizontal;
  int x;
  int y;
  WallPlacement(bool horizontal, int x, int y): horizontal(horizontal), x(x), y(y) {}
};

// //implement a struct for the player to be sort the information
// struct Player{
//   int playerID;
//   int x;
//   int y;
//   int goal;
//   int wallCount;
//   Player(int playerID, int x, int y, int goal, int wallCount): playerID(playerID), x(x), y(y), goal(goal), wallCount(wallCount) {}
// };

struct Move{
  int x;
  int y;
  Move(int x, int y): x(x), y(y) {}
};

class robidor {
  public:
    // Player playerOne;
    int playerOneX;
    int playerOneY;
    int playerOneGoal;
    int playerTwoX;
    int playerTwoY;
    int playerTwoGoal;
    int playerOneWall;
    int playerTwoWall;
    int winner;
    bool playerOneTurn;
    cell board[9][9]; 
    bool wall[10][10];
    robidor();
    robidor(robidor& other);
    bool isGameOver();
    int getWinner();
    int getPlayer(int x, int y);
    void movePlayer(char Direction);
    void move(int dX, int dY);
    void buildWall(bool horizontal, int x, int y);
    void removeWall(bool horizontal, int x, int y);
    void printBoard();
    void changeTurn();
    bool isPlayerAt(int x, int y);
    bool isWallBetween(int currX, int currY, int dX, int dY);
    std::vector<Move> avalMove();
    bool checkPathExistsForPlayer(int x, int y, int goal);
    bool isWallValid(bool horizontal, int x, int y);
    std::vector<std::vector<int>> bfsBestPath(int x, int y, bool playerOneTurn, bool goal);
};


#endif
