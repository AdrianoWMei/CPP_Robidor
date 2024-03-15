#include <iostream>
#include <vector>
#include <queue>
#include "robidor.h"

// #define DEBUG

using namespace std;
//player one start (4,0) end goal (x,8)
//p2 start (4,8), end goal(x,0)
robidor::robidor()
    :turn(0), playerOneX(4), playerOneY(8), playerOneGoal(8), playerTwoX(4), playerTwoY(0), playerTwoGoal(0), playerOneWall(10), playerTwoWall(10), winner(0), playerOneTurn(true)
    {
        for (int y = 0; y < 10; ++y) {
            for (int x = 0; x < 10; ++x) {
                wall[y][x] = false;  // False indicates no wall is present
            }
        }

        for (int y = 0; y < 9; ++y){
            for(int x = 0; x < 9; ++x){
                board[y][x].player = 0;
                board[y][x].wallUp = false;
                board[y][x].wallDown = false;
                board[y][x].wallLeft = false;
                board[y][x].wallRight = false;
            }
        }
        // //2d array loop
        // for (int y = 0; y < 9; ++y){
        //     for(int x = 0; x < 9; ++x){
        //         board[y][x].player = input from bits;
        //     }
        // }

        // for (int y = 0; y < 10; ++y){
        //     for(int x = 0; x < 10; ++x){
        //         wall[y][x] = bit coe;
        //     }
        // }


        board[0][4].player = 1;
        board[8][4].player = 2;
    };

robidor::robidor(robidor& other)
: playerOneX(other.playerOneX), playerOneY(other.playerOneY), playerOneGoal(other.playerOneGoal), playerTwoX(other.playerTwoX), playerTwoY(other.playerTwoY), playerTwoGoal(other.playerTwoGoal), playerOneWall(other.playerOneWall), playerTwoWall(other.playerTwoWall), winner(other.winner), playerOneTurn(other.playerOneTurn)
{
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            wall[y][x] = other.wall[y][x];  // False indicates no wall is present
        }
    }

    for (int y = 0; y < 9; ++y){
        for(int x = 0; x < 9; ++x){
            board[y][x].player = other.board[y][x].player;
            board[y][x].wallUp = other.board[y][x].wallUp;
            board[y][x].wallDown = other.board[y][x].wallDown;
            board[y][x].wallLeft = other.board[y][x].wallLeft;
            board[y][x].wallRight = other.board[y][x].wallRight;
        }
    }

    board[playerOneY][playerOneX].player = other.board[playerOneY][playerOneX].player;
    board[playerTwoY][playerTwoX].player = other.board[playerTwoY][playerTwoX].player;
}


bool robidor::isGameOver()
    {
      if(playerOneY == 0){
        winner = 1;
        return true;
      }
      else if(playerTwoY == 8){
        winner = 2;
        return true;
      }
      winner = 0;
      return false;
    };

int robidor::getWinner()
    {
        return winner;
    }

int robidor::getPlayer(int x, int y)
{
    return board[y][x].player;
}

void robidor::movePlayer(char direction) 
{
    int& currentX = playerOneTurn ? playerOneX : playerTwoX;
    int& currentY = playerOneTurn ? playerOneY : playerTwoY;
    int& otherX = playerOneTurn ? playerTwoX : playerOneX;
    int& otherY = playerOneTurn ? playerTwoY : playerOneY;
    int currentPlayer = !playerOneTurn ? 1 : 2;

    // Calculate new position based on direction
    int newX = currentX, newY = currentY;
    switch (direction) {
        case 'N': newY -= 1; break;
        case 'S': newY += 1; break;
        case 'E': newX += 1; break;
        case 'W': newX -= 1; break;
        default: std::cout << "Invalid direction" << std::endl; return;
    }

    // Boundary check
    if (newX < 0 || newX >= 9 || newY < 0 || newY >= 9) {
        #ifdef DEBUG
        std::cout << "Outer bound error" << std::endl;
        #endif
        return;
    }

    // Wall check (assuming you have a function or logic to check for walls)
    if (isWallBetween(currentX, currentY, newX, newY)) {
        #ifdef DEBUG 
        std::cout << "Wall blocking the way" << std::endl;
        #endif
        return;
    }
    if (newX == otherX && newY == otherY)
    {
        
    }
    // Move player
    else
    {move(newX, newY);}
}

void robidor::move (int dX, int dY)
{
    std::cout<<dX<<" "<<dY<<std::endl;
    int& currentX = playerOneTurn ? playerOneX : playerTwoX;
    int& currentY = playerOneTurn ? playerOneY : playerTwoY;
    int& otherX = !playerOneTurn ? playerOneX : playerTwoX;
    int& otherY = !playerOneTurn ? playerOneY : playerTwoY;
    int currentPlayer = playerOneTurn ? 1 : 2;
    int otherPlayer = !playerOneTurn ? 1 : 2;


    // if(board[dX][dY].player == otherPlayer)
    // {
    //East/right
    if(currentX - dX == -1)
    {
        // if(board[dY][dX])
        std::cout<<"West"<<std::endl;
    }
    //left
    if(currentX - dX == 1)
    {
        std::cout<<"left"<<std::endl;
    }
    //down
    if(currentY - dY == -1)
    {
        std::cout<<"down"<<std::endl;
    }
    //north/up
    if(currentY - dY == 1)
    {
        std::cout<<"up"<<std::endl;
    }
    // }


    board[currentY][currentX].player = 0; // Clear current position
    board[dY][dX].player = currentPlayer; // Set new position  
    currentX = dX; // Update player's X position
    currentY = dY; // Update player's Y position
}

