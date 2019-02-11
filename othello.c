//***************************************************
// Filename: othello.c
//
// Author: Nandigam
// Students: Randy Nguyen and Sam Ventocilla
//***************************************************

#include "othello.h"
bool isOnBoard(int r, int c);


// Displays the board (as SIZE x SIZE grid) on console
void displayBoard(char board[][SIZE])
{
	printf("\n");
	for (int i = 0; i < SIZE; i++) {
		printf((i == 0 ? "%5d" : "%3d"), i+1);
	}
	printf("\n");

	for (int i = 0; i < SIZE; i++) {
		printf("%2d", i+1);
		for (int j = 0; j < SIZE; j++) {
			printf("%3c", board[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

// Initializes the board with start configuration of discs (see project specs)
void initializeBoard(char board[][SIZE])
{
	// Assign each board space as empty.
	for (int i = 0; i < SIZE; i++) 
		for (int j = 0; j < SIZE; j++) 
			board[i][j] = '-';

	// Assign diagonal spaces to each player.
	board[3][3] = 'W';
	board[4][4] = 'W';
	board[3][4] = 'B';
	board[4][3] = 'B';
	return;
}

// Returns true if moving the disc to location row,col is valid; else returns false
bool isValidMove(char board[][SIZE], int row, int col, char disc)
{
	// Check if move is on the board
	if (isOnBoard(row, col) == false)
		return false;

	// Check if move is onto an empty space
	if(isOnBoard(row, col) == true)
		if (board[row][col] != '-')
			return false;

	// Save the opponent color
	char opponent = 'B';
	if (disc == 'B')
		opponent = 'W';

	// Check if move captures opponent piece in any direction
	for (int i = -1; i < 2; i++)
		for (int j = -1; j < 2; j++) {
			int searchR = row + i;
			int searchC = col + j;
			
			// Only search when on the board.
			if (isOnBoard(searchR, searchC) == true){

				// See if we surround the opponent.
				if(board[searchR][searchC] == opponent) 
					while (board[searchR][searchC] == opponent) {
						// Continue the search in a straight line.
						if (i == -1 && j == -1) {
							searchR--;
							searchC--;
						} else if (i == 0 && j == -1) {
							searchC--;
						} else if (i == 0 && j == 1) {
							searchC++;
						}
						else if (i == 1 && j == -1) {
							searchR++;
							searchC--;
						} else if (i == -1 && j == 0) {
							searchR--;
						} else if (i == 1 && j == 0) {
							searchR++;
						} else if (i == -1 && j == 1) {
							searchR--;
							searchC++;
						} else if (i == 1 && j == 1) {
							searchR++;
							searchC++;
						}
						
						// Check if this new space is ours. Return true if it is.
						if (isOnBoard(searchR, searchC) == true){
							if (board[searchR][searchC] == disc) {
								return true;
							}	
						}
						else{
							break;	
						}
					}
			}
		}
	return false;
}

// HELPER FUNCTION: Checks if a move is on the board.
bool isOnBoard(int r, int c) 
{
	if (r >= SIZE || r < 0)
		return false;
	if (c >= SIZE || c < 0)
		return false;
	return true;
}

// Places the disc at location row,col and flips the opponent discs as needed
void placeDiscAt(char board[][SIZE], int row, int col, char disc)
{
	int tempR, tempC;
	int permR = row;
	int permC = col;
	//goes through all of the difference changes
	for(int rChange = -1; rChange < 2; rChange++){
		for(int cChange = -1; cChange < 2; cChange++){
			tempR = rChange + permR;
			tempC = cChange + permC;

			//make sure it's not a blank space or the current player
			if(isOnBoard(tempR, tempC) == true){
				while(board[tempR][tempC] != '-' && board[tempR][tempC] != disc){
					//increment the pieces past that piece
					tempR += rChange;
					tempC += cChange; 

					//check if the piece is equal to the current player
					if(isOnBoard(tempR, tempC) == true){
						if(board[tempR][tempC] == disc){
							row = permR;
							col = permC;
							
							for(;tempR != row || tempC != col; col += cChange, row += rChange){
								if(isOnBoard(tempR, tempC) == true)
									board[row][col] = disc;
							}
							break;
						}
					}
					if(isOnBoard(tempR, tempC) == false)
						break;
				}	
			}
		}
	}

	if(isOnBoard(permR, permC) == true)
		board[permR][permC] = disc;
}

// Returns true if a valid move for disc is available; else returns false
bool isValidMoveAvailable(char board[][SIZE], char disc)
{
	// Look at empty spaces for valid moves.
	for (int i=0; i<SIZE; i++)
		for (int j=0; j<SIZE; j++)
			if (board[i][j] == '-')
				if(isValidMove(board, i, j, disc) == true)
					return true;

	return false;
}

// Returns true if the board is fully occupied with discs; else returns false
bool isBoardFull(char board[][SIZE])
{
	// Stores the number of dashes on the board.
	int numDash = 0;

	// Checks every board spaces and totals number of dashes.
	for(int i=0; i<SIZE; i++)
		for(int j=0; j<SIZE; j++)
			if(board[i][j] == '-')
				numDash++;

	// If the board is full then there are 64 dashes.
	if(numDash == 64)
		return true;
	
	// There were not 64 dashes.
	return false;
}

// Returns true if either the board is full or a valid move is not available for either disc
bool isGameOver(char board[][SIZE])
{
	if(isBoardFull(board) || (isValidMoveAvailable(board, 'B') == false
			       && isValidMoveAvailable(board, 'W') == false) )
		return true;

	return false;
}

// If there is a winner, it returns the disc (BLACK or WHITE) associated with the winner.
// In case of a tie, it returns EMPTY
char checkWinner(char board[][SIZE])
{	
	// Store the number of white and black chips separately.
	int white = 0;
	int black = 0;

	// Check every board space for chips and increment by color.
	for (int i=0; i<SIZE; i++)
		for (int j=0; j<SIZE; j++)
			if (board[i][j] == 'W')
				white++;
			else if (board[i][j] == 'B')
				black++;

	// The higher chips of a color wins.
	if (white > black)
		return WHITE;
	else if (black > white)
		return BLACK;
	else
		return EMPTY;
}
