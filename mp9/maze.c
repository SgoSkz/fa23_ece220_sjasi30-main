/*
 * Author:  Sean Jasin (sjasi3)
 * UIN:     672969187
 * Date:    25 Oct 2023
 * Description:
 *      This code is used for solving mazees using the DFS method which is
 * explained in the canvas page. This code is exclusively for the purpose of
 * class and is given without warranty. Any modification or usage of this code
 * resulting in damages cannot be held against the original creator. Use code at
 * your own risk. 
 *
 * The functions that I have modified:
 *  - createMaze            Creates the maze based on the file given
 *  - destroyMaze           Frees the memory taken up by the maze
 *  - printMaze             Prints the maze
 *  - solveMazeDFS          Solves the maze using DFS
 *
 * The functions that I have created:
 *  - canMove               Tells if moving to cell is possible
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "maze.h"


/*
 * createMaze -- Creates and fills a maze structure from the given file
 * INPUTS:       fileName - character array containing the name of the maze file
 * OUTPUTS:      None 
 * RETURN:       A filled maze structure that represents the contents of the input file
 * SIDE EFFECTS: None
 */
maze_t * createMaze(char * fileName)
{
    // Your code here. Make sure to replace following line with your own code.
    FILE *file = fopen(fileName, "r");                                  // Prep the file into file as FILE ptr
    maze_t *maze = malloc(sizeof(maze_t));                              // Allocate memory for maze

    char *txtBuf = malloc(sizeof(char) * 100);                          // Allocate memory for text buffer
    for(int row = -1; fgets(txtBuf, 100, file) != NULL; row++) {        // Until file has reached EOF
        if(row < 0) {                                                   // First line represents dimensions
            sscanf(txtBuf, "%d %d", &(maze->height), &(maze->width));   // string scan for dimensions
            maze->cells = malloc(sizeof(char*) * maze->height);         // Allocate memory for rows
            for(int i = 0; i < maze->height; i++) {                     // For every row 
                maze->cells[i] = malloc(sizeof(char) * maze->width);    // Allocate memory for cols
            }
            continue;                                                   // Continue
        }
        sscanf(txtBuf, "%[SE \%]", maze->cells[row]);                   // String scan for any "SE %" chars put into cell[i]
        for(int col = 0; col < maze->width; col++) {                    // Search entire line
            if(*(txtBuf + col) == 'S') {                                // Contains 'S' char?
                maze->startRow = row;                                   // Save coord for start row
                maze->startColumn = col;                                // Save coord for start col
            }
            if(*(txtBuf + col) == 'E') {                                // Contains 'E' char?
                maze->endRow = row;                                     // Save coord for end row
                maze->endColumn = col;                                  // Save coord for end col
            }
        }
    }
    fclose(file);                                                       // Close the file, we don't need anymore
    return maze;
}

/*
 * destroyMaze -- Frees all memory associated with the maze structure, including the structure itself
 * INPUTS:        maze -- pointer to maze structure that contains all necessary information 
 * OUTPUTS:       None
 * RETURN:        None
 * SIDE EFFECTS:  All memory that has been allocated for the maze is freed
 */
void destroyMaze(maze_t * maze)
{
    // Your code here.
    for(int i = 0; i < maze->height; i++) {         // For every row that exists
        free(maze->cells[i]);                       // Free the memory for the cols
    }
    free(maze->cells);                              // Free the memory for the rows
    free(maze);                                     // Free the memory for the entire maze
}

/*
 * printMaze --  Prints out the maze in a human readable format (should look like examples)
 * INPUTS:       maze -- pointer to maze structure that contains all necessary information 
 *               width -- width of the maze
 *               height -- height of the maze
 * OUTPUTS:      None
 * RETURN:       None
 * SIDE EFFECTS: Prints the maze to the console
 */
void printMaze(maze_t * maze)
{
    // Your code here.
    for(int i = 0; i < maze->height; i++) {
        for(int j = 0; j < maze->width; j++) {
            printf("%c", maze->cells[i][j]);
        }
        printf("\n");
    }
}

/*
 * solveMazeManhattanDFS -- recursively solves the maze using depth first search,
 * INPUTS:               maze -- pointer to maze structure with all necessary maze information
 *                       col -- the column of the cell currently beinging visited within the maze
 *                       row -- the row of the cell currently being visited within the maze
 * OUTPUTS:              None
 * RETURNS:              0 if the maze is unsolvable, 1 if it is solved
 * SIDE EFFECTS:         Marks maze cells as visited or part of the solution path
 */ 
int solveMazeDFS(maze_t * maze, int col, int row)
{
    // Your code here. Make sure to replace following line with your own code.

    // If the current position is neither START or END
    if(!(maze->cells[row][col] == START) && !(maze->cells[row][col] == END)) {
        maze->cells[row][col] = PATH;                   // Replace it with PATH
    }

    if(maze->cells[row][col] == END) {                  // If the current position is END
        return 1;                                       // Return 1, bois we did it
    }

    if(canMove(maze, col - 1, row)) {                   // If it can move here
        if(solveMazeDFS(maze, col - 1, row) > 0) {      // Try solving here and if solved
            return 1;                                   // return 1
        }
    }
    if(canMove(maze, col + 1, row)) {                   // If it can move here
        if(solveMazeDFS(maze, col + 1, row) > 0) {      // Try solving here and if solved
            return 1;                                   // return 1
        }
    }
    if(canMove(maze, col, row - 1)) {                   // If it can move here
        if(solveMazeDFS(maze, col, row - 1) > 0) {      // Try solving here and if solved
            return 1;                                   // return 1
        }
    }
    if(canMove(maze, col, row + 1)) {                   // If it can move here
        if(solveMazeDFS(maze, col, row + 1) > 0) {      // Try solving here and if solved
            return 1;                                   // return 1
        }
    }
    maze->cells[row][col] = VISITED;                    // Note that we've already visited this node
    return 0;                                           // Unsolved
}

/*
 * Tell the user if the move is possible
 * INPUTS:               maze -- pointer to maze structure with all necessary maze information
 *                       col -- the column of the cell currently beinging visited within the maze
 *                       row -- the row of the cell currently being visited within the maze
 * OUTPUTS:              None
 * RETURNS:              1 if move is possible, 0 otherwise
 */ 
int canMove(maze_t * maze, int col, int row) {
    if(!(col < maze->width) || col < 0) {
        return 0;
    }
    if(!(row < maze->height) || row < 0) {
        return 0;
    }
    if(!(maze->cells[row][col] == EMPTY) && !(maze->cells[row][col] == END)) {
        return 0;
    }
    return 1;
}