void robidor::buildWall(bool horizontal, int x, int y)
{
    if(playerOneTurn){
        playerOneWall--;
    }
    else
    {
        playerTwoWall--;
    }

    // if(x > 8 || x < 0)
    // {
    //     #ifdef DEBUG
    //     std::cout<<"out of bounds"<<std::endl;
    //     #endif
    //     return;
    // }
    // if(y > 8 || y < 0)
    // {
    //     #ifdef DEBUG
    //     std::cout<<"out of bounds"<<std::endl;
    //     #endif
    //     return;
    // }

    if(horizontal)
    {
        for (int i = 0; i <= 1; i++) {
            wall[y][x + i] = true;
            if(y > 0) board[y - 1][x + i].wallDown = true; // Cell above the wall
            if(y < 9) board[y][x + i].wallUp = true; // Cell below the wall
        }
    }
    else
    {
        for (int i = 0; i <= 1; i++) {
            wall[y + i][x] = true;
            // Update cell edges that are blocked by the wall
            if(x > 0) board[y + i][x - 1].wallRight = true; // Cell to the left of the wall
            if(x < 9) board[y + i][x].wallLeft = true; // Cell to the right of the wall
        }
    }
    return;
}

void robidor::removeWall(bool horizontal, int x, int y)
{
    if(playerOneTurn){
        playerOneWall++;
    }
    else
    {
        playerTwoWall++;
    }

    if(horizontal)
    {
        if(x+1 > 9){
            #ifdef DEBUG
            std::cout<<"out of bounds";
            #endif
            return;
        }
        for (int i = 0; i <= 1; i++) {
            wall[y][x + i] = false;
            if(y > 0) board[y - 1][x + i].wallDown = false; // Cell above the wall
            if(y < 9) board[y][x + i].wallUp = false; // Cell below the wall
        }
    }
    else
    {
        if(y +1 > 9){
            #ifdef DEBUG
            std::cout<<"out of bounds";
            #endif
            return;
        }
        // Place wall in 'wall' array and update 'board' cell edges accordingly
        for (int i = 0; i <= 1; i++) {
            wall[y + i][x] = false;
            // Update cell edges that are blocked by the wall
            if(x > 0) board[y + i][x - 1].wallRight = false; // Cell to the left of the wall
            if(x < 9) board[y + i][x].wallLeft = false; // Cell to the right of the wall
        }
    }
    return;
}

void robidor::printBoard() {
    for(int y = 0; y < 9; y++) {
        // Print the top edge of the cell row
        for(int x = 0; x < 9; x++) {
            std::cout << "+"; // Corner of the cell
            if (y > 0 && board[y-1][x].wallDown) std::cout << "---"; // Check for horizontal wall above
            else std::cout << "   ";
        }
        std::cout << "+" << std::endl; // Rightmost corner of the row

        // Print the player positions and vertical walls
        for(int x = 0; x < 9; x++) {
            if (x > 0 && board[y][x-1].wallRight) std::cout << "|"; // Check for vertical wall to the left
            else std::cout << " "; // Space if no vertical wall to the left

            // Print the player or empty space
            int player = getPlayer(x, y); // Assuming getPlayer() returns the player number or 0 for empty
            if(player == 1) std::cout << " 1 "; // Player 1
            else if(player == 2) std::cout << " 2 "; // Player 2
            else std::cout << "   "; // Empty space
        }

        // Print the rightmost vertical wall if present
        if (board[y][8].wallRight) std::cout << "|";
        std::cout << std::endl;

        // // Print horizontal walls and their intersections with vertical walls below each cell row, if any
        // if (y < 8) { // Skip the last row
        //     for(int x = 0; x < 9; x++) {
        //         std::cout << "+"; // Left corner or intersection
        //         if (board[y][x].wallDown) std::cout << "---"; // Horizontal wall
        //         else std::cout << "   ";
        //     }
        //     std::cout << "+" << std::endl; // Rightmost corner after horizontal walls
        // }
    }

    // Print the bottom edge of the last row
    for(int x = 0; x < 9; x++) {
        std::cout << "+";
        if (board[8][x].wallDown) std::cout << "---"; // Check for horizontal wall below the last row
        else std::cout << "   ";
    }
    std::cout << "+" << std::endl; // Rightmost corner of the last row
}

void robidor::changeTurn(){
    playerOneTurn = !playerOneTurn;
    turn += 1;
}

