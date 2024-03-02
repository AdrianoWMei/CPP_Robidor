#include <iostream>
#include <vector>
#include <queue>
#include "robidor.h"

using namespace std;
//player one start (4,0) end goal (x,8)
//p2 start (4,8), end goal(x,0)
robidor::robidor()
    : playerOneX(4), playerOneY(0), playerOneGoal(8), playerTwoX(4), playerTwoY(8), playerTwoGoal(0), playerOneWall(10), playerTwoWall(10), winner(0), playerOneTurn(true)
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
        board[playerOneY][playerOneX].player = 1;
        board[playerTwoY][playerTwoX].player = 2;
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
      if(playerOneY == 8){
        winner = 1;
        return true;
      }
      else if(playerTwoY == 0){
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
    int currentPlayer = playerOneTurn ? 1 : 2;

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
        std::cout << "Outer bound error" << std::endl;
        return;
    }

    // Wall check (assuming you have a function or logic to check for walls)
    if (isWallBetween(currentX, currentY, newX, newY)) {
        std::cout << "Wall blocking the way" << std::endl;
        return;
    }

    // Move player
    move(newX, newY);
}

void robidor::move (int dX, int dY)
{
    int& currentX = playerOneTurn ? playerOneX : playerTwoX;
    int& currentY = playerOneTurn ? playerOneY : playerTwoY;
    int currentPlayer = playerOneTurn ? 1 : 2;

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
            std::cout<<"out of bounds";
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
            std::cout<<"out of bounds";
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
        if(x+1 > 9){
            std::cout<<"out of bounds";
            return false;
        }
        for (int i = 0; i <= 1; i++) {
            if(wall[y][x+i] == true)
            {
                std::cout<<"a wall already exist here!"<<std::endl;
                return false;
            }
        }
    }else
    {
        if(y +1 > 9){
            std::cout<<"out of bounds";
            return false;
        }
        // Place wall in 'wall' array and update 'board' cell edges accordingly
        for (int i = 0; i <= 1; i++) {
            if(wall[y+i][x] == true)
            {
                std::cout<<"a wall already exist here!"<<std::endl;
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

std::vector<std::vector<int>> robidor::bfsBestPath(int x, int y, bool playerOneTurn, bool goal)
{
    std::vector<std::vector<int>> visited(9,std::vector<int>(9,-1));
    for(int i = 0; i < 9; i ++)
    {
        for(int j = 0; j < 9; j ++)
        {
            std::cout<<visited[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
    return visited;

}

