IN
POPR AX
IN
POPR BX
IN
POPR CX ; _9

; Получил коэффициенты

PUSHR AX
OUT
PUSHR BX
OUT
PUSHR CX
OUT ; _18

; Вывел коэффициенты

PUSH 0
PUSHR AX
JE 197 ; _24

; ---- IF AX != 0

    PUSH 0
    PUSHR BX
    JE 145 ; _30

; ---- ---- IF BX != 0

        PUSH 0
        PUSHR CX
        JE 129 ; _36

; ---- ---- ---- IF CX != 0

            PUSHR BX
            PUSHR BX
            MUL ; _41

            PUSH -4
            PUSHR AX
            PUSHR CX
            MUL
            MUL
            ADD ; _50

            POPR DX ; _52

; Нашел дискриминант

            PUSH 0
            PUSHR DX
            JB 125 ; _58

; ---- ---- ---- ---- IF DX >= 0

                PUSH 0
                PUSHR DX
                JE 109 ; _64

; ---- ---- ---- ---- ---- IF DX != 0

                    PUSH 2
                    OUT
                    PUSH -1
                    PUSHR BX
                    MUL ; _72

                    PUSHR DX
                    SQRT
                    ADD ; _76

                    POPR EX
                    PUSH 2
                    PUSHR AX
                    MUL ; _83

                    PUSHR EX
                    DIV
                    OUT ; _87

                    PUSHR EX
                    PUSHR DX
                    SQRT
                    PUSH -2
                    MUL
                    ADD
                    POPR EX ; _98

                    PUSHR AX
                    PUSH 2
                    MUL
                    PUSHR EX
                    DIV
                    OUT ; _107

                    HLT ; _108

; ---- ---- ---- ---- ---- IF DX == 0

                    PUSH 1
                    OUT
                    PUSH 2
                    PUSHR AX
                    MUL ; _116

                    PUSH -1
                    PUSHR BX
                    MUL
                    DIV
                    OUT
                    HLT ; _124

; ---- ---- ---- ---- IF DX < 0

                PUSH 0
                OUT
                HLT ; _128

; ---- ---- ---- IF CX == 0

            PUSH 2
            OUT ; _131

            PUSH 0
            OUT ; _134

            PUSHR AX
            PUSH -1
            PUSHR BX
            MUL
            DIV
            OUT
            HLT ; _144

; ---- ---- IF BX == 0

        PUSHR AX
        PUSH -1
        PUSHR CX
        MUL
        DIV
        POPR DX ; _154

        PUSH 0
        PUSHR DX
        JB 193 ; _160

; ---- ---- ---- IF DX >= 0  "Не дискриминант"

            PUSH 0
            PUSHR DX
            JNE 175 ; _166

; ---- ---- ---- ---- IF DX == 0

                PUSH 1
                OUT
                PUSHR DX
                SQRT
                OUT
                HLT ; _174

; ---- ---- ---- ---- IF DX > 0

            PUSH 2
            OUT ; _177

            PUSHR DX
            SQRT
            POPR DX
            PUSHR DX
            OUT ; 185

            PUSHR DX
            PUSH -1
            MUL
            OUT
            HLT ; _192

; ---- ---- ---- IF DX < 0  "Не дискриминант"

            PUSH 0
            OUT
            HLT ; _196

; ---- IF AX == 0

    PUSH 0
    PUSHR BX
    JE 216 ; _202

; ---- ---- IF BX != 0

        PUSH 1
        OUT ; _205

        PUSHR BX
        PUSHR CX
        PUSH -1
        MUL
        DIV
        OUT
        HLT ; _215

; ---- ---- IF BX == 0

        PUSH 0
        PUSHR CX
        JE 229  ; _221

; ---- ---- ---- IF CX != 0

            PUSH -1
            OUT
            HLT ; _225

; ---- ---- ---- IF CX == 0

            PUSH 9999
            OUT
            HLT ; _229