bool robidor::isWallBetween(int x1, int y1, int x2, int y2) {
    // Ensure the coordinates are within the board limits
    if (x1 < 0 || x1 >= 9 || y1 < 0 || y1 >= 9 || x2 < 0 || x2 >= 9 || y2 < 0 || y2 >= 9) {
        return true; // Treat out-of-bounds as blocked
    }

    // Horizontal movement
    if (y1 == y2) {
        if (x2 > x1) { // Moving right
            return board[y1][x1].wallRight;
        } else if (x2 < x1) { // Moving left
            return board[y2][x2].wallRight; // or board[y1][x1].wallLeft if you prefer
        }
    }
    // Vertical movement
    else if (x1 == x2) {
        if (y2 > y1) { // Moving down
            return board[y1][x1].wallDown;
        } else if (y2 < y1) { // Moving up
            return board[y2][x2].wallDown; // or board[y1][x1].wallUp if you prefer
        }
    }

    return false; // Assuming no wall by default if not moving horizontally or vertically
}

bool robidor::isPlayerAt(int x, int y){
    if(x == playerOneX && y == playerOneY){
        return true;
    }
    else if(x == playerTwoX && y == playerTwoY){
    return true;
    }
    return false;
}

std::vector<Move> robidor::avalMove() {
    std::vector<Move> moves;
    int currentPlayerX = playerOneTurn ? playerOneX : playerTwoX;
    int currentPlayerY = playerOneTurn ? playerOneY : playerTwoY;
    
    // Directions: N, S, E, W
    int dirs[4][2] = {{0, -1}, {0, 1}, {1, 0}, {-1, 0}};
    
    for (int i = 0; i < 4; ++i) {
        int newX = currentPlayerX + dirs[i][0];
        int newY = currentPlayerY + dirs[i][1];
        
        // Ensure move is within bounds and not blocked by a wall
        if (newX >= 0 && newX < 9 && newY >= 0 && newY < 9 && !isWallBetween(currentPlayerX, currentPlayerY, newX, newY)) {
            if (isPlayerAt(newX, newY)) {
                // Check for jump
                int jumpX = newX + dirs[i][0];
                int jumpY = newY + dirs[i][1];
                if (jumpX >= 0 && jumpX < 9 && jumpY >= 0 && jumpY < 9 && !isWallBetween(newX, newY, jumpX, jumpY) && !isPlayerAt(jumpX, jumpY)) {
                    moves.push_back(Move(jumpX, jumpY));
                } else if(isWallBetween(newX, newY, jumpX, jumpY)){
                    //check diagonals when there is a wall blocking the jump
                    // If jump is blocked, then check diagonals, ensuring no walls block these moves
                    for (int j = -1; j <= 1; j += 2) {
                        int diagX = currentPlayerX + (i >= 2 ? j : 0); // Adjust for horizontal movement
                        int diagY = currentPlayerY + (i < 2 ? j : 0); // Adjust for vertical movement
                        // Ensure diagonal move is within bounds, not blocked by a wall, and there's no player in the diagonal cell
                        if (diagX >= 0 && diagX < 9 && diagY >= 0 && diagY < 9 && !isWallBetween(currentPlayerX, currentPlayerY, diagX, diagY) && !isPlayerAt(diagX, diagY)) {
                            moves.push_back(Move(diagX, diagY));
                        }
                    }
                }
            } else {
                // Add the move if it's simply to an empty cell with no immediate obstructions
                moves.push_back(Move(newX, newY));
            }
        }
    }
    
    return moves;
}

bool robidor::checkPathExistsForPlayer(int startX, int startY, int goalY) {
    std::vector<std::vector<bool>> visited(9, std::vector<bool>(9, false));
    std::queue<Move> queue;
    queue.push(Move(startX,startY));

    while (!queue.empty()) {
        Move current = queue.front();
        queue.pop();

        // Goal check: has reached the goal line
        if (current.y == goalY) {
            return true;
        }

        // Mark the current cell as visited
        visited[current.y][current.x] = true;

        // Directions: N, S, E, W
        std::vector<Move> directions = {{0, -1}, {0, 1}, {1, 0}, {-1, 0}};

        for (const auto& dir : directions) {
            int newX = current.x + dir.x;
            int newY = current.y + dir.y;

            // Boundary check
            if (newX >= 0 && newX < 9 && newY >= 0 && newY < 9 && !visited[newY][newX]) {
                // Check if there's a wall between the current cell and the next cell
                if (!isWallBetween(current.x, current.y, newX, newY)) {
                    queue.push({newX, newY});
                }
            }
        }
    }

    return false; // No path found
}

