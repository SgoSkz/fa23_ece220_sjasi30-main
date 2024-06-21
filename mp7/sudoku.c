#include "sudoku.h"

//-------------------------------------------------------------------------------------------------
// Start here to work on your MP7
//-------------------------------------------------------------------------------------------------
// Author: Sean W Jasin (sjasi3)
// UIN: 672969187
// Date: 11 Oct '23 00:17:54
// Description:
//  This is a program that solves a sudoku puzzle programmatically. There are 5
//  functions which I have modified to make this code work. Those functions are
//  the following
//  - is_val_in_row         => Checks if a value is in the corresponding row
//  - is_val_in_col         => Checks if a value is in the corresponding col
//  - is_val_in_3x3_zone    => Checks if a value is in the corresponding 3x3
//  - is_val_valid          => Checks if a value may be place at a coordinate
//  - solve_sudoku          => Solves the sudoku puzzle
//  The functionality of each function can be seen above, for more detail, the
//  functions each have comments throughout the entirety of the code. When
//  writing the code for `solve_sudoku` I used the pseudocode located on the
//  canvas page

// You are free to declare any private functions if needed.

// Function: is_val_in_row
// Return true if "val" already existed in ith row of array sudoku.
int is_val_in_row(const int val, const int i, const int sudoku[9][9]) {

    assert(i>=0 && i<9);

    // BEG TODO
    for(int k = 0; k < 9; k++) {        // Iterate 0 thru 9 because there are 9 rows
        if(val == sudoku[i][k]) {       // IF the value exists in the rows
            return 1;                   // Return true/1
        }
    }

    return 0;                           // Value does not exist return false/0
    // END TODO
}

// Function: is_val_in_col
// Return true if "val" already existed in jth column of array sudoku.
int is_val_in_col(const int val, const int j, const int sudoku[9][9]) {

    assert(j>=0 && j<9);

    // BEG TODO
    for(int k = 0; k < 9; k++) {        // Iterate 0 thru 9 because there are 9 cols
        if(val == sudoku[k][j]) {       // IF the value exists in the cols
            return 1;                   // Return true/1
        }
    }

    return 0;                           // Value does not exist return false/0
    // END TODO
}

// Function: is_val_in_3x3_zone
// Return true if val already existed in the 3x3 zone corresponding to (i, j)
int is_val_in_3x3_zone(const int val, const int i, const int j, const int sudoku[9][9]) {

    assert(i>=0 && i<9);

    // BEG TODO
    int zx, zy;                         // Init position values for the 3x3
    zx = i/3;                           // Get the x position for the 3x3 (top left)
    zy = j/3;                           // Get the y position for the 3x3 (top left)
    for(int k = 0; k < 3; k++) {        // Iterate thru 3
        for(int l = 0; l < 3; l++) {    // Iterate thru 3
            if(val == sudoku[zx*3+k][zy*3+l]) { // If the value exists in the 3x3
                return 1;               // Return true/0
            }
        }
    }

    return 0;                           // Value does nto exist return false/0
    // END TODO
}

// Function: is_val_valid
// Return true if the val is can be filled in the given entry.
int is_val_valid(const int val, const int i, const int j, const int sudoku[9][9]) {

    assert(i>=0 && i<9 && j>=0 && j<9);

    // BEG TODO
    if(is_val_in_row(val, i, sudoku)) { // If the value exists in row
        return 0;                       // Not valid, return false/0
    }
    if(is_val_in_col(val, j, sudoku)) { // If the value exists in col
        return 0;                       // Not valid, return false/0
    }
    if(is_val_in_3x3_zone(val, i, j, sudoku)) { // If the value exists in 3x3
        return 0;                       // Not valid return false/0
    }

    return 1;                           // Value does not yet exist and therefore valid, return true/1
    // END TODO
}

// Procedure: solve_sudoku
// Solve the given sudoku instance.
int solve_sudoku(int sudoku[9][9]) {

    // BEG TODO.

    int row = -1, col = -1;             // Init row and col to check
    for(int i = 0; i < 9; i++) {        // Iterate thru rows
        for(int j = 0; j < 9; j++) {    // Iterate thru cols
            if(sudoku[i][j] == 0) {     // If zero, sudoku is not solved
                row = i;                // Set row value accordingly
                col = j;                // Set col value accordingly
                break;                  // break out of loop
            }
        }
        if(row >= 0 && col >= 0) {      // If row and col were set, break out of loop
            break;
        }
    }
    if(row == -1 && col == -1) {        // If row and col were not set after checking
        return 1;                       // Sudoku puzzle is solved, return true/1
    }

    for(int num = 1; num <= 9; num++) { // Iterate thru 1 to 9 inclusive
        if(is_val_valid(num, row, col,  // Check if the number is valid in the pos
                         sudoku)) {     // we found earlier
            sudoku[row][col] = num;     // Set the value at the position we found to the number
            if(solve_sudoku(sudoku) == 1) { // If the sudoku is solved
                return 1;               // Return true/1
            }
            sudoku[row][col] = 0;       // Unset the value
        }
    }

    return 0;                           // Sudoku was not solved, return false/0
    // END TODO.
}

// Procedure: print_sudoku
void print_sudoku(int sudoku[9][9])
{
    int i, j;
    for(i=0; i<9; i++) {
        for(j=0; j<9; j++) {
            printf("%2d", sudoku[i][j]);
        }
        printf("\n");
    }
}

// Procedure: parse_sudoku
void parse_sudoku(const char fpath[], int sudoku[9][9]) {
    FILE *reader = fopen(fpath, "r");
    assert(reader != NULL);
    int i, j;
    for(i=0; i<9; i++) {
        for(j=0; j<9; j++) {
            fscanf(reader, "%d", &sudoku[i][j]);
        }
    }
    fclose(reader);
}





