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

struct playMove{
  bool isWall;
  bool horizontal;
  int x;
  int y;
  playMove(bool isWall, bool horizontal, int x, int y): isWall(isWall), horizontal(horizontal), x(x), y(y) {}
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

// struct Node
// {
//   int x, y;
//   int dist;
//   std::vector<Move> backtrack;
//   Node(int x, int y, int dist, std::vector<Move> backtrack = {})
//   : x(x), y(y), dist(dist), backtrack(std::vector<Move> {Move(x,y)}) {};
//   bool operator==(const Node& other) const{return x == other.x && y == other.y;} 
// };

class robidor {
  public:
    // Player playerOne;
    int turn;
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
    std::pair<int,std::vector<Move>> bfsBestPath(int x, int y, bool goal, bool playerTurn);
    playMove evalBFS();
    void copyAPI(int otherBoard[9][9], bool otherWall[10][10]);
    void reset();

    void sendWall(int x1, int x2, int y1, int y2);
    bool detectWall();

};


#endif