bool robidor::isWallValid(bool horizontal, int x, int y) {
    //also make sure that a wall does not already exist ( can't build an intersection of wall)
    // Temporarily place the wall

    if(horizontal)
    {
      for (int i = 0; i <= 1; i++) {
          if(board[y][x+i].wallUp == true && board[y+1][x+i].wallDown == true)
          {
              // std::cout<<"a wall already exist here!"<<std::endl;
              return false;
          }
      }
    }else
    {
        // Place wall in 'wall' array and update 'board' cell edges accordingly
      for (int i = 0; i <= 1; i++) {
        //update to not use wall
          if(board[y+i][x].wallLeft == true && board[y+i][x+1].wallRight == true)
          {
              // std::cout<<"a wall already exist here!"<<std::endl;
              return false;
          }
        }
    }

    buildWall(horizontal, x, y);

    // Check if both players have a valid path to their respective goals
    bool playerOneHasPath = checkPathExistsForPlayer(playerOneX, playerOneY, playerOneGoal);
    bool playerTwoHasPath = checkPathExistsForPlayer(playerTwoX, playerTwoY, playerTwoGoal);

    // Remove the temporarily placed wall
    removeWall(horizontal, x, y);

    // If either player does not have a path, the wall placement is invalid
    return playerOneHasPath && playerTwoHasPath;
}

