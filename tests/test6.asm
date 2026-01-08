; Test Invalid Jump Target
; Try to jump to a memory address that is outside the bytecode range.

PUSH 1
JMP 10000 ; Jump to an address well outside reasonable bounds (assuming small test file)
HALT
