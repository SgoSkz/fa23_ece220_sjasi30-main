; partners: N/A
; Author: Sean W Jasin (sjasi3, 672969187)
;
; Intro Paragraph
; This program is a calculator. In order to simplify the calculation process,
; the equations are written in RPN AKA (R)everse (P)olish (N)otation. The main
; functions implemented by me (not including sub functions ie, LOOP_FUNCTION or
; DONE_FUNCTION or subroutines) are the following
; - CHARS2STACK, CHECK_OPS, CHECK_NUMS, INVALID, EQUAL, OPERATOR, NUMBER
; 
; The main subroutines implemented by me (not including sub parts of
; subroutine) are the following
; - PRINT_HEX, CHECK_NEXT, EVALUATE, PLUS, MIN, MUL, DIV, EXP
;
; Descriptions of each function and subroutine is listed prior to its
; definition/LABEL
;
; Because of some issues regarding whether to implement negatives as a result
; of a different calculation for division and exponents, I have implemented
; negatives for both just in case. However, this does not affect the
; functionality of the program when the inputs are positive.
.ORIG x3000

; Implementation of LOOP that gets character input
CHARS2STACK 
    AND R2, R2, #0          ; Make sure R2 is NOT set
    GETC                    ; GETC -> R0
    OUT                     ; OUT -> R0
    ST R0, R0_ORIG          ; Save R0 in case we change it
    JSR CHECK_NEXT          ; Checking for equal sign
    BRnp #1                 ; If NOT '=', skip the BR
    JSR EQUAL               ; Run the EQUAL function
    JSR CHECK_NEXT          ; Checking for space
    BRz CHARS2STACK         ; If ' ', BR back to checking CHAR
    
; Checks if input was an operator
CHECK_OPS
    ADD R2, R2, #-7         ; Subtract by 7
    BRnp #2                 ; If it IS 0, we finished going thru operators
    AND R2, R2, #0          ; Reset R2
    BR CHECK_NUMS           ; Check for number input instead
    ADD R2, R2, #7          ; Add the 7 back for orig number
    JSR CHECK_NEXT          ; Checks against symbols "+-*/^"
    BRz OPERATOR            ; If CHECK_NEXT set cc to 0, input is operator
    BR CHECK_OPS            ; LOOP back to top
    
; Checks if input was a number
CHECK_NUMS
    ADD R2, R2, #-9         ; Subtract total of 17
    ADD R2, R2, #-8
    BRnp #2                 ; If it IS 0, we finished going thru numbers
    AND R2, R2, #0          ; Reset R2
    BR INVALID              ; Result is not a recognizable symbol
    ADD R2, R2, #9          ; Add total of 17 back to get orig number
    ADD R2, R2, #8          
    JSR CHECK_NEXT          ; Checks against numbers 0-9
    BRz NUMBER              ; If CHECK_NEXT set cc to 0, input is number
    BR CHECK_NUMS           ; LOOP back to top

; Tells user input was not valid
INVALID
    LEA R0, INV_MSG         ; Load message address to R0
    PUTS                    ; Print out message
    HALT                    ; Halt program

; Checks next symbols which are valid in address stored at R1
CHECK_NEXT
    LEA R1, SYMBOLS         ; Load the valid symbols
    ADD R1, R1, R2          ; Add offset of R2 to the address
    LDR R1, R1, #0          ; Load symbol value into R1. i.e. A=65 B=66 etc
    NOT R1, R1              ; Negate R1
    ADD R1, R1, #1
    ADD R2, R2, #1          ; Add to R2 offset
    LD R0, R0_ORIG          ; Get the original R0 in case it changed
    ADD R0, R0, R1          ; Add the new R1 to R0. results in 0 if same
    RET                     ; Return to R7
    
; What to do if input was '='
EQUAL
    LD R3, STACK_START      ; Load STACK_START -> R3
    LD R4, STACK_TOP        ; Load STACK_TOP -> R4
    ADD R4, R4, #1          ; Since we want 1 value in stack, add 1
    NOT R4, R4              ; Negate R4
    ADD R4, R4, #1
    ADD R3, R3, R4          ; Add the new R4 to R3
    BRnp INVALID            ; If NOT 0, contents are invalid
    JSR POP                 ; POP the value in the STACK
    ADD R3, R0, #0          ; STORE result in R3
    ADD R5, R0, #0          ; STORE result in R5 as told in MP2 instructions
    BR PRINT_HEX            ; Print the hex variation of value
    
