; Calculate Area of Circle A = pi * r^2
; r = 5, pi = 3

PUSH 5      ; Push radius
DUP         ; Stack: 5, 5
MUL         ; Stack: 25
PUSH 3      ; Push pi
MUL         ; Stack: 75
HALT        ;
