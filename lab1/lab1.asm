.ORIG x3000
; write your code here
ST R3, NUM

GET_FOUR_HEX
    LD R2, COUNTER
    ADD R2, R2, #0
    BRz STOP
    ADD R2, R2, #-1
    ST R2, COUNTER
    AND R0, R0, #0              ; Set to 0
    AND R1, R1, #0
    AND R2, R2, #0
    ADD R2, R2, #4
    
    ; ADD R3, R3, #0
    ; BRn #1
    ; ADD R1, R1, #-1
    ; ADD R1, R1, #1
    
    BR GET_FOUR_BIT

GET_FOUR_BIT
    ADD R2, R2, #-1
    BRn #1
    BR SHIFT_LEFT
    BR GET_HEX
    
SHIFT_LEFT
    ADD R1, R1, R1
    ADD R3, R3, #0
    BRn #1
    ADD R1, R1, #-1
    ADD R1, R1, #1
    ADD R3, R3, R3
    BR GET_FOUR_BIT

GET_HEX
    ; AND R1, R1, #0
    ; ADD R1, R3, #0
    ST R1 PREV
    ADD R1, R1, #-10
    BRn NOT_ALPHA
    LD R0, ASCII_ALPHA
    ADD R0, R0, R1
    BRnzp PRINT
    
NOT_ALPHA
    LD R0, ASCII_NUMB
    LD R1, PREV
    ADD R0, R0, R1
    
PRINT
    OUT
    BR GET_FOUR_HEX

STOP HALT

COUNTER     .FILL #4
NUM         .FILL #0
PREV        .STRINGZ " "
ASCII_ALPHA .STRINGZ "A"
ASCII_NUMB  .STRINGZ "0"
.end