//This function takes a player's turn and coordinates to generate a 2d vector of the possible cells that the player can reach.
//The vector includes information on the distance (aka moves needed to reach there) and the path taken to reach it.
//It uses a BFS approach to search the possible locations.
std::pair<int,std::vector<Move>> robidor::bfsBestPath(int x, int y, bool goal, bool playerTurn)
{
    int currentGoal = !playerTurn ? playerOneGoal : playerTwoGoal;
    // std::cout<<currentGoal<<std::endl;
    int enemyID = !playerTurn ? 1 : 2;
    std::vector<std::vector<int>> visited(9,std::vector<int>(9,-1));
    std::vector<std::vector<std::vector<Move>>> moveMap(9, std::vector<std::vector<Move>>(9, std::vector<Move>(1, Move(-1, -1))));
    // std::vector<std::vector<std::<queue<Move>>>> moveList(9,std::vector<int>(9,Move(x,y)));
    struct Node
    {
        int x, y;
        int dist;
        std::vector<Move> backtrack;
        Node(int x, int y, int dist, std::vector<Move> backtrack = {})
        : x(x), y(y), dist(dist), backtrack(std::vector<Move> {Move(x,y)}) {};
        bool operator==(const Node& other) const{return x == other.x && y == other.y;} 
    };

    std::queue<Node> gameQ;
    std::vector<Move> initialMove;
    std::vector<Move> winningPath;

    initialMove.push_back(Move(x,y));
    moveMap[y][x] = initialMove;
    gameQ.push(Node(x,y,0,initialMove));
    // #ifdef DEBUG
    // std::cout<<"Initial Move"<<std::endl;
    // std::cout<<moveMap[x][y].front().x<<","<<moveMap[x][y].front().y<<std::endl;
    // #endif
    visited[y][x] = 0;
    while(!gameQ.empty()){
        Node currN = gameQ.front();
        // std::cout<<enemyID<<std::endl;
        gameQ.pop();
        // #ifdef DEBUG
        //     std::cout<<currN.x<< " "<< currN.y <<std::endl;
        //     std::cout<<currentGoal<<std::endl;
        // #endif
        if(goal && currentGoal == currN.y) {
            break;}
        if(currN.y + 1 < 9 && visited[currN.y+1][currN.x] < 0 && !isWallBetween(currN.x, currN.y,currN.x,currN.y+1))
        {
            if(board[currN.y+1][currN.x].player == enemyID && currN.y + 1 != 8)
            {
                visited[currN.y+1][currN.x] = 0;
                if (!isWallBetween(currN.x, currN.y+1,currN.x,currN.y+2) && currN.y + 2 < 9 && visited[currN.y+2][currN.x] < 0) 
                {
                    //Enemy player is in the attempted-to-reach position
                    //gamerule - attempt to jump over the player
                    //if condition checks 1. no wall behind the player
                    //2. position has not been reached before 3. boundary check
                    //queues the position behind the player
                    std::vector<Move> currPath = moveMap[currN.y][currN.x];
                    currPath.push_back(Move(currN.x, currN.y+2));
                    moveMap[currN.y+2][currN.x] = currPath;
                    visited[currN.y+2][currN.x] = currN.dist + 1;
                    if(goal && currentGoal == currN.y+2) {break;}
                    gameQ.push(Node(currN.x,currN.y+2, currN.dist+1,currPath));
                }
                else
                {
                if(!isWallBetween(currN.x,currN.y+1,currN.x+1,currN.y+1) && currN.x+1 < 9 && (visited[currN.y+1][currN.x+1] < 0 || currN.dist < visited[currN.y+1][currN.x+1]))
                    {
                    std::vector<Move> currPath = moveMap[currN.y][currN.x];
                    currPath.push_back(Move(currN.x+1, currN.y+1));
                    moveMap[currN.y+1][currN.x+1] = currPath;
                    visited[currN.y+1][currN.x+1] = currN.dist + 1;
                    if(goal && currentGoal == currN.y+2) {break;}
                    gameQ.push(Node(currN.x+1,currN.y+1, currN.dist+1,currPath));
                    }
                if(!isWallBetween(currN.x,currN.y+1,currN.x-1,currN.y+1) && currN.x-1 >= 0 && (visited[currN.y+1][currN.x-1] < 0 || currN.dist < visited[currN.y+1][currN.x-1]))
                    {
                    std::vector<Move> currPath = moveMap[currN.y][currN.x];
                    currPath.push_back(Move(currN.x-1, currN.y+1));
                    moveMap[currN.y+1][currN.x-1] = currPath;
                    visited[currN.y+1][currN.x-1] = currN.dist + 1;
                    if(goal && currentGoal == currN.y+2) {break;}
                    gameQ.push(Node(currN.x-1,currN.y+1, currN.dist+1,currPath));
                    }
                }
            }
            else{
            std::vector<Move> currPath = moveMap[currN.y][currN.x];
            currPath.push_back(Move(currN.x, currN.y+1));
            moveMap[currN.y+1][currN.x] = currPath;
            visited[currN.y+1][currN.x] = currN.dist + 1;
            if(goal && currentGoal == currN.y+1) {break;}
            gameQ.push(Node(currN.x,currN.y+1, currN.dist+1,currPath));}
        }
        if(currN.y - 1 >= 0 && visited[currN.y-1][currN.x] < 0 && !isWallBetween(currN.x, currN.y,currN.x,currN.y-1))
        {
            if(board[currN.y-1][currN.x].player == enemyID)
            {
                visited[currN.y-1][currN.x] = 0;
                if (!isWallBetween(currN.x, currN.y-1,currN.x,currN.y-2) && currN.y - 2 >= 0 && visited[currN.y-2][currN.x] < 0) 
                {
                    std::vector<Move> currPath = moveMap[currN.y][currN.x];
                    currPath.push_back(Move(currN.x, currN.y-2));
                    moveMap[currN.y-2][currN.x] = currPath;
                    visited[currN.y-2][currN.x] = currN.dist + 1;
                    if(goal && currentGoal == currN.y-2) {break;}
                    gameQ.push(Node(currN.x,currN.y-2, currN.dist+1,currPath));
                }
                else
                {
                if(!isWallBetween(currN.x,currN.y-1,currN.x+1,currN.y-1) && currN.x+1 < 9 && (visited[currN.y-1][currN.x+1] < 0 || currN.dist < visited[currN.y-1][currN.x+1]))
                    {
                    std::vector<Move> currPath = moveMap[currN.y][currN.x];
                    currPath.push_back(Move(currN.x+1, currN.y-1));
                    moveMap[currN.y-1][currN.x+1] = currPath;
                    visited[currN.y-1][currN.x+1] = currN.dist + 1;
                    if(goal && currentGoal == currN.y+1) {break;}
                    gameQ.push(Node(currN.x+1,currN.y-1, currN.dist+1,currPath));
                    }
                if(!isWallBetween(currN.x,currN.y-1,currN.x-1,currN.y-1) && currN.x-1 >= 0 && (visited[currN.y-1][currN.x-1] < 0|| currN.dist < visited[currN.y-1][currN.x-1]))
                    {
                    std::vector<Move> currPath = moveMap[currN.y][currN.x];
                    currPath.push_back(Move(currN.x-1, currN.y-1));
                    moveMap[currN.y-1][currN.x-1] = currPath;
                    visited[currN.y-1][currN.x-1] = currN.dist + 1;
                    if(goal && currentGoal == currN.y+1) {break;}
                    gameQ.push(Node(currN.x-1,currN.y-1, currN.dist+1,currPath));
                    }
                }   
            }
            else{
            std::vector<Move> currPath = moveMap[currN.y][currN.x];
            currPath.push_back(Move(currN.x, currN.y-1));
            moveMap[currN.y-1][currN.x] = currPath;
            visited[currN.y-1][currN.x] = currN.dist + 1;
            #ifdef DEBUG2
            // for(int i = 0; i < currPath.size(); i ++)
            //     std::cout<<currPath.size()<<currPath[i].x<<" "<<currPath[i].y <<std::endl;
            std::cout<<"size"<<std::endl;
            // std::cout<<currN.backtrack.size()<<std::endl;
            std::cout<<currPath.size()<<std::endl;
            std::cout<<"back"<<std::endl;
            std::cout<<" "<<currPath.back().x<<" "<<currPath.back().y <<std::endl;
            std::cout<<"front"<<std::endl;
            std::cout<<" "<<currPath.front().x<<" "<<currPath.front().y <<std::endl;
            #endif
            if(goal && currentGoal == currN.y-1) {break;}
            gameQ.push(Node(currN.x,currN.y-1, currN.dist+1,currPath));}
        }
        if(currN.x + 1 < 9 && visited[currN.y][currN.x+1] < 0 && !isWallBetween(currN.x, currN.y,currN.x+1,currN.y))
        {
            if(board[currN.y][currN.x+1].player == enemyID)
            {
                visited[currN.y][currN.x+1] = 0;
                if (!isWallBetween(currN.x+1, currN.y,currN.x+2,currN.y) && currN.x + 2 < 9 && visited[currN.y][currN.x+2] < 0) 
                {
                    std::vector<Move> currPath = moveMap[currN.y][currN.x];
                    currPath.push_back(Move(currN.x+2, currN.y));
                    moveMap[currN.y][currN.x+2] = currPath;
                    visited[currN.y][currN.x+2] = currN.dist + 1;
                    gameQ.push(Node(currN.x+2,currN.y, currN.dist+1,currPath));
                }
                else
                {
                if(!isWallBetween(currN.x+1,currN.y,currN.x+1,currN.y+1) && currN.y+1 < 9 && (visited[currN.y+1][currN.x+1] < 0 || currN.dist < visited[currN.y+1][currN.x+1]))
                    {
                    std::vector<Move> currPath = moveMap[currN.y][currN.x];
                    currPath.push_back(Move(currN.x+1, currN.y+1));
                    moveMap[currN.y+1][currN.x+1] = currPath;
                    visited[currN.y+1][currN.x+1] = currN.dist + 1;
                    if(goal && currentGoal == currN.y+1) {break;}
                    gameQ.push(Node(currN.x+1,currN.y+1, currN.dist+1,currPath));
                    }
                if(!isWallBetween(currN.x+1,currN.y,currN.x+1,currN.y-1) && currN.y-1 >= 0 && (visited[currN.y-1][currN.x+1] < 0|| currN.dist < visited[currN.y-1][currN.x+1]))
                    {
                    std::vector<Move> currPath = moveMap[currN.y][currN.x];
                    currPath.push_back(Move(currN.x+1, currN.y-1));
                    moveMap[currN.y-1][currN.x+1] = currPath;
                    visited[currN.y-1][currN.x+1] = currN.dist + 1;
                    if(goal && currentGoal == currN.y-1) {break;}
                    gameQ.push(Node(currN.x+1,currN.y-1, currN.dist+1,currPath));
                    }
                }   
            }
            else
            {
            std::vector<Move> currPath = moveMap[currN.y][currN.x];
            currPath.push_back(Move(currN.x+1, currN.y));
            moveMap[currN.y][currN.x+1] = currPath;
            visited[currN.y][currN.x+1] = currN.dist + 1;
            gameQ.push(Node(currN.x+1,currN.y, currN.dist+1,currPath));}
        }
        if(currN.x - 1 >= 0 && visited[currN.y][currN.x-1] < 0 && !isWallBetween(currN.x, currN.y,currN.x-1,currN.y))
        {
            if(board[currN.y][currN.x-1].player == enemyID)
            {
                visited[currN.y][currN.x-1] = 0;
                if (!isWallBetween(currN.x-1, currN.y,currN.x-2,currN.y) && currN.x - 2 < 9 && visited[currN.y][currN.x-2] < 0) 
                {
                    std::vector<Move> currPath = moveMap[currN.y][currN.x];
                    currPath.push_back(Move(currN.x-2, currN.y));
                    moveMap[currN.y][currN.x-2] = currPath;
                    visited[currN.y][currN.x-2] = currN.dist + 1;
                    gameQ.push(Node(currN.x-2,currN.y, currN.dist+1,currPath));
                }
                else
                {
                if(!isWallBetween(currN.x-1,currN.y,currN.x-1,currN.y+1) && currN.y+1 < 9 && (visited[currN.y+1][currN.x-1] < 0 || currN.dist < visited[currN.y+1][currN.x-1]))
                    {
                    std::vector<Move> currPath = moveMap[currN.y][currN.x];
                    currPath.push_back(Move(currN.x-1, currN.y+1));
                    moveMap[currN.y+1][currN.x-1] = currPath;
                    visited[currN.y+1][currN.x-1] = currN.dist + 1;
                    if(goal && currentGoal == currN.y+1) {break;}
                    gameQ.push(Node(currN.x-1,currN.y+1, currN.dist+1,currPath));
                    }
                if(!isWallBetween(currN.x-1,currN.y,currN.x-1,currN.y-1) && currN.y-1 >= 0 && (visited[currN.y-1][currN.x-1] < 0|| currN.dist < visited[currN.y-1][currN.x-1]))
                    {
                    std::vector<Move> currPath = moveMap[currN.y][currN.x];
                    currPath.push_back(Move(currN.x-1, currN.y-1));
                    moveMap[currN.y-1][currN.x-1] = currPath;
                    visited[currN.y-1][currN.x-1] = currN.dist + 1;
                    if(goal && currentGoal == currN.y-1) {break;}
                    gameQ.push(Node(currN.x-1,currN.y-1, currN.dist+1,currPath));
                    }
                }   
            }
            else{
            std::vector<Move> currPath = moveMap[currN.y][currN.x];
            currPath.push_back(Move(currN.x-1, currN.y));
            moveMap[currN.y][currN.x-1] = currPath;
            visited[currN.y][currN.x-1] = currN.dist + 1;
            gameQ.push(Node(currN.x-1,currN.y, currN.dist+1,currPath));}
        }
    }
    int lowestDist = 999;
    std::vector<Move> bt;
    std::pair<int,std::vector<Move>> bestMove;
    for (int i = 0 ; i < 9; i ++)
    {
        // std::cout<<playerOneBFS[playerOneGoal][i]<<std::endl;
        if(visited[currentGoal][i] != -1 && visited[currentGoal][i] < lowestDist)
        {
            lowestDist = visited[currentGoal][i];
            bt = moveMap[currentGoal][i];
        }
    }

    bestMove = std::make_pair(lowestDist,bt);

    #ifdef DEBUG
    for(int i = 0; i < 9; i ++)
    {
        for(int j = 0; j < 9 ; j ++)
        {
            std::cout<<"("<<visited[i][j]<<")";
        }
        std::cout<<endl;
    }
    // std::cout<<"distance"<<std::endl;
    // std::cout<<lowestDist<<std::endl;
    // std::cout<<"bt"<<std::endl;
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j ++)
        {
            std::cout<<"("<<visited[i][j]<<")";
        }
        std::cout<<std::endl;
    }
    
    // std::cout<<winningPath.front().x << winningPath.front().y<<std::endl;
    #endif
    return bestMove;

}

