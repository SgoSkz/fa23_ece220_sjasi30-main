/*
 * Author 	Sean Jasin
 * NetID	sjasi3
 * UID		672929187
 * ECE		220
 * Date		Sept 13 '23
 *
 * This code prints out the values contained in a certain row of pascals triangle
 * ie row 4 would be 1 4 6 4 1. This code is provided for ECE220 MP3. 
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>

unsigned long k(int row, int k);		// Init function declaration

int main()
{
	int row;				// Initialize row for use in input

	printf("Enter a row index: ");		// Print message to user
	scanf("%d",&row);			// Scan line for %d, int

	// Write your code here
	for(int i = 0; i <= row; i++) {		// Goes thru all indices of a row
		printf("%ld ", k(row, i));	// Prints result of the function k given 
	}
	printf("\n");				// Prints a new line
	return 0;				// Return 0 on success
}

/*
 * Calculates the given function provided in the MP3 thing. The equation being
 * PI notation, i=1 to k, for (n+1-i)/i
 *
 * int row	which row to run the function for
 * int k	which index to calculate
 * return	unsigned long
 */
unsigned long k(int row, int k) {
	unsigned long ans = 1;				// Init var ans for use in PI notation
	for(int i = 1; i <= k; i++) {		// Loop through the numbers, i=1 thru k
		ans *= (row+1-i);		// Do math. See function documentation
		ans /= i;
	}
	return ans;		// Return the desired value
}