; Decides which operation to use based on input. If popping results in INVALID,
; this function goes to INVALID function. The rest of this function is
; essentially my own implementation of switch case. R2 is used as the input.
; R2: switch case input, R3: subroutine input, R4: subroutine input
; R5, TABLE address for jumping to address
OPERATOR
    JSR POP                 ; POP
    ADD R5, R5, #0          ; Check against R5 for validity
    BRnp INVALID            ; Invalid if R5 is NOT 0
    ADD R4, R0, #0          ; STORE result in R4
    JSR POP                 ; POP for second value
    ADD R5, R5, #0          ; Check against R5 for validity
    BRnp INVALID            ; Invalid if R5 is NOT 0
    ADD R3, R0, #0          ; STORE result in R3
    ADD R2, R2, #-3         ; R2 holds the offset of operators +3
    LEA R5, TABLE           ; Load the effective address of the table to R5
    ADD R5, R5, R2          ; Offset the address by R2
    JSRR R5                 ; JSSR to loc at R5
    JSR PUSH                ; Push the resulting R0 to the STACK
    BR CHARS2STACK          ; Go back to checking inputs

; A table of BRs to go to which simplifies the process of going to the correct
; operation subroutine
TABLE
    BR PLUS
    BR MIN
    BR MUL
    BR DIV
    BR EXP
    HALT                    ; Should only happen if something goes wrong (unlikely)
    
; Pushes number to STACK unless full
; R0: character input, R6: EVALUATE's output,
; R5: validity of PUSH (0: OK, 1:NOT OK)
NUMBER
    LD R0, R0_ORIG          ; Load original R0 in case it was changed
    JSR EVALUATE            ; Evaluate R0 as number
    ADD R0, R6, #0          ; Put the number value into R0
    JSR PUSH                ; Push number into the STACK
    ADD R5, R5, #0          ; Check validity of PUSH
    BRz CHARS2STACK         ; OK? Go receive more char input
    BR INVALID              ; NOT OK? Yell at the user
    
; R0_ORIG is used to hold the original R0 value
; SYMBOLS is used to check against user input
; INV_MSG is the message to print when invalid
R0_ORIG .BLKW #1
SYMBOLS .STRINGZ "= +-*/^0123456789"
INV_MSG .STRINGZ "Invalid Expression"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;R3- value to print in hexadecimal
; R0: PRINTING OUTPUT & MSB2LSB OUTPUT
; R3: Original input and TMP_COUNTER (for 4 HEX)
; R4: BIT Counter (for 4 BITS)
PRINT_HEX
    AND R0, R0, #0          ; Make sure R0 is cleared
    ST R3, R3_VAL           ; Store the original R3 value
    AND R3, R3, #0          ; Clear R3
    ADD R3, R3, #4          ; Set up counter for R3
    ST R3, TMP_COUNTER      ; Store the counter
    ST R4, R4_VAL           ; Store R4's original value
    LD R4, TMP_COUNTER      ; Set up counter for R4

; Puts the MSB of R3 into R0
MSB2LSB
    ADD R0, R0, R0          ; Bit shift R0 LEFT
    LD R3, R3_VAL           ; Get orig R3 value
    ADD R3, R3, #0          ; Add 0 set cc
    BRzp #1                 ; If NOT negative skip 1 PC addr
    ADD R0, R0, #1          ; Add 1 to R0
    ADD R3, R3, R3          ; Bit shift R3 LEFT
    ST R3, R3_VAL           ; Store R3 into R3_VAL
    ADD R4, R4, #-1         ; Count down R4
    BRp MSB2LSB             ; Still positive? LOOP to top
    ADD R3, R0, #0          ; Set R3 equal to R0. B/C R3 is input for CONV_HEX
    BR CONV_HEX             ; Explicit BR to CONV_HEX. 