playMove robidor::evalBFS()
{
    int currentGoal = !playerOneTurn ? playerOneGoal : playerTwoGoal;
    int currentX = playerOneTurn ? playerOneX : playerTwoX;
    int currentY = playerOneTurn ? playerOneY : playerTwoY;
    int otherX = playerOneTurn ? playerTwoX : playerOneX;
    int otherY = playerOneTurn ? playerTwoY : playerOneY;
    std::pair<int,std::vector<Move>> currPlayer = bfsBestPath(currentX, currentY, true, playerOneTurn);
    std::pair<int,std::vector<Move>> otherPlayer = bfsBestPath(otherX, otherY, true, playerOneTurn);

    // std::pair<int,std::vector<Move>> wallBFSOther;
    // std::pair<int,std::vector<Move>> wallBFSCurr;

    // std::vector<WallPlacement> wallAval;
    // WallPlacement bestwall{false,-1,-1};
    // int lowestVal = 999;

    //compare best BFS values (loop through end points to find the lowest value)
    //if BFS for computer has lowest -> keep chasing path to it
    //else check all possible walls and their bfs result (pick the one that minimizes computer distance + maximize enemy player distance)
    //return the path move to reach the lowest value distanc
    // std::cout<<currPlayer.second[1].x<<" "<<currPlayer.second[1].y<<std::endl;
    playMove m {false, false, currPlayer.second[1].x, currPlayer.second[1].y};
    // if(currPlayer.first < otherPlayer.first)
    // {
    //     return m.x = ;
    //     //less distance --> prioritize moving
    // }
    // else
    // {
    //find the wall that increases distance
    // int wallDistOther = 0;
    //make sure that the wall does not increase computer distance
    // int wallDistCurr = currPlayer.first;
    // for(int i = 0; i < 7; i ++)
    // {
    //     for(int j = 0; j < 7; j ++)
    //     {
    //         if(isWallValid(true, i, j))
    //         {
    //             buildWall(true,i,j);
    //             std::pair<int,std::vector<Move>> wallBFSOther = bfsBestPath(otherX, otherY, true, playerOneTurn);
    //             std::pair<int,std::vector<Move>> wallBFSCurr = bfsBestPath(currentX, currentY, true, playerOneTurn);
    //             if(wallBFSCurr.first < wallBFSOther.first && wallBFSCurr.first  < lowestVal && wallBFSCurr.first <= currPlayer.first)
    //             {
    //                 bestwall = WallPlacement(i,j,true);
    //                 lowestVal = wallBFSCurr.first;
    //                 std::cout<<"the lowest horizontal wall distance is "<< wallBFSCurr.first<<std::endl;
    //             }
    //             // std::cout<<wallBFSOther.first<<" "<<wallBFSCurr.first<<std::endl;
    //             removeWall(true,i,j);
    //         }
    //         if(isWallValid(false, i, j))
    //         {
    //             buildWall(false,i,j);
    //             std::pair<int,std::vector<Move>> wallBFSOther = bfsBestPath(otherX, otherY, true, playerOneTurn);
    //             std::pair<int,std::vector<Move>> wallBFSCurr = bfsBestPath(currentX, currentY, true, playerOneTurn);
    //             // std::cout<<wallBFSOther.first<<" "<<wallBFSCurr.first<<std::endl;
    //             if(wallBFSCurr.first < wallBFSOther.first && wallBFSCurr.first  < lowestVal  && wallBFSCurr.first <= currPlayer.first)
    //             {
    //                 bestwall = WallPlacement(i,j,false);
    //                 lowestVal = wallBFSCurr.first;
    //                 std::cout<<"the lowest Vertical wall distance is "<< wallBFSCurr.first<<std::endl;
    //             }
    //             removeWall(false,i,j);
    //         }
    //     }
    // }

    // if(lowestVal < currPlayer.first)
    // {
    //     m.isWall = true;
    //     m.horizontal = bestwall.horizontal;
    //     m.x =  bestwall.x;
    //     m.y =  bestwall.y;
    //     std::cout<<"("<<m.isWall<<","<<bestwall.x<< bestwall.y<<")"<<std::endl;
    // }

    #ifdef DEBUG
    // std::cout<<"bfsdist"<<std::endl;
    // std::cout<<otherPlayer.first<<std::endl;
    // std::cout<<winningPath.front().x << winningPath.front().y<<std::endl;
    #endif
    // return currPlayer.second[1];
    return m;
}

