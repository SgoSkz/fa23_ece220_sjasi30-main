;
; The code given to you here implements the histogram calculation that 
; we developed in class.  In programming lab, we will add code that
; prints a number in hexadecimal to the monitor.
;
; Your assignment for this program is to combine these two pieces of 
; code to print the histogram to the monitor.
;
; If you finish your program, 
;    ** commit a working version to your repository  **
;    ** (and make a note of the repository version)! **


	.ORIG	x3000		; starting address is x3000


;
; Count the occurrences of each letter (A to Z) in an ASCII string 
; terminated by a NUL character.  Lower case and upper case should 
; be counted together, and a count also kept of all non-alphabetic 
; characters (not counting the terminal NUL).
;
; The string starts at x4000.
;
; The resulting histogram (which will NOT be initialized in advance) 
; should be stored starting at x3F00, with the non-alphabetic count 
; at x3F00, and the count for each letter in x3F01 (A) through x3F1A (Z).
;
; table of register use in this part of the code
;    R0 holds a pointer to the histogram (x3F00)
;    R1 holds a pointer to the current position in the string
;       and as the loop count during histogram initialization
;    R2 holds the current character being counted
;       and is also used to point to the histogram entry
;    R3 holds the additive inverse of ASCII '@' (xFFC0)
;    R4 holds the difference between ASCII '@' and 'Z' (xFFE6)
;    R5 holds the difference between ASCII '@' and '`' (xFFE0)
;    R6 is used as a temporary register
;

	LD R0,HIST_ADDR      	; point R0 to the start of the histogram
	
	; fill the histogram with zeroes 
	AND R6,R6,#0		; put a zero into R6
	LD R1,NUM_BINS		; initialize loop count to 27
	ADD R2,R0,#0		; copy start of histogram into R2

	; loop to fill histogram starts here
HFLOOP	STR R6,R2,#0	; write a zero into histogram
	ADD R2,R2,#1		; point to next histogram entry
	ADD R1,R1,#-1		; decrement loop count
	BRp HFLOOP		    ; continue until loop count reaches zero

	; initialize R1, R3, R4, and R5 from memory
	LD R3,NEG_AT		; set R3 to additive inverse of ASCII '@'
	LD R4,AT_MIN_Z		; set R4 to difference between ASCII '@' and 'Z'
	LD R5,AT_MIN_BQ		; set R5 to difference between ASCII '@' and '`'
	LD R1,STR_START		; point R1 to start of string

	; the counting loop starts here
COUNTLOOP
	LDR R2,R1,#0		; read the next character from the string
	BRz PRINT_HIST		; found the end of the string

	ADD R2,R2,R3		; subtract '@' from the character
	BRp AT_LEAST_A		; branch if > '@', i.e., >= 'A'
NON_ALPHA
	LDR R6,R0,#0		; load the non-alpha count
	ADD R6,R6,#1		; add one to it
	STR R6,R0,#0		; store the new non-alpha count
	BRnzp GET_NEXT		; branch to end of conditional structure
AT_LEAST_A
	ADD R6,R2,R4		; compare with 'Z'
	BRp MORE_THAN_Z     ; branch if > 'Z'

; note that we no longer need the current character
; so we can reuse R2 for the pointer to the correct
; histogram entry for incrementing
ALPHA	ADD R2,R2,R0	; point to correct histogram entry
	LDR R6,R2,#0		; load the count
	ADD R6,R6,#1		; add one to it
	STR R6,R2,#0		; store the new count
	BRnzp GET_NEXT		; branch to end of conditional structure

; subtracting as below yields the original character minus '`'
MORE_THAN_Z
	ADD R2,R2,R5		; subtract '`' - '@' from the character
	BRnz NON_ALPHA		; if <= '`', i.e., < 'a', go increment non-alpha
	ADD R6,R2,R4		; compare with 'z'
	BRnz ALPHA		    ; if <= 'z', go increment alpha count
	BRnzp NON_ALPHA		; otherwise, go increment non-alpha

GET_NEXT
	ADD R1,R1,#1		; point to next character in string
	BRnzp COUNTLOOP		; go to start of counting loop



PRINT_HIST

; you will need to insert your code to print the histogram here

; do not forget to write a brief description of the approach/algorithm
; for your implementation, list registers used in this part of the code,
; and provide sufficient comments

; NOTE: Intro Paragraph
; This program prints a histogram by doing the following steps
; 1. Print the character and a space
; 2. Get the address of the beginning of the histogram
; 3. Read value stored at address
; 4. Get the 4 MSB and store it in another variable
; 5. Convert the value stored previously and convert it to HEX
;    5i. Subtract 10 from variable
;   5ii. Is value >= 0
;  5iii. Yes? Add offset from "A"
;   4iv. No? Add 10 then add offset from "0"
; 6. Loop back to the top
; Each sections uses registers differently but generally, in this program, R5
; and R6 are used for subroutine input and outputs. R4 is used as a counter. R0
; is used for single temp storage and print outputs

