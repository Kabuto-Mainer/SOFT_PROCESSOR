[  1] IN                                      10
[  2] POPR AX                                 8 0
[  4] IN                                      10
[  5] POPR BX                                 8 1
[  7] IN                                      10
[  8] POPR CX ; _9                            8 2
[ 10] PUSHR AX                                9 0
[ 12] OUT                                     6
[ 13] PUSHR BX                                9 1
[ 15] OUT                                     6
[ 16] PUSHR CX                                9 2
[ 18] OUT ; _18                               6
[ 19] PUSH 0                                  0 0
[ 21] PUSHR AX                                9 0
[ 23] JE 197 ; _24                            16 197
[ 25]     PUSH 0                              0 0
[ 27]     PUSHR BX                            9 1
[ 29]     JE 145 ; _30                        16 145
[ 31]         PUSH 0                          0 0
[ 33]         PUSHR CX                        9 2
[ 35]         JE 129 ; _36                    16 129
[ 37]             PUSHR BX                    9 1
[ 39]             PUSHR BX                    9 1
[ 41]             MUL ; _41                   4
[ 42]             PUSH -4                     0 -4
[ 44]             PUSHR AX                    9 0
[ 46]             PUSHR CX                    9 2
[ 48]             MUL                         4
[ 49]             MUL                         4
[ 50]             ADD ; _50                   1
[ 51]             POPR DX ; _52               8 3
[ 53]             PUSH 0                      0 0
[ 55]             PUSHR DX                    9 3
[ 57]             JB 125 ; _58                12 125
[ 59]                 PUSH 0                  0 0
[ 61]                 PUSHR DX                9 3
[ 63]                 JE 109 ; _64            16 109
[ 65]                     PUSH 2              0 2
[ 67]                     OUT                 6
[ 68]                     PUSH -1             0 -1
[ 70]                     PUSHR BX            9 1
[ 72]                     MUL ; _72           4
[ 73]                     PUSHR DX            9 3
[ 75]                     SQRT                5
[ 76]                     ADD ; _76           1
[ 77]                     POPR EX             8 4
[ 79]                     PUSH 2              0 2
[ 81]                     PUSHR AX            9 0
[ 83]                     MUL ; _83           4
[ 84]                     PUSHR EX            9 4
[ 86]                     DIV                 3
[ 87]                     OUT ; _87           6
[ 88]                     PUSHR EX            9 4
[ 90]                     PUSHR DX            9 3
[ 92]                     SQRT                5
[ 93]                     PUSH -2             0 -2
[ 95]                     MUL                 4
[ 96]                     ADD                 1
[ 97]                     POPR EX ; _98       8 4
[ 99]                     PUSHR AX            9 0
[101]                     PUSH 2              0 2
[103]                     MUL                 4
[104]                     PUSHR EX            9 4
[106]                     DIV                 3
[107]                     OUT ; _107          6
[108]                     HLT ; _108          7
[109]                     PUSH 1              0 1
[111]                     OUT                 6
[112]                     PUSH 2              0 2
[114]                     PUSHR AX            9 0
[116]                     MUL ; _116          4
[117]                     PUSH -1             0 -1
[119]                     PUSHR BX            9 1
[121]                     MUL                 4
[122]                     DIV                 3
[123]                     OUT                 6
[124]                     HLT ; _124          7
[125]                 PUSH 0                  0 0
[127]                 OUT                     6
[128]                 HLT ; _128              7
[129]             PUSH 2                      0 2
[131]             OUT ; _131                  6
[132]             PUSH 0                      0 0
[134]             OUT ; _134                  6
[135]             PUSHR AX                    9 0
[137]             PUSH -1                     0 -1
[139]             PUSHR BX                    9 1
[141]             MUL                         4
[142]             DIV                         3
[143]             OUT                         6
[144]             HLT ; _144                  7
[145]         PUSHR AX                        9 0
[147]         PUSH -1                         0 -1
[149]         PUSHR CX                        9 2
[151]         MUL                             4
[152]         DIV                             3
[153]         POPR DX ; _154                  8 3
[155]         PUSH 0                          0 0
[157]         PUSHR DX                        9 3
[159]         JB 193 ; _160                   12 193
[161]             PUSH 0                      0 0
[163]             PUSHR DX                    9 3
[165]             JNE 175 ; _166              17 175
[167]                 PUSH 1                  0 1
[169]                 OUT                     6
[170]                 PUSHR DX                9 3
[172]                 SQRT                    5
[173]                 OUT                     6
[174]                 HLT ; _174              7
[175]             PUSH 2                      0 2
[177]             OUT ; _177                  6
[178]             PUSHR DX                    9 3
[180]             SQRT                        5
[181]             POPR DX                     8 3
[183]             PUSHR DX                    9 3
[185]             OUT ; 185                   6
[186]             PUSHR DX                    9 3
[188]             PUSH -1                     0 -1
[190]             MUL                         4
[191]             OUT                         6
[192]             HLT ; _192                  7
[193]             PUSH 0                      0 0
[195]             OUT                         6
[196]             HLT ; _196                  7
[197]     PUSH 0                              0 0
[199]     PUSHR BX                            9 1
[201]     JE 216 ; _202                       16 216
[203]         PUSH 1                          0 1
[205]         OUT ; _205                      6
[206]         PUSHR BX                        9 1
[208]         PUSHR CX                        9 2
[210]         PUSH -1                         0 -1
[212]         MUL                             4
[213]         DIV                             3
[214]         OUT                             6
[215]         HLT ; _215                      7
[216]         PUSH 0                          0 0
[218]         PUSHR CX                        9 2
[220]         JE 229  ; _221                  16 229
[222]             PUSH -1                     0 -1
[224]             OUT                         6
[225]             HLT ; _225                  7
[226]             PUSH 9999                   0 9999
[228]             OUT                         6
[229]             HLT ; _229                  7
