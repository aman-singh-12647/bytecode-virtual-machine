; --- Program: Iterative Factorial ---
; Logic:
; result = 1
; counter = 5
; while (counter > 0) {
;    result = result * counter
;    counter--
; }

PUSH 1        ;
STORE 0       ; Mem[0] (result) = 1

PUSH 5        ;
STORE 1       ; Mem[1] (counter) = 5

loop_start:
    LOAD 1        ; Load counter
    JZ end        ; If counter == 0, jump to end

    ; Perform result = result * counter
    LOAD 0        ; Load result
    LOAD 1        ; Load counter
    MUL           ; Multiply
    STORE 0       ; Store back to result

    ; Decrement counter
    LOAD 1        ; Load counter
    PUSH 1        ;
    SUB           ; Subtract 1
    STORE 1       ; Store new counter

    JMP loop_start;

end:
    LOAD 0        ; Load final result to top of stack
    HALT          ; Expected Stack: [ 120 ]
