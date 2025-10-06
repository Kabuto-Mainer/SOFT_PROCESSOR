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
JE 160 ; _24

; ---- IF AX != 0

PUSH 0
PUSHR BX
JE 138 ; _30

; ---- ---- IF BX != 0

PUSH 0
PUSHR CX
JE 122 ; _36

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
JB 121 ; _58

; ---- ---- ---- ---- IF DX >= 0

PUSH 0
PUSHR DX
JE 105 ; _64

; ---- ---- ---- ---- ---- IF DX != 0

PUSH -1
PUSHR BX
MUL ; _69

PUSHR DX
SQRT
ADD ; _73

POPR EX
PUSH 2
PUSHR AX
MUL ; _80

PUSHR EX
DIV
OUT ; _84

PUSHR EX
PUSHR DX
SQRT
PUSH -2
MUL
ADD
POPR EX ; _95

PUSHR AX
PUSH 2
MUL
PUSHR EX
DIV
OUT ; _104

HLT ; _105

; ---- ---- ---- ---- ---- IF DX == 0

PUSH 2
PUSHR AX
MUL ; _110

PUSH -1
PUSHR BX
MUL
DIV
OUT
HLT ; _118

; ---- ---- ---- ---- IF DX < 0

PUSH -9137
OUT
HLT ; _122

; ---- ---- ---- IF CX == 0

PUSH 0
OUT ; _125

PUSHR AX
PUSH -1
PUSHR BX
MUL
DIV
OUT
HLT ; _135

; ---- ---- IF BX == 0

PUSHR AX
PUSH -1
PUSHR CX
MUL
DIV
POPR DX ; _145

PUSH 0
PUSHR DX
JB 156 ; _151

; ---- ---- ---- IF DX >= 0  "Не дискриминант"

PUSHR DX
SQRT
OUT
HLT ; _156

; ---- ---- ---- IF DX < 0  "Не дискриминант"

PUSH -9137
OUT
HLT ; _160

; --- IF AX == 0

PUSH -1234
OUT
HLT ; _164

; Дописать !!!
