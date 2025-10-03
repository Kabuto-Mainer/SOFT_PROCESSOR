# Это программа решения квадратного уравнения
# A, B, C - пока задаются только через compiler_name_table
#
# -B
PUSH 2
PUSH $A
MUL
# Вычисление D
PUSH 4
PUSH $A
PUSH $C
MUL
MUL
PUSH $B
PUSH $B
MUL
SUB
SQRT
# Нашли корень из D
# Считаем -B
PUSH 0
PUSH $B
SUB
SUB
# Нашли числитель
DIV
OUT
# Нашли первый корень
PUSH 2
PUSH $A
MUL
PUSH 4
PUSH $A
PUSH $C
MUL
MUL
PUSH $B
PUSH $B
MUL
SUB
SQRT
PUSH 0
PUSH $B
SUB
ADD
DIV
OUT
HLT
