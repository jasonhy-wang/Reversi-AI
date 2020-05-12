// Author: Jason Wang
// Date: March 18, 2020
// Purpose: Set up the rerversi game for class made AI vs AI created by me

#include <stdio.h>
#include <stdbool.h>
#include "lab8part2lib.h"

// set up the game board
void preSetBoard(char board[][26], int n)
{
    // initalize the board to all unoccupied
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            board[i][j] = 'U';
        }
    }

    // set the centerPieces to the starting configuation
    board[(n / 2) - 1][(n / 2) - 1] = 'W';
    board[(n / 2) - 1][(n / 2)] = 'B';
    board[(n / 2)][(n / 2) - 1] = 'B';
    board[(n / 2)][(n / 2)] = 'W';

    return;
}
// prints the board
void printBoard(char board[][26], int n)
{
    int border = 97; // ascii value of a
    printf("  ");
    // prints the top border values of the board
    for(int i = 0; i < n; i++) {
        printf("%c", border);
        border++;
    }
    printf("\n");

    border = 97;
    // prints the game pieces
    for(int i = 0; i < n; i++) {
        printf("%c", border);
        printf(" ");
        for(int j = 0; j < n; j++) {
            printf("%c", board[i][j]);
        }
        border++;
        printf("\n");
    }
    return;
}

// check if the postion lies within the boarder dimensions
bool positionInBounds(int n, int row, int col)
{

    if(row >= 0 && row < n && col >= 0 && col < n) {

        return true;
    }

    return false;
}

// checks if row/col is a legal position for the colour piece to play
bool checkLegalInDirection(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol)
{

    int x = row + deltaRow;
    int y = col + deltaCol;

    // if the direction it's checking is not in bounds, or there is no more spaces after the line return false
    if(!positionInBounds(n, x, y) || !positionInBounds(n, x + deltaRow, y + deltaCol)) {
        return false;
    }

    // determine the opposte colour
    char opponentColour;
    if(colour == 'B') {
        opponentColour = 'W';
    } else {
        opponentColour = 'B';
    }

    // first check if there is the opposite color in the direction
    if(board[x][y] != opponentColour) {
        return false;
    }

    // if there is another one of your pieces piece after the opponent's piece the move is valid
    int i = 0;
    while(board[x + deltaRow * i][y + deltaCol * i] == opponentColour) {
        i++;
        if(!positionInBounds(n, x + deltaRow * i, y + deltaCol * i)) { // if whole row all oppoents colour
            return false;
        }
        if(board[x + deltaRow * i][y + deltaCol * i] == colour) { // if the next one is your own colour
            // printf("TRUE");
            return true;
        }
    }
    return false;
}

// player colour moves and pieces are swapped to colour
void swapPieces(char board[][26], int n, int x, int y, char colour)
{
    // determinate oppoent colour
    char opponentColour;
    if(colour == 'B') {
        opponentColour = 'W';
    } else {
        opponentColour = 'B';
    }

    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            if(i != 0 || j != 0) {
                if(checkLegalInDirection(board, n, x, y, colour, i, j)) {
                    int counter = 1;
                    // change the cour of the board until you get to your colour
                    while(board[x + i * counter][y + j * counter] == opponentColour) {
                        board[x + i * counter][y + j * counter] = colour;
                        counter++;
                    }
                }
            }
        }
    }
    return;
}

// checks if the move is valid
bool checkLegaLMove(char board[][26], int n, int i, int j, char colour)
{
    if(checkLegalInDirection(board, n, i, j, colour, -1, -1) || checkLegalInDirection(board, n, i, j, colour, 0, -1) ||
        checkLegalInDirection(board, n, i, j, colour, +1, -1) || checkLegalInDirection(board, n, i, j, colour, -1, 0) ||
        checkLegalInDirection(board, n, i, j, colour, +1, 0) || checkLegalInDirection(board, n, i, j, colour, -1, 1) ||
        checkLegalInDirection(board, n, i, j, colour, 0, +1) || checkLegalInDirection(board, n, i, j, colour, +1, +1)) {

        return true;
    }

    return false;
}

