[  1] IN                                                           10
[  2] POPR AX                                                      8 0
[  4] CALL :4                                                      19 8
[  6] OUT                                                          6
[  7] HLT                                                          7
[  8] :4                                                           
[  8] PUSHR AX                                                     9 0
[ 10] PUSH 1                                                       0 1
[ 12] PUSHR AX                                                     9 0
[ 14] JE $+19                                                      16 34
[ 16]     PUSHR AX                                                 9 0
[ 18]     PUSH -1                                                  0 -1
[ 20]     ADD                                                      1
[ 21]     POPR AX                                                  8 0
[ 23]     CALL :4                                                  19 8
[ 25]     PUSHR AX                                                 9 0
[ 27]     PUSH 1                                                   0 1
[ 29]     ADD                                                      1
[ 30]     POPR AX                                                  8 0
[ 32]     MUL                                                      4
[ 33]     RET                                                      20
[ 34] RET                                                          20
