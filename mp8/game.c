#include "game.h"

/*
 * Author: Sean Jasin (sjasi3)
 * UIN: 672969187
 * Date: 19 October 2023 3:17 AM
 * Description:
 *      This is 2048 but in C because I had to do it for the MP. The method that
 * this game works is that it it checks the rows/cols starting with the furthest
 * cell at the input direction. IE if up, check the cells furthest up first then
 * go down. This works to make sure cells do not get added more than once. When
 * checking for legal moves left on the board, the code creates a copy of the
 * game and runs each move seeing if any moves are possible, if not it returns 0
 * meaning there are no possible moves
 *
 * Functions made/modified by me:
 *  - copy_the_game_lol
 *  - get_cell
 *  - legal_move_check
 *  - make_game
 *  - move_a
 *  - move_d
 *  - move_s
 *  - move_w
 *  - remake_game
 *
 *      This code was made for the intention of ECE220 in mind and holds no
 * liability in damages when trying attempting to imitate. By using this code
 * you acknowledge that any damange caused cannot hold me liable
 */

game * make_game(int rows, int cols)
/*! Create an instance of a game structure with the given number of rows
    and columns, initializing elements to -1 and return a pointer
    to it. (See game.h for the specification for the game data structure) 
    The needed memory should be dynamically allocated with the malloc family
    of functions.
*/
{
    //Dynamically allocate memory for game and cells (DO NOT modify this)
    game * mygame = malloc(sizeof(game));
    mygame->cells = malloc(rows*cols*sizeof(cell));

    //YOUR CODE STARTS HERE:  Initialize all other variables in game struct
    mygame->cols = cols;                        // Store the cols into the game
    mygame->rows = rows;                        // Store the rows into the game

    for(int i = 0; i < rows; i++) {             // Iterate thru rows
        for(int j = 0; j < cols; j++) {         // Iterate thru cols
            *get_cell(mygame, i, j) = -1;       // Set values to -1
        }
    }

    mygame->score = 0;                          // Set score to be 0 initially

    return mygame;
}

void remake_game(game ** _cur_game_ptr,int new_rows,int new_cols)
/*! Given a game structure that is passed by reference, change the
	game structure to have the given number of rows and columns. Initialize
	the score and all elements in the cells to -1. Make sure that any 
	memory previously allocated is not lost in this function.	
*/
{
	/*Frees dynamically allocated memory used by cells in previous game,
	 then dynamically allocates memory for cells in new game.  DO NOT MODIFY.*/
	free((*_cur_game_ptr)->cells);
	(*_cur_game_ptr)->cells = malloc(new_rows*new_cols*sizeof(cell));

	//YOUR CODE STARTS HERE:  Re-initialize all other variables in game struct
    (*_cur_game_ptr)->rows = new_rows;          // Store the new rows val in cur_game
    (*_cur_game_ptr)->cols = new_cols;          // Store the new cols val in cur_game
    for(int i = 0; i < new_rows; i++) {         // Iterate thru rows
        for(int j = 0; j < new_cols; j++) {     // Iterate thru cols
            *get_cell(*_cur_game_ptr, i, j) = -1;   // Set values to -1
        }
    }

	return;	
}

void destroy_game(game * cur_game)
/*! Deallocate any memory acquired with malloc associated with the given game instance.
    This includes any substructures the game data structure contains. Do not modify this function.*/
{
    free(cur_game->cells);
    free(cur_game);
    cur_game = NULL;
    return;
}

cell * get_cell(game * cur_game, int row, int col)
/*! Given a game, a row, and a column, return a pointer to the corresponding
    cell on the game. (See game.h for game data structure specification)
    This function should be handy for accessing game cells. Return NULL
	if the row and col coordinates do not exist.
*/
{
    //YOUR CODE STARTS HERE
    if(row*cur_game->cols + col >= cur_game->rows * cur_game->cols ) {return NULL;}
    return &(cur_game->cells[row*cur_game->cols+col]);  // To get the position we must
                                                        // do math such that a 1d array
                                                        // holds 2d data, row * # of
                                                        // cols + col
}