; this section is setting up variables and offsets
; R0 is used for printing and temp storage of HIST_ADDR
; R5 is used to create offsets iteration from 0-27
; R6 contains the number we are trying to print in hex
    LD R0, HIST_ADDR    ; Start of histogram at R0
    LD R5, HIST_POS     ; Get the current position in histogram
    ADD R0, R0, R5      ; Offset R0 by HIST_POS
    LDR R6, R0, #0      ; LOAD Value at addr stored in R0
    LD R0, ALPHA_CHAR   ; LOAD the first alphabet char
    ADD R0, R0, #-2     ; Offset -1 for @ symbol
    
; This section is for checking the current iteration and that it has not
; exceeded 27
    ADD R5, R5, #1      ; Add to the position
    ST R5, HIST_POS     ; Store the position
    ADD R5, R5, #-16    ; Subtract total of 27
    ADD R5, R5, #-11
    BRp DONE            ; If == 0 then finish
    
; After determining that the iteration has not passed 27, we set up the
; variables and stored values for the future. We also print the char being
; represented as well as a space char
    LD R5, HIST_POS     ; Set R5 to HIST_POS
    ADD R0, R0, R5      ; Offset +R5 for iteration through alphabet
    OUT                 ; Print current char
    LD R0, SPACE        ; Load SPACE into R0
    OUT                 ; Print space
    
; Sets up variables for grabbing the 4 MSB and storing into R5's LSB
MSB2LSB                 ; COPIES 4 MSB from R6 to R5 as LSB
    AND R4, R4, #0      ; Clear R4
    ADD R4, R4, #4      ; Set R4 to 4
    ST R4, ITER_ONE     ; Store R4 to ITER_ONE
    BR M2L_NBIT
    
; Prints hex variation of the number stored in R5 by checking if the number is
; greater than or equal to 10. If it is, subtract 10 and offset the char
; "A"+newR5
PRT_HEX                 ; Prints hex varaiation of number stored in R5
    LD R0, ALPHA_CHAR   ; Set R0 to "A"
    ADD R5, R5, #-10    ; Subtract 10
    BRzp #2             ; If >= 0, skip next two instructions
    ADD R5, R5, #10     ; Go back to original number
    LD R0, NUMBE_CHAR   ; Set R0 to "0"
    ADD R0, R0, R5      ; Add R5 (offset) to R0
    OUT                 ; Print
    
; Sets up the iteration for the next set of 4 bits
M2L_NBIT                ; LOOP for each set of 4 bits
    LD R4, ITER_ONE     ; Load ITER_ONE to R4
    ADD R4, R4, #-1     ; Count down
    BRzp #3             ; If ITER_ONE is < 0 leave loop
    LD R0, BREAK        ; Load newline into R0
    OUT                 ; Output newline
    BR PRINT_HIST       ; Go to PRINT_HIST
    ST R4, ITER_ONE     ; Store R4 back into ITER_ONE
    AND R4, R4, #0      ; Clear R4
    ADD R4, R4, #4      ; Set up R4 for iteration two
    AND R5, R5, #0      ; Clear R5

; Gets 4 MSB from R6 and stores them in R5 LSB
M2L_LOOP                ; Gets the value of the 4 MSB
    ADD R5, R5, R5      ; Bit shift left (doesn't matter if early because R5 should be #0)
    ; ADD R5, R5, #1      ; Add 1 to R5       NOTE: Can replace BRn with BRzp and ADD step
    ADD R6, R6, #0      ; Check value of R6
    BRzp #1             ; If > 0 (MSB is 1) skip 1 step
    ADD R5, R5, #1      ; Add 1
    ADD R6, R6, R6      ; BitBit shift left R6
    ADD R4, R4, #-1     ; Count down
    BRp M2L_LOOP        ; Go to next bit
    BR PRT_HEX

DONE	HALT			; done

ITER_ONE    .FILL #0
ITER_TWO    .FILL #0
HIST_POS    .FILL #0
ALPHA_CHAR  .STRINGZ "A"
NUMBE_CHAR  .STRINGZ "0"
BREAK       .STRINGZ "\n"
SPACE       .STRINGZ " "

; the data needed by the program
NUM_BINS	.FILL #27	; 27 loop iterations
NEG_AT		.FILL xFFC0	; the additive inverse of ASCII '@'
AT_MIN_Z	.FILL xFFE6	; the difference between ASCII '@' and 'Z'
AT_MIN_BQ	.FILL xFFE0	; the difference between ASCII '@' and '`'
HIST_ADDR	.FILL x3F00 ; histogram starting address
STR_START	.FILL x4000	; string starting address

; for testing, you can use the lines below to include the string in this
; program...
; STR_START	.FILL STRING	; string starting address
; STRING		.STRINGZ "This is a test of the counting frequency code.  AbCd...WxYz."



	; the directive below tells the assembler that the program is done
	; (so do not write any code below it!)

	.END
