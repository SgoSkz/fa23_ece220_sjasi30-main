/*
 * Author: Sean W Jasin (sjasi3)
 * UIN: 672969187
 * Date: 10-02-23
 * Partners: N/A
 * Description & Tasks:
 *      I created 3 different functions. Each one iterates thru a board either
 * making a copy or checking the surroundings of a single cell. The point of
 * this exercise is to practice array usage. I also used malloc and free to
 * practice safe memory usage. Whereas C99 standard now allows for
 * `type var[int var]` declaration of arrays. This is also known as VLA or
 * variable length array.
 */

/*
 * Code I removed for fear of getting docked
 *
            *(board + pos) =                                                // If neighbor either 2 or 3
                (*(newBoard + pos) == 2 || *(newBoard + pos) == 3)          // Shorthand IF notation (Condition) ? (ret if true) : (ret if false)
                                 ? *(board + pos)                           // Keep old value
                                 : 0;                                       // Set 0 (dead cell)
            *(board + pos) = (*(newBoard + pos) == 3) ? 1 : *(board + pos); // If # of neighbor 3 set 1 otherwise keep old value
 */

/*
 * countLiveNeighbor
 * Inputs:
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * row: the row of the cell that needs to count alive neighbors.
 * col: the col of the cell that needs to count alive neighbors.
 * Output:
 * return the number of alive neighbors. There are at most eight neighbors.
 * Pay attention for the edge and corner cells, they have less neighbors.
 */
#include <stdlib.h>
int countLiveNeighbor(int *board, int boardRowSize, int boardColSize, int row,
                      int col) {
    int cnt = 0;                                                    // Store the count of neighbors
    int curCol, curRow, pos;                                        // Initialize variables for later
    for (int i = -1; i < 2; i++) {
        curRow = row + i;                                           // Calculate the current row
        if (curRow < 0 || curRow >= boardRowSize) {continue;}       // No under/overflow
        for (int j = -1; j < 2; j++) {
            curCol = col + j;                                       // Calculate the current col
            pos = curCol + curRow * boardColSize;                   // Calulcate overall position
            if (curCol < 0 || curCol >= boardColSize) {continue;}   // No under/overflow
            if (pos == col + row * boardColSize) {continue;}        // Do not scan self
            if (*(board + pos) != 1) {continue;}                    // If dead cell move on
            cnt++;                                                  // Add to counter
        }
    }
    return cnt;                                                     // Return the amount of living neighbor cells
}

/*
 * Update the game board to the next step.
 * Input:
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: board is updated with new values for next step.
 */
void updateBoard(int *board, int boardRowSize, int boardColSize) {
    int *newBoard = malloc(boardRowSize * boardColSize * sizeof(int));      // Allocate memory to create a same size board
    int pos;                                                                // Store position somewhere
    // Create a board containing the number of alive neighbors
    for (int curRow = 0; curRow < boardRowSize; curRow++) {
        for (int curCol = 0; curCol < boardColSize; curCol++) {
            pos = curCol + curRow * boardColSize;                           // Calculate position
            *(newBoard + pos) =                                             // Store living neighbors in newBoard
                countLiveNeighbor(board, boardRowSize, boardColSize, curRow, curCol);
        }
    }
    // Calculate the next state of the board
    for (int curRow = 0; curRow < boardRowSize; curRow++) {
        for (int curCol = 0; curCol < boardColSize; curCol++) {
            pos = curCol + curRow * boardColSize;                           // Calculate position
            if(*(newBoard + pos) != 2 && *(newBoard + pos) != 3) {          // If not 2 or 3, then cell is dead
                *(board + pos) = 0;
            }
            if(*(newBoard + pos) == 3) {                                    // If 3 then cell is alive
                *(board + pos) = 1;
            }
        }
    }
    free(newBoard);                                                         // Free memory
    return;                                                                 // Explicit return
}

/*
 * aliveStable
 * Checks if the alive cells stay the same for next step
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: return 1 if the alive cells for next step is exactly the same with
 * current step or there is no alive cells at all.
 * return 0 if the alive cells change for the next step.
 */
int aliveStable(int *board, int boardRowSize, int boardColSize) {
    int *newBoard = malloc(boardRowSize * boardColSize * sizeof(int));  // Allocate memory to create the exact same board
    int pos;                                                            // Store position somewhere
    for (int curRow = 0; curRow < boardRowSize; curRow++) {
        for (int curCol = 0; curCol < boardColSize; curCol++) {
            pos = curCol + curRow * boardColSize;                       // Calculate position
            *(newBoard + pos) = *(board + pos);                         // Copy board to newBoard
        }
    }
    updateBoard(newBoard, boardRowSize, boardColSize);                  // Update newBoard to see if outcome changes
    for (int curRow = 0; curRow < boardRowSize; curRow++) {
        for (int curCol = 0; curCol < boardColSize; curCol++) {
            pos = curCol + curRow * boardColSize;                       // Calculate position
            if (*(newBoard + pos) != *(board + pos)) {                  // If at any point there is variation
                free(newBoard);                                         // Free memory
                return 0;                                               // Return early
            }
        }
    }
    free(newBoard);                                                     // Free memory
    return 1;
}