int move_w(game * cur_game)
/*!Slides all of the tiles in cur_game upwards. If a tile matches with the 
   one above it, the tiles are merged by adding their values together. When
   tiles merge, increase the score by the value of the new tile. A tile can 
   not merge twice in one turn. If sliding the tiles up does not cause any 
   cell to change value, w is an invalid move and return 0. Otherwise, return 1. 
*/
{
    //YOUR CODE STARTS HERE
    int changed = 0;                            // Keep the state stored in this value
    int *curVal, *against;                      // Ptrs to the current value and the value we're
                                                // checking against
    for(int checkRow = 0; checkRow < cur_game->rows - 1; checkRow++) {  // Go thru all the rows except for the last
        for(int checkCol = 0; checkCol < cur_game->cols; checkCol++) {  // Go thru every col
            curVal = get_cell(cur_game, checkRow, checkCol);            // Store the current value
            if(*curVal <= 0) {                                          // If the value is <0 we don't care abt this cell
                continue;                                               // Skip
            }
            for(int rowAgainst = checkRow+1; rowAgainst < cur_game->rows; rowAgainst++) {   // Iterate thru rows
                against = get_cell(cur_game, rowAgainst, checkCol);     // Store into ptr for cleaner code
                if(*curVal == *against) {                               // If the curVal value and agaisnt value are same
                    *curVal += *against;                                // Add against value to curVal value and put in curVal
                    *against = -1;                                      // Set agaisnt value to be -1 so we do not add to it again
                    changed = 1;                                        // The board has now changed update that
                    cur_game->score += *curVal;                         // The score is the original + the sum of the added values
                    break;
                }
                if(*against != -1) {                                    // If the cell we just checked is not -1
                    break;                                              // Break loop because the curVal cannot be combined
                }
            }
        }
    }

    // Shift everything up
    for(int shiftCol = 0; shiftCol < cur_game->cols; shiftCol++) {          // Go thru each column
        for(int shiftRow = 0; shiftRow < cur_game->rows - 1; shiftRow++) {  // Go thru each row except the last
            curVal = get_cell(cur_game, shiftRow, shiftCol);                // Store ptr
            if(*curVal != -1) {                 // If the value is not -1 it has something in it
                continue;                       // We cannot move value into here, skip
            }
            for(int find_nonzero = shiftRow + 1; find_nonzero < cur_game->rows; find_nonzero++) {   // Iterate thru every row after shiftRow
                against = get_cell(cur_game, find_nonzero, shiftCol);       // Store ptr
                if(*against == -1) {                                        // If the value is -1 it is zero
                    continue;                                               // Skip because we're not storing zeros anywhere
                }
                *curVal = *against;                                         // Store the non-zero in curVal value
                *against = -1;                                              // Set against value to -1
                changed = 1;                                                // The game state has changed
                break;                                                      // We will not be changing curVal again
            }
        }
    }

    return changed;
};

int move_s(game * cur_game) //slide down
{
    //YOUR CODE STARTS HERE
    int changed = 0;                            // Keep the state stored in this value
    int *curVal, *against;                      // Ptrs to the current value and the value we're
                                                // checking against
    for(int checkRow = cur_game->rows - 1; checkRow > 0; checkRow--) {  // Go thru all the rows except for the last
        for(int checkCol = 0; checkCol < cur_game->cols; checkCol++) {  // Go thru every col
            curVal = get_cell(cur_game, checkRow, checkCol);            // Store the current value
            if(*curVal <= 0) {                                          // If the value is <0 we don't care abt this cell
                continue;                                               // Skip
            }
            for(int rowAgainst = checkRow-1; rowAgainst >= 0; rowAgainst--) {   // Iterate thru rows
                against = get_cell(cur_game, rowAgainst, checkCol);     // Store into ptr for cleaner code
                if(*curVal == *against) {                               // If the curVal value and agaisnt value are same
                    *curVal += *against;                                // Add against value to curVal value and put in curVal
                    *against = -1;                                      // Set agaisnt value to be -1 so we do not add to it again
                    changed = 1;                                        // The board has now changed update that
                    cur_game->score += *curVal;                         // The score is the original + the sum of the added values
                    break;
                }                                                                                                                  
                if(*against != -1) {                                    // If the cell we just checked is not -1
                    break;                                              // Break loop because the curVal cannot be combined
                }
            }
        }
    }

    // Shift everything down
    for(int shiftCol = 0; shiftCol < cur_game->cols; shiftCol++) {          // Go thru each column
        for(int shiftRow = cur_game->rows - 1; shiftRow > 0; shiftRow--) {  // Go thru each row except the last
            curVal = get_cell(cur_game, shiftRow, shiftCol);                // Store ptr
            if(*curVal != -1) {                 // If the value is not -1 it has something in it
                continue;                       // We cannot move value into here, skip
            }
            for(int find_nonzero = shiftRow - 1; find_nonzero >= 0; find_nonzero--) {   // Iterate thru every row after shiftRow
                against = get_cell(cur_game, find_nonzero, shiftCol);   // Store ptr
                if(*against == -1) {                                    // If the value is -1 it is zero
                    continue;                                           // Skip because we're not storing zeros anywhere
                }                                                                                                        
                *curVal = *against;                                     // Store the non-zero in curVal value
                *against = -1;                                          // Set against value to -1
                changed = 1;                                            // The game state has changed
                break;                                                  // We will not be changing curVal again
            }
        }
    }

    return changed;
};

