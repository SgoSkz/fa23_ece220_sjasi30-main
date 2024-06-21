/* 
 * Author	Sean Jasin
 * NetID	sjasi3
 * UID		672969187
 * ECE		220
 * Date		Sept 13 '23
 *
 * Intro Paragraph
 * This is sample code written for the purpose of learning GDB debugging. The
 * following code has been modified so that it will compile and run properly.
 * The errors, have been documented below
 *
 * ERRORS
 * In main.c main function
 * 1. 	print_semiprimes did not have a semicolon
 *
 * In function is_prime
 * 1.	the return values were swapped, if number % i == 0 that means it is cleanly
 * 	divisible, this is not prime and should return 0. However the original
 * 	returned 1 if divisible and 0 when not. Which essential creates an
 * 	"is_not_prime" function
 *
 * In function print_semiprimes
 * 1.	k = i%j, k represents the other number for semiprime which is the
 * 	quotient. % finds remainder not the quotient. Replace % with /
 *
 * 2.	in the if( is_prime(k)) block, we need to set the ret value to 1, this
 * 	is because we found semiprimes. This affects the return value which
 * 	the function states: if semiprimes were not found return 0 else 1
 *
 * 3. 	in the same block from 3, we need to break the loop so we don't have
 * 	the same number appear multiple times
 */
#include <stdlib.h>
#include <stdio.h>


/*
 * is_prime: determines whether the provided number is prime or not
 * Input    : a number
 * Return   : 0 if the number is not prime, else 1
 */
int is_prime(int number)
{
    int i;
    if (number == 1) {return 0;}
    for (i = 2; i < number; i++) { //for each number smaller than it
        if (number % i == 0) { //check if the remainder is 0
            return 0;
        }
    }
    return 1;
}


/*
 * print_semiprimes: prints all semiprimes in [a,b] (including a, b).
 * Input   : a, b (a should be smaller than or equal to b)
 * Return  : 0 if there is no semiprime in [a,b], else 1
 */
int print_semiprimes(int a, int b)
{
    int i, j, k;
    int ret = 0;
    for (i = a; i <= b; i++) { //for each item in interval
        //check if semiprime
	for (j = 2; j <= i; j++) {
            if (i%j == 0) {
                if (is_prime(j)) {
		    k = i/j;			// Originally %, which get
						// remainder, we need to check
						// other number for prime so
						// use division
                    if (is_prime(k)) {
                        printf("%d ", i);
			ret = 1;		// Set return value to 1
			break;			// Skip rest for number because
						// no dupes
                    }
                }
            }
        }

    }
    printf("\n");
    return ret;
}