// playMove robidor::evalBFS_forceWall()
// {
//     int currentGoal = playerOneTurn ? playerOneGoal : playerTwoGoal;
//     int currentX = playerOneTurn ? playerOneX : playerTwoX;
//     int currentY = playerOneTurn ? playerOneY : playerTwoY;
//     int otherX = playerOneTurn ? playerTwoX : playerOneX;
//     int otherY = playerOneTurn ? playerTwoY : playerOneY;
    
//     playMove wallPM {true, true, currentX, currentY};

//     int lowestVal = 9999;
//     for(int i = 0; i < 7; i ++)
//                 {
//                     for(int j = 0; j < 7 ; j ++)
//                     {
//                         if(isWallValid(true, i, j))
//                         {
//                             buildWall(true,i,j);
//                             std::pair<int,std::vector<Move>> wallBFSOther = bfsBestPath(otherX, otherY, true, playerOneTurn);
//                             std::pair<int,std::vector<Move>> wallBFSCurr = bfsBestPath(currentX, currentY, true, playerOneTurn);
//                             if(wallBFSCurr.first < wallBFSOther.first && wallBFSCurr.first  < lowestVal )
//                             {
//                                 WallPlacement bestWall = WallPlacement(j,i,true);
//                                 lowestVal = wallBFSCurr.first;
//                                 wallPM.x = j;
//                                 wallPM.y = i;
//                                 // std::cout<<"the lowest horizontal wall distance is "<< wallBFSCurr.first<<std::endl;
//                             }
//                             // std::cout<<wallBFSOther.first<<" "<<wallBFSCurr.first<<std::endl;
//                             removeWall(true,i,j);
//                         }
//                     }
//                     }
//     return wallPM;
// }

