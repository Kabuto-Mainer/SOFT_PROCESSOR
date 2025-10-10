; Эта программа решает квадратные уравнения при любых целочисленных коэффициентах
; Сначала она выводит полученные коэффициенты для проверки правильности ввода
; Затем она выводит количество решений: 9999, 2, 1, 0
; (P.S.) 9999 - бесконечное количество корней
; Затем она выводит сами корни
;--------------------------------------------------------------------------------------


; Функция получения коэффициентов (0)
IN
POPR AX
IN
POPR BX
IN
POPR CX


; Вывода коэффициентов (10)
PUSHR AX
OUT
PUSHR BX
OUT
PUSHR CX
OUT


; Само решение квадратки (19)
; ---- A(AX) ? 0

PUSH 0
PUSHR AX
JE 210

; ---- IF AX != 0

; ---- ---- B(BX) ? 0

    PUSH 0
    PUSHR BX
    JE 150

; ---- ---- BX != 0

; ---- ---- ---- C(CX) ? 0

        PUSH 0
        PUSHR CX
        JE 130

; ---- ---- ---- CX != 0

            PUSHR BX
            PUSHR BX
            MUL
            PUSH -4
            PUSHR AX
            PUSHR CX
            MUL
            MUL
            ADD
            POPR DX ; Нашли D

; ---- ----- ---- ---- D(DX) ? 0

                PUSH 0
                PUSHR DX
                JB 124

; ---- ---- ---- ---- DX >= 0

; ---- ---- ---- ---- ---- D(DX) ? 0

                    PUSH 0
                    PUSHR DX
                    JNE 84

; ---- ---- ---- ---- ---- DX == 0

                        PUSH 2
                        PUSHR AX
                        MUL
                        PUSHR BX
                        PUSH -1
                        MUL
                        DIV

                        PUSH 1 ; Количество корней
                        POPR EX
                        POPR FX
                        JMP :5

; ---- ---- ---- ---- ---- DX > 0

                        PUSH 2 ; Количество корней
                        POPR EX

                        PUSH 2
                        PUSHR AX
                        MUL
                        PUSHR DX
                        SQRT
                        PUSH -1
                        PUSHR BX
                        MUL
                        ADD
                        DIV
                        POPR FX

                        PUSH 2
                        PUSHR AX
                        MUL
                        PUSHR DX
                        SQRT
                        PUSH -1
                        PUSHR BX
                        MUL
                        SUB
                        DIV
                        POPR GX

                        JMP :5

; ---- ---- ---- ---- DX < 0

                    PUSH 0 ; Количество корней
                    POPR EX
                    JMP :5

; ---- ---- ---- CX == 0

                PUSH 2 ; Количество корней
                POPR EX

                PUSH 0
                POPR FX

                PUSHR AX
                PUSH -1
                PUSHR BX
                MUL
                DIV
                POPR GX

                JMP :5

; ---- ---- BX == 0

            PUSHR AX
            PUSHR CX
            PUSH -1
            MUL
            DIV
            POPR HX

; ---- ---- ---- -C/A(HX) ? 0

                PUSH 0
                PUSHR HX
                JB 204

; ---- ---- ---- HX >= 0

; ---- ---- ---- ---- -C/A(HX) ? 0

                    PUSH 0
                    PUSHR HX
                    JE 194

; ---- ---- ---- ---- HX > 0

                        PUSH 2 ; Количество корней
                        POPR EX

                        PUSHR HX
                        SQRT
                        POPR HX
                        PUSHR HX
                        POPR FX
                        PUSHR HX
                        PUSH -1
                        MUL
                        POPR GX

                        JMP :5

; ---- ---- ---- ---- HX == 0

                        PUSH 1
                        POPR EX

                        PUSH 0
                        POPR FX


                        JMP :5

; ---- ---- ---- HX < 0

                    PUSH 0 ; Количество корней
                    POPR EX

                    JMP :5

; ---- AX == 0

; ---- ---- B(BX) ? 0

            PUSH 0
            PUSHR BX
            JE 232

; ---- ---- BX != 0

            PUSH 1 ; Количество корней
            POPR EX

            PUSHR BX
            PUSH -1
            PUSHR CX
            MUL
            DIV
            POPR FX

            JMP :5
; ---- ---- BX == 0

; ---- ---- ---- C(CX) ? 0

            PUSH 0
            PUSHR CX
            JE 244

; ---- ---- ---- CX != 0

            PUSH 0
            POPR EX

            JMP :5

; ---- ---- ---- CX == 0

            PUSH 9999
            POPR EX

            JMP :5


; Функция вывода корней (244)

:5
PUSHR EX
OUT

; Amount_roots(EX)

PUSH 2
PUSHR EX
JNE 266

; EX == 2

    PUSHR FX
    OUT
    PUSHR GX
    OUT
    HLT

PUSH 1
PUSHR EX
JNE 275

; EX == 1

    PUSHR FX
    OUT
    HLT

HLT