; Converts whatever value in R3 is to its HEX equivalent.
; R3: Input, R0: used for printing
CONV_HEX
    LEA R0, HEX_CONV        ; Load effective address of HEX_CONV to R0
    ADD R0, R0, R3          ; Add an offset of R3
    LDR R0, R0, #0          ; Load symbol at addr stored in R0
    OUT                     ; Output the symbol in R0
    AND R4, R4, #0          ; Reset R4 counter
    ADD R4, R4, #4
    LD R3, TMP_COUNTER      ; Load TMP_COUNTER (HEX Counter)
    AND R0, R0, #0          ; Reset R0
    ADD R3, R3, #-1         ; Count down HEX Counter
    ST R3, TMP_COUNTER      ; Store HEX Counter for later
    BRp MSB2LSB             ; cc positive from R3 + -1? Keep looping
    ; LD R0, NEW_LINE         ; This wasn't necessary so it's commented out
    ; OUT
    LD R4, R4_VAL           ; Load the original R4 value back to R4
    HALT                    ; Finished program
    
; HEX_CONV is used to make printing the HEX variation of 0-15 easily
; NEW_LINE stores a NEW_LINE character, UNUSED
; TMP_COUNTER stores every temporary counter <- only used once at a time
; R3_VAL holds original R3 value
; R4_VAL holds original R4 value
; R7_VAL holds original R7 value <- used when JSR'ing twice
HEX_CONV    .STRINGZ "0123456789ABCDEF"
; NEW_LINE    .STRINGZ "\n"
TMP_COUNTER .BLKW #1
R3_VAL      .BLKW #1
R4_VAL      .BLKW #1
R7_VAL      .BLKW #1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;R0 - character input from keyboard
;R6 - current numerical output
;
; Evaluates the number representation of R0 by subtracting the character '0'
; from R0
EVALUATE
    LD R6, HEX_CONV         ; Load the first char in HEX_CONV (aka '0')
    NOT R6, R6              ; Negate R6
    ADD R6, R6, #1
    ADD R6, R6, R0          ; Add R0 to R6. Will result in numerical output
    RET                     ; Return to R7

;your code goes here


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
;
; Does addition such that R0 = R3 + R4
PLUS	
    AND R0, R0, #0          ; Reset R0
    ADD R0, R3, R4          ; Set R0 = R3 + R4
    RET                     ; Return to R7
;your code goes here
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
;
; Does subtraction such that R0 = R3 - R4
MIN	
    AND R0, R0, #0          ; Reset R0
    NOT R4, R4              ; Negate R4
    ADD R4, R4, #1
    ADD R0, R3, R4          ; Set R0 = R3 + R4 (where R4 is now negated)
    RET                     ; Return to R7
;your code goes here
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
;
; Does multiplication such that R0 = R3 * R4
MUL	
    AND R0, R0, #0          ; Reset R0
    ST R4, R4_VAL           ; Store R4 original value

; Loops until right side number R4 magnitude is 0
LOOP_MUL
    ADD R4, R4, #0          ; Check R4 if negative
    BRzp #2                 ; If NOT negative, skip 2 PC addr
    NOT R4, R4
    ADD R4, R4, #1
    
    ADD R4, R4, #-1         ; Count down R4
    BRn DONE_MUL            ; If result is negative, the loop has finished
    ADD R0, R0, R3          ; Add R3 to R0 until loop finishes
    BR LOOP_MUL             ; LOOP to top

; What to do when loop is finished
DONE_MUL
    LD R4, R4_VAL           ; Load R4 original value back to R4
    ADD R4, R4, #0          ; Set CC
    BRzp #2                 ; If value zero or positive, do not negate R0
    NOT R0, R0              ; Negate R0
    ADD R0, R0, #1
    RET                     ; Return to R7

;your code goes here
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
;
; Does division such that R0 = R3 / R4
DIV	
    AND R0, R0, #0          ; Reset R0
    ST R3, R3_VAL           ; Store original R3 value
    ST R4, R4_VAL           ; Store original R4 value
    ADD R4, R4, #0          ; Set CC
    BRz INVALID             ; Invalid no division by zero
    
; Division loop until cant subtract
LOOP_DIV
    ADD R3, R3, #0          ; Set CC
    BRzp #2                 ; if NOT negative, skip negating R3
    NOT R3, R3              ; Negate R3
    ADD R3, R3, #1
    
    ADD R4, R4, #0          ; Set CC
    BRn #2                  ; if negative, skip negating R4
    NOT R4, R4              ; Negate R4
    ADD R4, R4, #1
    
    ADD R3, R3, R4          ; Subtact R4 from R3
    BRn DONE_DIV            ; Negative? Don't add to R0 and finish
    ADD R0, R0, #1          ; Add to R0
    BR LOOP_DIV             ; Loop back to top
    
