; --- Program: Fibonacci Sequence ---
; Calculate 8th Fib number (0, 1, 1, 2, 3, 5, 8, 13, 21...)
; Mem[0] = a (current), Mem[1] = b (next), Mem[2] = count

PUSH 0      ;
STORE 0     ; a = 0
PUSH 1      ;
STORE 1     ; b = 1
PUSH 8      ; Calculate 8th number
STORE 2     ; count = 8

loop:
    LOAD 2      ;
    JZ done     ; if count == 0, done

    ; temp = a + b
    LOAD 0      ;
    LOAD 1      ;
    ADD         ;
    ; a = b
    LOAD 1      ;
    STORE 0     ;
    ; b = temp
    STORE 1     ; Store (a+b) into b

    ; count--
    LOAD 2      ;
    PUSH 1      ;
    SUB         ;
    STORE 2     ;
    
    JMP loop    ;

done:
    LOAD 0      ; Result is in 'a'
    HALT        ; Expected Stack: [ 21 ]
