; Benchmark: Tight Loop
; Counts down from 10,000,000 to 0

PUSH 10000000  ; Counter
loop_start:
    PUSH 1
    SUB        ; Decrement
    DUP
    JNZ loop_start
    HALT