int move_a(game * cur_game) //slide left
{
    //YOUR CODE STARTS HERE
    int changed = 0;                            // Keep the state stored in this value
    int *curVal, *against;                      // Ptrs to the current value and the value we're
                                                // checking against
    for(int checkCol = 0; checkCol < cur_game->cols - 1; checkCol++) {  // Go thru all the cols except for the last
        for(int checkRow = 0; checkRow < cur_game->rows; checkRow++) {  // Go thru every row
            curVal = get_cell(cur_game, checkRow, checkCol);            // Store the current value
            if(*curVal <= 0) {                                          // If the value is <0 we don't care abt this cell
                continue;                                               // Skip
            }
            for(int colAgainst = checkCol+1; colAgainst < cur_game->cols; colAgainst++) {
                against = get_cell(cur_game, checkRow, colAgainst);     // Store into ptr for cleaner code
                if(*curVal == *against) {                               // If the curVal value and agaisnt value are same
                    *curVal += *against;                                // Add against value to curVal value and put in curVal
                    *against = -1;                                      // Set agaisnt value to be -1 so we do not add to it again
                    changed = 1;                                        // The board has now changed update that
                    cur_game->score += *curVal;                         // The score is the original + the sum of the added values
                    break;
                }                                                                                                                  
                if(*against != -1) {                                    // If the cell we just checked is not -1
                    break;                                              // Break loop because the curVal cannot be combined
                }
            }
        }
    }

    // Shift everything left
    for(int shiftRow = 0; shiftRow < cur_game->rows; shiftRow++) {          // Go thru each row
        for(int shiftCol = 0; shiftCol < cur_game->cols - 1; shiftCol++) {  // Go thru each col except the last
            curVal = get_cell(cur_game, shiftRow, shiftCol);                // Store ptr
            if(*curVal != -1) {                 // If the value is not -1 it has something in it
                continue;                       // We cannot move value into here, skip
            }
            for(int find_nonzero = shiftCol + 1; find_nonzero < cur_game->cols; find_nonzero++) {
                against = get_cell(cur_game, shiftRow, find_nonzero);   // Store ptr
                if(*against == -1) {                                    // If the value is -1 it is zero
                    continue;                                           // Skip because we're not storing zeros anywhere
                }                                                                                                        
                *curVal = *against;                                     // Store the non-zero in curVal value
                *against = -1;                                          // Set against value to -1
                changed = 1;                                            // The game state has changed
                break;                                                  // We will not be changing curVal again
            }
        }
    }

    return changed;
};

