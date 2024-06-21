/* ======================== INTRODUCTION PARAGRAPH =============================
 * Author: Sean W Jasin (sjasi3)
 * UIN: 672969187
 * Date: 25 Sept '23
 *
 * Functionality and Tasks:
 * This program is the backend for the game needed to be designed for class. The
 * program has three main functions, make_guess, set_seed, and start_game. The
 * descriptions of each function is written above the function as a comment. I
 * have modified each function such that the required tests will work given
 * random scenarios. 
 */

/*
 *
 * prog5.c - source file adapted from UIUC ECE198KL Spring 2013 Program 4
 *           student code -- GOLD VERSION by Steven S. Lumetta
 */

/*
 * The functions that you must write are defined in the header file.
 * Blank function prototypes with explanatory headers are provided
 * in this file to help you get started.
 */



#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "prog5.h"


/*
 * You will need to keep track of the solution code using file scope
 * variables as well as the guess number.
 */

static int guess_number;
static int solution1;
static int solution2;
static int solution3;
static int solution4;


/*
 * set_seed -- This function sets the seed value for pseudorandom
 * number generation based on the number the user types.
 * The input entered by the user is already stored in the string seed_str by the code in main.c.
 * This function should use the function sscanf to find the integer seed value from the 
 * string seed_str, then initialize the random number generator by calling srand with the integer
 * seed value. To be valid, exactly one integer must be entered by the user, anything else is invalid. 
 * INPUTS: seed_str -- a string (array of characters) entered by the user, containing the random seed
 * OUTPUTS: none
 * RETURN VALUE: 0 if the given string is invalid (string contains anything
 *               other than a single integer), or 1 if string is valid (contains one integer)
 * SIDE EFFECTS: initializes pseudo-random number generation using the function srand. Prints "set_seed: invalid seed\n"
 *               if string is invalid. Prints nothing if it is valid.
 gcc -g -std=c99 -Wall -Werror main.c prog5.c -o mp5*/
int
set_seed (const char seed_str[])
{
    //    Example of how to use sscanf to read a single integer and check for anything other than the integer
    //    "int seed" will contain the number typed by the user (if any) and the string "post" will contain anything after the integer
    //    The user should enter only an integer, and nothing else, so we will check that only "seed" is read. 
    //    We declare
    //    int seed;
    //    char post[2];
    //    The sscanf statement below reads the integer into seed. 
    //    sscanf (seed_str, "%d%1s", &seed, post)
    //    If there is no integer, seed will not be set. If something else is read after the integer, it will go into "post".
    //    The return value of sscanf indicates the number of items read succesfully.
    //    When the user has typed in only an integer, only 1 item should be read sucessfully. 
    //    Check that the return value is 1 to ensure the user enters only an integer. 
    //    Feel free to uncomment these statements, modify them, or delete these comments as necessary. 
    //    You may need to change the return statement below
    int seed = INT_MIN;                                 // Set var to min value which no one will enter anyways
    char post[2];                                       // Init post[2] as per instructions
    int ret = sscanf(seed_str, "%d%1s", &seed, post);   // scan the string for a int and string/char
    if(seed == INT_MIN || ret != 1) {                   // If seed not changed || more than 1 value scanned
        printf("set_seed: invalid seed\n");             // Notify of invalid
        return 0;                                       // return early 0
    }
    /* printf("\n");                                       // print a new line so it looks like the one on canvas */
    srand(seed);                                        // Set seed for rand
    return 1;                                           // return status 1 for valid
}


/*
 * start_game -- This function is called by main.c after set_seed but before the user makes guesses.
 *               This function creates the four solution numbers using the approach
 *               described in the wiki specification (using rand())
 *               The four solution numbers should be stored in the static variables defined above. 
 *               The values at the pointers should also be set to the solution numbers.
 *               The guess_number should be initialized to 1 (to indicate the first guess)
 * INPUTS: none
 * OUTPUTS: *one -- the first solution value (between 1 and 8)
 *          *two -- the second solution value (between 1 and 8)
 *          *three -- the third solution value (between 1 and 8)
 *          *four -- the fourth solution value (between 1 and 8)
 * RETURN VALUE: none
 * SIDE EFFECTS: records the solution in the static solution variables for use by make_guess, set guess_number
 */
void
start_game (int* one, int* two, int* three, int* four)
{
    //your code here                                    // sol is shortened form of solution
    solution1 = *one    = (rand() % 8) + 1;             // Set the global var sol1 and value at ptr 1 to rand
    solution2 = *two    = (rand() % 8) + 1;             // Set the global var sol2 and value at ptr 2 to rand
    solution3 = *three  = (rand() % 8) + 1;             // Set the global var sol3 and value at ptr 3 to rand
    solution4 = *four   = (rand() % 8) + 1;             // Set the global var sol4 and value at ptr 4 to rand
    guess_number = 1;                                   // Set guess_number to 1 as per instructions on canvas
    /* printf("%d %d %d %d", solution1, solution2, solution3, solution4); */
}

