; --- Program: Nested Calls ---
; Main -> Calls 'add_and_double(10, 20)' -> Calls 'double(30)' -> Returns 60

PUSH 10         ;
PUSH 20         ;
CALL add_and_double;
HALT            ;

; Function: double(x)
; Input: Top of stack
; Output: x * 2
double:
    PUSH 2      ;
    MUL         ;
    RET         ;

; Function: add_and_double(a, b)
; Input: Stack [a, b]
; Output: (a + b) * 2
add_and_double:
    ADD         ; Stack: [30]
    CALL double ; Call nested function
    RET         ; Return to main