; What to do when DIV is finished
DONE_DIV
    LD R3, R3_VAL           ; Load original R3 value
    ADD R3, R3, #0          ; Set CC
    BRzp #2                 ; If NOT negative, don't negate R0
    NOT R0, R0              ; Negate R0
    ADD R0, R0, #1
    
    LD R4, R4_VAL           ; Load original R4 value
    ADD R4, R4, #0          ; Set CC
    BRzp #2                 ; If NOT negative, don't negate R0
    NOT R0, R0              ; Negate R0
    ADD R0, R0, #1
    RET                     ; Return to R7
;your code goes here
	
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
;
; Does exponents such that R0 = R3 ^ R4
EXP
    ST R3, R3_VAL           ; Store the R3 value
    ST R4, TMP_COUNTER      ; Store R4 as TMP_COUNTER
    ST R7, R7_VAL           ; Store original R7. MUL subroutine changes R7
    AND R0, R0, #0          ; Reset R0
    
    ADD R4, R4, #0          ; Set CC
    BRn DONE_EXP            ; If R4 is negative, value will be 0
    
    ADD R0, R0, #1          ; Add starting value of R0
    
; Loop exp until R4 (TMP_COUNTER) becomes 0
LOOP_EXP
    LD R4, TMP_COUNTER      ; Load TMP_COUNTER into R4
    ADD R4, R4, #-1         ; Count down
    ST R4, TMP_COUNTER      ; Store it back into TMP_COUNTER
    BRn DONE_EXP            ; If CC from R4 + -1 is negative, fin loop
    
    ADD R4, R0, #0          ; Set R4 to R0 value (initial or from MUL)
    LD R3, R3_VAL           ; Load R3 value into R3 in case it changed
    JSR MUL                 ; Run MUL subroutine
    
    BR LOOP_EXP             ; Loop back to top
    
; What to do when EXP done looping
DONE_EXP
    LD R7, R7_VAL           ; Load the original R7 value
    RET                     ; Return to R7
;your code goes here
	
;IN:R0, OUT:R5 (0-success, 1-fail/overflow)
;R3: STACK_END R4: STACK_TOP
;
PUSH	
	ST R3, PUSH_SaveR3	;save R3
	ST R4, PUSH_SaveR4	;save R4
	AND R5, R5, #0		;
	LD R3, STACK_END	;
	LD R4, STACk_TOP	;
	ADD R3, R3, #-1		;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz OVERFLOW		;stack is full
	STR R0, R4, #0		;no overflow, store value in the stack
	ADD R4, R4, #-1		;move top of the stack
	ST R4, STACK_TOP	;store top of stack pointer
	BRnzp DONE_PUSH		;
OVERFLOW
	ADD R5, R5, #1		;
DONE_PUSH
	LD R3, PUSH_SaveR3	;
	LD R4, PUSH_SaveR4	;
	RET


PUSH_SaveR3	.BLKW #1	;
PUSH_SaveR4	.BLKW #1	;


;OUT: R0, OUT R5 (0-success, 1-fail/underflow)
;R3 STACK_START R4 STACK_TOP
;
POP	
	ST R3, POP_SaveR3	;save R3
	ST R4, POP_SaveR4	;save R3
	AND R5, R5, #0		;clear R5
	LD R3, STACK_START	;
	LD R4, STACK_TOP	;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz UNDERFLOW		;
	ADD R4, R4, #1		;
	LDR R0, R4, #0		;
	ST R4, STACK_TOP	;
	BRnzp DONE_POP		;
UNDERFLOW
	ADD R5, R5, #1		;
DONE_POP
	LD R3, POP_SaveR3	;
	LD R4, POP_SaveR4	;
	RET


POP_SaveR3	.BLKW #1	;
POP_SaveR4	.BLKW #1	;
STACK_END	.FILL x3FF0	;
STACK_START	.FILL x4000	;
STACK_TOP	.FILL x4000	;


.END