/*
 * make_guess -- This function is called by main.c after the user types in a guess.
 *               The guess is stored in the string guess_str. 
 *               The function must calculate the number of perfect and misplaced matches
 *               for a guess, given the solution recorded earlier by start_game
 *               The guess must be valid (contain only 4 integers, within the range 1-8). If it is valid
 *               the number of correct and incorrect matches should be printed, using the following format
 *               "With guess %d, you got %d perfect matches and %d misplaced matches.\n"
 *               If valid, the guess_number should be incremented.
 *               If invalid, the error message "make_guess: invalid guess\n" should be printed and 0 returned.
 *               For an invalid guess, the guess_number is not incremented.
 * INPUTS: guess_str -- a string consisting of the guess typed by the user
 * OUTPUTS: the following are only valid if the function returns 1 (A valid guess)
 *          *one -- the first guess value (between 1 and 8)
 *          *two -- the second guess value (between 1 and 8)
 *          *three -- the third guess value (between 1 and 8)
 *          *four -- the fourth color value (between 1 and 8)
 * RETURN VALUE: 1 if the guess string is valid (the guess contains exactly four
 *               numbers between 1 and 8), or 0 if it is invalid
 * SIDE EFFECTS: prints (using printf) the number of matches found and increments guess_number(valid guess) 
 *               or an error message (invalid guess)
 *               (NOTE: the output format MUST MATCH EXACTLY, check the wiki writeup)
 */
int
make_guess (const char guess_str[], int* one, int* two, 
            int* three, int* four)
{
    //  One thing you will need to read four integers from the string guess_str, using a process
    //  similar to set_seed
    //  The statement, given char post[2]; and four integers w,x,y,z,
    //  sscanf (guess_str, "%d%d%d%d%1s", &w, &x, &y, &z, post)
    //  will read four integers from guess_str into the integers and read anything else present into post
    //  The return value of sscanf indicates the number of items sucessfully read from the string.
    //  You should check that exactly four integers were sucessfully read.
    //  You should then check if the 4 integers are between 1-8. If so, it is a valid guess
    //  Otherwise, it is invalid.  
    //  Feel free to use this sscanf statement, delete these comments, and modify the return statement as needed
    int right = 0, place = 0;                           // Init the two variables, right and place
                                                        // right signifies how many the user got right
                                                        // place signifies how many where in the wrong pos
    int ans[] = {INT_MIN, INT_MIN, INT_MIN, INT_MIN};   // Set all initial ans to INT_MIN which no one will input
    char post[2];                                       // Init post[2] as per instructions
    int pair_ans[] = {0, 0, 0, 0};                   // Tells us if number has already been paired
    int pair_sol[] = {0, 0, 0, 0};                   // Tells us if number has already been paired
    int sol[] = {                                       // Make the set of solutions as an array for easier
        solution1,                                      // comparison
        solution2,
        solution3,
        solution4
        };

    // Scan the string for 4 int, 1 c/s 
    int ret = sscanf(guess_str, "%d%d%d%d%1s", (ans+0), (ans+1), (ans+2), (ans+3), post);

    if(ret != 4) {                                      // There must be exactly 4 scanned
        printf("make_guess: invalid guess\n");          // inform user of invalid guess
        return 0;                                       // return early 0
    }

    // Iterate thru list of answers
    for(int i = 0; i < sizeof(ans)/sizeof(*(ans+0)); i++) {
        if(*(ans+i) == INT_MIN) {                       // If value @ ans+offset is INT_MIN
            printf("make_guess: invalid guess\n");      // Invalid guess
            return 0;                                   // return early 0
        }
        if(*(ans+i) <= 0 || *(ans+i) > 8) {             // Check values are within range
            printf("make_guess: invalid guess\n");      // Invalid guess
            return 0;                                   // return early 0
        }
    }

    // Iterate thru list of answers again
    for(int i = 0; i < sizeof(ans)/sizeof(*(ans+0)); i++) {
        if(*(ans+i) == *(sol+i)) {                      // Check if there are positional matches
            /* printf("paired pos %d", i); */
            right++;                                    // Count up for right, see above for var
            pair_ans[i] = 1;                            // Updates the paired status at pos
            pair_sol[i] = 1;                            // ^^^
            continue;                                   // Continue to proceeding loop
        }
    }

    // Iterate thru the solutions if positional was not same
    for(int i = 0; i < sizeof(ans)/sizeof(*(ans+0)); i++) {
        /* printf("checking %d\n", *(ans+i)); */
        if(pair_ans[i] != 0) {                          // Is first paired?
            continue;                                   // Skip if yes
        }
        // Iterate thru the solutions
        for(int j = 0; j < sizeof(sol)/sizeof(*(sol+0)); j++) {
            /* printf("pair at %d is %d", j, pair_ans[j]); */
            if(pair_sol[j] != 0) {                      // Is pos j paired?
                continue;                               // Skip if yes
            }
            /* printf("comparing %d to %d", *(ans+i), *(sol+j)); */
            if(*(ans+i) == *(sol+j)) {                  // Check if ans matches with any solution
                place++;                                // Count up for place, see above for var
                pair_sol[j] = 1;                        // Updates the paired status at pos
                break;                                  // Break out of this loop to avoid dupes
            }
        }
    }
    *one = ans[0];                                      // Update these things for main.c
    *two = ans[1];                                      
    *three = ans[2];
    *four = ans[3];

    // Let the user know information about their guess
    printf("With guess %d, you got %d perfect matches and %d misplaced matches.\n", guess_number, right, place);
    
    // Increment the number of guesses
    guess_number++;
    return 1;
}