void robidor::copyAPI(int otherBoard[9][9], bool otherWall[10][10])
{
    for(int i = 0; i < 9; i ++)
    {
        for(int j = 0; j < 9; j ++)
        {
            if(otherBoard[i][j] != 0)
            {
                //update player position
                board[playerOneY][playerOneX].player = 0;
                board[i][j].player = otherBoard[i][j];
            }
        }
    }

    for(int i = 0; i < 10; i ++)
    {
        for(int j = 0; j < 10; j ++)
        {
            wall[i][j] = otherWall[i][j];
            //vertical wall down
            if(otherWall[i][j] == true && otherWall[i-1][j] == true && i+1 < 10)
            {
                 if(i > 0) board[i][j-1].wallLeft = true;
                 if(i > 0) board[i][j].wallLeft = true;
                 if(i < 9) board[i+1][j-1].wallRight = true;
                 if(i < 9) board[i+1][j].wallRight = true;
            }
            //horizontal wall
            if(otherWall[i][j] == true && otherWall[i][j+1] == true && j+1 < 10)
            {
                 if(i > 0) board[i][j].wallUp = true;
                 if(i > 0) board[i][j+1].wallUp = true;
                 if(j > 9) board[i-1][j].wallDown = true;
                 if(j < 9) board[i-1][j+1].wallDown = true;
            }
        }
    }
}

void robidor::reset(){
         for (int y = 0; y < 10; ++y) {
            for (int x = 0; x < 10; ++x) {
                wall[y][x] = false;  // False indicates no wall is present
            }
        }

        for (int y = 0; y < 9; ++y){
            for(int x = 0; x < 9; ++x){
                board[y][x].wallUp = false;
                board[y][x].wallDown = false;
                board[y][x].wallLeft = false;
                board[y][x].wallRight = false;
            }
        }
        //lcd to remove all walls and wait for the walls to be detected as removed
        while(playerOneX != 4 && playerOneY != 0)
        {
            if(playerOneX > 4)
            {
                //
                move(playerOneX - 1, playerOneY);
            }
            if(playerOneX < 4)
            {
                //move right
                move(playerOneX + 1, playerOneY);
            }
            if(playerOneY >= 0)
            {
                //move down
                move(playerOneX, playerOneY-1);
            }
        }

        board[0][4].player = 1;
        board[8][4].player = 2;  
        //reset the values
        playerOneX = 4;
        playerOneY = 0;
        playerTwoX = 4;
        playerTwoY = 8;
        playerOneWall = playerTwoWall = 10;
        winner = 0;
        playerOneTurn = true;
        //code to move robot back to its spot
}

void robidor::sendWall(int x1, int x2, int y1, int y2)
{
 //send coordinates to board   
}

bool robidor::detectWall()
{
    for(int i = 0; i < 9; i ++ )
    {
        for(int j = 0; j < 9; j ++)
        {
            if(wall[i][j] == true)
            {
                return true;
            }
        }
    }
    return false;
}