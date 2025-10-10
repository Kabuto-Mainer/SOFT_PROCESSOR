IN
POPR AX

CALL :4
OUT
HLT


:4

PUSHR AX

PUSH 1
PUSHR AX
JE $+19

    PUSHR AX
    PUSH -1
    ADD
    POPR AX

    CALL :4

    PUSHR AX
    PUSH 1
    ADD
    POPR AX

    MUL
    RET

RET