// calucate the score of of colour, score represents the number of pieces it will flip
int score(char board[][26], int n, int x, int y, char colour, int i, int j)
{
    // determine opponent colour
    char opponentColour;
    int score = 0;
    if(colour == 'B') {
        opponentColour = 'W';
    } else {
        opponentColour = 'B';
    }

    int counter = 1;

    // if corners instanly go ther
    if((x == 0 && y == 0) || (x == 0 && y == n - 1) || (x == n - 1 && y == 0) || (x == n - 1 && y == n - 1)) {
        return 99999;
    } else if((x == 1 && y == 0) || (x == 0 && y == 6) || (x == 7 && y == 6) || (x == 1 && y == 1)) {
        // right of corner gives acces to corner
        score = score - 2;
    } else if((x == 0 && y == 1) || (x == 0 && y == 6) || (x == 7 && y == 6) || (x == 1 && y == 1)) {
        // forces oppoonent to give access to corner block
        score = score + 2;
    } else if((x == 1 && y == 1) || (x == 2 && y == 6) || (x == 6 && y == 1) || (x == 6 && y == 6)) {
        // gives direct access to corner
        score = score - 4;
    } else if((x == 1 && y == 2) || (x == 1 && y == 5) || (x == 6 && y == 2) || (x == 6 && y == 5)) {
        // gives acces to corner blocks but not as bac
        score = score - 1;
    } else if((x == 2 && y == 2) || (x == 2 && y == 5) || (x == 5 && y == 2) || (x == 5 && y == 5)) {
        // forces user to give access to corner spot
        score = score + 1;
    } else if((x == 0 && y == 3) || (x == 0 && y == 4) || (x == 7 && y == 3) || (x == 7 && y == 4)) {
        // middle of map good position
        score = score + 1;
    } else if((x == 1 && y == 3) || (x == 1 && y == 4) || (x == 6 && y == 3) || (x == 6 && y == 4)) {
        // bad move -1 point
        score = score - 1;
    } else if((x == 2 && y == 3) || (x == 2 && y == 4) || (x == 5 && y == 3) || (x == 5 && y == 4)) {
        // good move takes control of center
        score = score + 1;
    }

    while(board[x + i * counter][y + j * counter] == opponentColour) {
        score++;
        //                        int currentX = x + i * counter;
        //                        int currentY = y + j * counter;
        counter++;
    }

    return score;
}

