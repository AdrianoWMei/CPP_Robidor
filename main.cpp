#include <iostream>
#include "robidor.h"
#include "algorithm.h"

int main(){
    robidor gamestate;
    Algorithm alg;
    gamestate.printBoard();
    while(!gamestate.isGameOver())
    {
        if(gamestate.playerOneTurn)
        {
            //manual player UI for testing. to be changed for embedded system
            char i, dir, horizontal;
            int x, y;
            std::cout<<"Will you move or build? (M/B)";
            std::cin >> i;
            if(i == 'M'){
                std::cout<<"What direction? (N/S/W/E) ";
                std::cin >> dir;
                gamestate.movePlayer(dir);
                gamestate.changeTurn();
            }
            else if( i == 'B'){
                std::cout<<"horizontal (Y/N)? ";
                std::cin >> horizontal;
                std::cout<<"Coordinate X? ";
                std::cin >> x;
                std::cout<<"Coordinate Y? ";
                std::cin >> y;
                if(horizontal == 'Y')
                {
                    if(gamestate.isWallValid(true,x,y)){
                        gamestate.buildWall(true,x,y);
                        gamestate.changeTurn();
                    }
                    else
                    {
                        std::cout<<"the wall is invaid, try again"<<std::endl;
                    }
                }
                else    
                {
                    if(gamestate.isWallValid(false,x,y)){
                        gamestate.buildWall(false,x,y);
                        gamestate.changeTurn();
                    }
                    else
                    {
                        std::cout<<"the wall is invaid, try again"<<std::endl;
                    }
                }

                //for embedded board
                //while
            }
            else{
                std::cout<<"incorrect value";
            }
        }
        else
        {
            gamestate.bfsBestPath(gamestate.playerTwoX, gamestate.playerTwoY, gamestate.playerOneTurn, true);
            //random alg.
            std::vector<Move> temp = gamestate.avalMove();
            //gamestate.bfsDistance(gamestate.playerTwoX, gamestate.playerTwoY, true, gamestate.playerOneTurn);
            // int random = rand() % temp.size();
            if(!gamestate.board[gamestate.playerTwoY][gamestate.playerTwoX].wallDown)
                gamestate.move(gamestate.playerTwoX, gamestate.playerTwoY-1);
            else if(!gamestate.board[gamestate.playerTwoY][gamestate.playerTwoX].wallLeft)
                gamestate.move(gamestate.playerTwoX-1, gamestate.playerTwoY);
            else if(!gamestate.board[gamestate.playerTwoY][gamestate.playerTwoX].wallRight)
                gamestate.move(gamestate.playerTwoX+1, gamestate.playerTwoY);
            else
                gamestate.move(gamestate.playerTwoX, gamestate.playerTwoY+1);
            // Move bestMove = alg.chooseBestMove(gamestate);
            gamestate.changeTurn();
        }
        gamestate.printBoard();
    }
    // std::cout<<gamestate.board[0][0].player<<std::endl;
    return 1;
};