int move_d(game * cur_game){ //slide to the right
    //YOUR CODE STARTS HERE
    int changed = 0;                            // Keep the state stored in this value
    int *curVal, *against;                      // Ptrs to the current value and the value we're
                                                // checking against
    for(int checkCol = cur_game->cols - 1; checkCol > 0; checkCol--) {  // Go thru all the cols except for the last
        for(int checkRow = 0; checkRow < cur_game->rows; checkRow++) {  // Go thru every row
            curVal = get_cell(cur_game, checkRow, checkCol);            // Store the current value
            if(*curVal <= 0) {                                          // If the value is <0 we don't care abt this cell
                continue;                                               // Skip
            }
            for(int colAgainst = checkCol-1; colAgainst >= 0; colAgainst--) {
                against = get_cell(cur_game, checkRow, colAgainst);     // Store into ptr for cleaner code
                if(*curVal == *against) {                               // If the curVal value and agaisnt value are same
                    *curVal += *against;                                // Add against value to curVal value and put in curVal
                    *against = -1;                                      // Set agaisnt value to be -1 so we do not add to it again
                    changed = 1;                                        // The board has now changed update that
                    cur_game->score += *curVal;                         // The score is the original + the sum of the added values
                    break;
                }                                                                                                                  
                if(*against != -1) {                                    // If the cell we just checked is not -1
                    break;                                              // Break loop because the curVal cannot be combined
                }
            }
        }
    }

    // Shift everything right
    for(int shiftRow = 0; shiftRow < cur_game->rows; shiftRow++) {          // Go thru each row
        for(int shiftCol = cur_game->cols - 1; shiftCol > 0; shiftCol--) {  // Go thru each col except the last
            curVal = get_cell(cur_game, shiftRow, shiftCol);                // Store ptr
            if(*curVal != -1) {
                continue;
            }
            for(int find_nonzero = shiftCol - 1; find_nonzero >= 0; find_nonzero--) {
                against = get_cell(cur_game, shiftRow, find_nonzero);   // Store ptr
                if(*against == -1) {                                    // If the value is -1 it is zero
                    continue;                                           // Skip because we're not storing zeros anywhere
                }                                                                                                        
                *curVal = *against;                                     // Store the non-zero in curVal value
                *against = -1;                                          // Set against value to -1
                changed = 1;                                            // The game state has changed
                break;                                                  // We will not be changing curVal again
            }
        }
    }

    return changed;
};

int legal_move_check(game * cur_game)
/*! Given the current game check if there are any legal moves on the board. There are
    no legal moves if sliding in any direction will not cause the game to change.
	Return 1 if there are possible legal moves, 0 if there are none.
 */
{
    //YOUR CODE STARTS HERE
    game * copy_game = malloc(sizeof(game));    // Allocate the proper memory for the copy_game
    copy_game->cells = malloc(cur_game->rows*cur_game->cols*sizeof(cell));  // Allocate the proper memory for the cells
    copy_game->rows = cur_game->rows;           // Store the correct amount of rows in copy_game
    copy_game->cols = cur_game->cols;           // Store the correct amount of cols in copy_game

    copy_the_game_lol(cur_game, copy_game);     // Create a copy of the cur_game
    if(move_w(copy_game)) {                     // Test the movements on copy_game so real game is not modified
        return 1;                               // return 1 if valid
    }
    copy_the_game_lol(cur_game, copy_game);     // Create a copy of the cur_game
    if(move_s(copy_game)) {                     // Test the movements on copy_game so real game is not modified
        return 1;                               // return 1 if valid
    }
    copy_the_game_lol(cur_game, copy_game);     // Create a copy of the cur_game
    if(move_a(copy_game)) {                     // Test the movements on copy_game so real game is not modified
        return 1;                               // return 1 if valid
    }
    copy_the_game_lol(cur_game, copy_game);     // Create a copy of the cur_game
    if(move_d(copy_game)) {                     // Test the movements on copy_game so real game is not modified
        return 1;                               // return 1 if valid
    }
    return 0;                                   // Return 0, no valid moves
}

/*
 * Copies the cur_game to copy_game but dereferences the ptr
 */
void copy_the_game_lol(game * cur_game, game * copy_game) {
    cell *copy_cell;
    for(int row = 0; row < cur_game->rows; row++) {
        for(int col = 0; col < cur_game->cols; col++) {
            copy_cell = get_cell(copy_game, row, col);
            *copy_cell = *get_cell(cur_game, row, col);
        }
    }
}

/*! code below is provided and should not be changed */