// make the bot move
void computerMoves(char board[][26], int n, char colour)
{

    int maxX;              // the x location of the best mofe
    int maxY;              // the y location of the best move
    int maxScore = -99999; // setting the inital max score to super low

    // go through every location on the board
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            // if the space is empty
            if(board[i][j] == 'U') {
                // check all directions of that piece
                for(int k = -1; k <= 1; k++) {
                    for(int q = -1; q <= 1; q++) {
                        if(k != 0 || q != 0) {
                            int points = 0;
                            // check the score of the move if the move is valid
                            if(checkLegalInDirection(board, n, i, j, colour, k, q)) {
                                // printf("x = %d, y = %d, i = %d, j = %d\n", i, j, k, q);
                                points = score(board, n, i, j, colour, k, q);
                                // see if this move is better than the pervious move
                                if(points > maxScore) {
                                    maxScore = points;
                                    maxX = i;
                                    maxY = j;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // computer makes the move
    //    printf("maxX = %d, maxY=%d\n", maxX, maxY);
    //    printf("maxScore  = %d\n", maxScore);
    board[maxX][maxY] = colour;

    // make the move and swaps the pieces
    swapPieces(board, n, maxX, maxY, colour);

    printf("Computer places %c at %c%c.\n", colour, maxX + 97, maxY + 97);
    return;
}

char nextPlayer(char currentPlayer)
{

    if(currentPlayer == 'W') {
        return 'B';
    } else {
        return 'W';
    }
}

void determineWinner(char board[][26], int n)
{

    int numB = 0;
    int numW = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(board[i][j] == 'W') {
                numW++;
            } else if(board[i][j] == 'B') {
                numB++;
            }
        }
    }

    if(numB > numW) {
        printf("B player wins.\n");
    }else if(numW>numB){
        printf("W player wins.\n");
    }else{
        printf("Draw!");
    }
}

int numAvailableMove(char board[][26], int n, char colour)
{
    int numMoves = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            // if the spot is empty check if the move is valid or not for white
            if(board[i][j] == 'U') {
                // check all directions and if any direction is true, return it as a possible move
                if(checkLegalInDirection(board, n, i, j, colour, -1, -1) ||
                    checkLegalInDirection(board, n, i, j, colour, 0, -1) ||
                    checkLegalInDirection(board, n, i, j, colour, +1, -1) ||
                    checkLegalInDirection(board, n, i, j, colour, -1, 0) ||
                    checkLegalInDirection(board, n, i, j, colour, +1, 0) ||
                    checkLegalInDirection(board, n, i, j, colour, -1, 1) ||
                    checkLegalInDirection(board, n, i, j, colour, 0, +1) ||
                    checkLegalInDirection(board, n, i, j, colour, +1, +1)) {
                    numMoves++;
                }
            }
        }
    }

    return numMoves;
}

// check if the game has ended by checking if there is U, return true if has not ended return false if it has
bool gameEnd(char board[][26], int n)
{
    bool game = false;
    int numB = 0;
    int numW = 0;

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(board[i][j] == 'U') {
                game = true;
            } else if(board[i][j] == 'B') {
                numB++;
            } else {
                numW++;
            }
        }
    }

    // if the board only have colour of one piece
    if(numB == 0 || numW == 0) {
        return false;
    }

    // if neither place can move end the game
    if(numAvailableMove(board, n, 'B') == 0 && numAvailableMove(board, n, 'W') == 0) {
        return false;
    }

    return game;
}

// moves for the created AI, looking ahead to minimize the number of flips the opponent can do
void bestAiMove(char board[][26], int n, char myAIColour)
{

    char tempBoard[n][26];
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            tempBoard[i][j] = board[i][j];
        }
    }
}
int main(void)
{

    int n;
    char computerAIColour; // colour of computer AI
    char myAIColour;       // colour of my AI
    char activePlayer = 'B';
    bool gameOn = true;
    int row;
    int col;

    printf("Enter the board dimension: ");
    scanf("%d", &n);
    printf("Computer plays(B/W): ");
    scanf(" %c", &myAIColour);

    if(myAIColour == 'W') {
        computerAIColour = 'B';
    } else {
        computerAIColour = 'W';
    }

    char board[n][26];

    preSetBoard(board, n);
    printBoard(board, n);

    while(gameOn) {
        if(numAvailableMove(board, n, activePlayer) != 0) {
            if(activePlayer == computerAIColour) {
                //                findSmartestMove(board, n, computerAIColour, &row, &col);
                //                printf("Testing AI move (row, col): %c%c\n", row + 'a', col + 'a');
                //                board[row][col] = computerAIColour;
                //                swapPieces(board, n, row, col, computerAIColour);
                char move[3];

                printf("Enter move for colour %c (RowCol): ", computerAIColour);
                scanf("%s", move);

                int x = move[0] - 97;
                int y = move[1] - 97;

                // check if the move is legal if it is make the move else the computer wins if its
                // invalid
                if(checkLegaLMove(board, n, x, y, computerAIColour)) {
                    // add the piece and swap accordingly
                    board[x][y] = computerAIColour;
                    swapPieces(board, n, x, y, computerAIColour);

                } else {
                    printf("Invalid move.\n"); // if an invalid move is detetcted the computer wins
                    gameOn = false;
                    printf("%c player wins.\n", computerAIColour);
                }

            } else {
                computerMoves(board, n, myAIColour);
            }

            if(gameOn) {
                printBoard(board, n);
                activePlayer = nextPlayer(activePlayer);

                // check if the game has ended;
                gameOn = gameEnd(board, n);
                if(!gameOn) {
                    determineWinner(board, n);
                }
            }
        } else {
            printf("%c player has no valid move.\n", activePlayer);
            activePlayer = nextPlayer(activePlayer);
        }
    }

    return 0;
}