; Test Undefined Label
; Try to usage a label that has not been defined.

PUSH 5
JMP NON_EXISTENT_LABEL
HALT