void rand_new_tile(game * cur_game)
/*! insert a new tile into a random empty cell. First call rand()%(rows*cols) to get a random value between 0 and (rows*cols)-1.
*/
{
	
	cell * cell_ptr;
    cell_ptr = 	cur_game->cells;
	
    if (cell_ptr == NULL){ 	
        printf("Bad Cell Pointer.\n");
        exit(0);
    }
	
	
	//check for an empty cell
	int emptycheck = 0;
	int i;
	
	for(i = 0; i < ((cur_game->rows)*(cur_game->cols)); i++){
		if ((*cell_ptr) == -1){
				emptycheck = 1;
				break;
		}		
        cell_ptr += 1;
	}
	if (emptycheck == 0){
		printf("Error: Trying to insert into no a board with no empty cell. The function rand_new_tile() should only be called after tiles have succesfully moved, meaning there should be at least 1 open spot.\n");
		exit(0);
	}
	
    int ind,row,col;
	int num;
    do{
		ind = rand()%((cur_game->rows)*(cur_game->cols));
		col = ind%(cur_game->cols);
		row = ind/cur_game->cols;
    } while ( *get_cell(cur_game, row, col) != -1);
        //*get_cell(cur_game, row, col) = 2;
	num = rand()%20;
	if(num <= 1){
		*get_cell(cur_game, row, col) = 4; // 1/10th chance
	}
	else{
		*get_cell(cur_game, row, col) = 2;// 9/10th chance
	}
}

int print_game(game * cur_game) 
{
    cell * cell_ptr;
    cell_ptr = 	cur_game->cells;

    int rows = cur_game->rows;
    int cols = cur_game->cols;
    int i,j;
	
	printf("\n\n\nscore:%d\n",cur_game->score); 
	
	
	printf("\u2554"); // topleft box char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // top box char
	printf("\u2557\n"); //top right char 
	
	
    for(i = 0; i < rows; i++){
		printf("\u2551"); // side box char
        for(j = 0; j < cols; j++){
            if ((*cell_ptr) == -1 ) { //print asterisks
                printf(" **  "); 
            }
            else {
                switch( *cell_ptr ){ //print colored text
                    case 2:
                        printf("\x1b[1;31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4:
                        printf("\x1b[1;32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8:
                        printf("\x1b[1;33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 16:
                        printf("\x1b[1;34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 32:
                        printf("\x1b[1;35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 64:
                        printf("\x1b[1;36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 128:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 256:
                        printf("\x1b[32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 512:
                        printf("\x1b[33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 1024:
                        printf("\x1b[34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 2048:
                        printf("\x1b[35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4096:
                        printf("\x1b[36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8192:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
					default:
						printf("  X  ");

                }

            }
            cell_ptr++;
        }
	printf("\u2551\n"); //print right wall and newline
    }
	
	printf("\u255A"); // print bottom left char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // bottom char
	printf("\u255D\n"); //bottom right char
	
    return 0;
}

int process_turn(const char input_char, game* cur_game) //returns 1 if legal move is possible after input is processed
{ 
	int rows,cols;
	char buf[200];
	char garbage[2];
    int move_success = 0;
	
    switch ( input_char ) {
    case 'w':
        move_success = move_w(cur_game);
        break;
    case 'a':
        move_success = move_a(cur_game);
        break;
    case 's':
        move_success = move_s(cur_game);
        break;
    case 'd':
        move_success = move_d(cur_game);
        break;
    case 'q':
        destroy_game(cur_game);
        printf("\nQuitting..\n");
        return 0;
        break;
	case 'n':
		//get row and col input for new game
		dim_prompt: printf("NEW GAME: Enter dimensions (rows columns):");
		while (NULL == fgets(buf,200,stdin)) {
			printf("\nProgram Terminated.\n");
			return 0;
		}
		
		if (2 != sscanf(buf,"%d%d%1s",&rows,&cols,garbage) ||
		rows < 0 || cols < 0){
			printf("Invalid dimensions.\n");
			goto dim_prompt;
		} 
		
		remake_game(&cur_game,rows,cols);
		
		move_success = 1;
		
    default: //any other input
        printf("Invalid Input. Valid inputs are: w, a, s, d, q, n.\n");
    }

	
	
	
    if(move_success == 1){ //if movement happened, insert new tile and print the game.
         rand_new_tile(cur_game); 
		 print_game(cur_game);
    } 

    if( legal_move_check(cur_game) == 0){  //check if the newly spawned tile results in game over.
        printf("Game Over!\n");
        return 0;
    }
    return 1;
}
