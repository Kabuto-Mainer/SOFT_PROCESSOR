[  1] IN                                                           10
[  2] POPR AX                                                      8 0
[  4] IN                                                           10
[  5] POPR BX                                                      8 1
[  7] IN                                                           10
[  8] POPR CX                                                      8 2
[ 10] PUSHR AX                                                     9 0
[ 12] OUT                                                          6
[ 13] PUSHR BX                                                     9 1
[ 15] OUT                                                          6
[ 16] PUSHR CX                                                     9 2
[ 18] OUT                                                          6
[ 19] PUSH 0                                                       0 0
[ 21] PUSHR AX                                                     9 0
[ 23] JE 210                                                       16 210
[ 25]     PUSH 0                                                   0 0
[ 27]     PUSHR BX                                                 9 1
[ 29]     JE 150                                                   16 150
[ 31]         PUSH 0                                               0 0
[ 33]         PUSHR CX                                             9 2
[ 35]         JE 130                                               16 130
[ 37]             PUSHR BX                                         9 1
[ 39]             PUSHR BX                                         9 1
[ 41]             MUL                                              4
[ 42]             PUSH -4                                          0 -4
[ 44]             PUSHR AX                                         9 0
[ 46]             PUSHR CX                                         9 2
[ 48]             MUL                                              4
[ 49]             MUL                                              4
[ 50]             ADD                                              1
[ 51]             POPR DX ; Нашли D                           8 3
[ 53]                 PUSH 0                                       0 0
[ 55]                 PUSHR DX                                     9 3
[ 57]                 JB 124                                       12 124
[ 59]                     PUSH 0                                   0 0
[ 61]                     PUSHR DX                                 9 3
[ 63]                     JNE 84                                   17 84
[ 65]                         PUSH 2                               0 2
[ 67]                         PUSHR AX                             9 0
[ 69]                         MUL                                  4
[ 70]                         PUSHR BX                             9 1
[ 72]                         PUSH -1                              0 -1
[ 74]                         MUL                                  4
[ 75]                         DIV                                  3
[ 76]                         PUSH 1 ; Количество корней 0 1
[ 78]                         POPR EX                              8 4
[ 80]                         POPR FX                              8 5
[ 82]                         JMP :5                               11 250
[ 84]                         PUSH 2 ; Количество корней 0 2
[ 86]                         POPR EX                              8 4
[ 88]                         PUSH 2                               0 2
[ 90]                         PUSHR AX                             9 0
[ 92]                         MUL                                  4
[ 93]                         PUSHR DX                             9 3
[ 95]                         SQRT                                 5
[ 96]                         PUSH -1                              0 -1
[ 98]                         PUSHR BX                             9 1
[100]                         MUL                                  4
[101]                         ADD                                  1
[102]                         DIV                                  3
[103]                         POPR FX                              8 5
[105]                         PUSH 2                               0 2
[107]                         PUSHR AX                             9 0
[109]                         MUL                                  4
[110]                         PUSHR DX                             9 3
[112]                         SQRT                                 5
[113]                         PUSH -1                              0 -1
[115]                         PUSHR BX                             9 1
[117]                         MUL                                  4
[118]                         SUB                                  2
[119]                         DIV                                  3
[120]                         POPR GX                              8 6
[122]                         JMP :5                               11 250
[124]                     PUSH 0 ; Количество корней 0 0
[126]                     POPR EX                                  8 4
[128]                     JMP :5                                   11 250
[130]                 PUSH 2 ; Количество корней   0 2
[132]                 POPR EX                                      8 4
[134]                 PUSH 0                                       0 0
[136]                 POPR FX                                      8 5
[138]                 PUSHR AX                                     9 0
[140]                 PUSH -1                                      0 -1
[142]                 PUSHR BX                                     9 1
[144]                 MUL                                          4
[145]                 DIV                                          3
[146]                 POPR GX                                      8 6
[148]                 JMP :5                                       11 250
[150]             PUSHR AX                                         9 0
[152]             PUSHR CX                                         9 2
[154]             PUSH -1                                          0 -1
[156]             MUL                                              4
[157]             DIV                                              3
[158]             POPR HX                                          8 7
[160]                 PUSH 0                                       0 0
[162]                 PUSHR HX                                     9 7
[164]                 JB 204                                       12 204
[166]                     PUSH 0                                   0 0
[168]                     PUSHR HX                                 9 7
[170]                     JE 194                                   16 194
[172]                         PUSH 2 ; Количество корней 0 2
[174]                         POPR EX                              8 4
[176]                         PUSHR HX                             9 7
[178]                         SQRT                                 5
[179]                         POPR HX                              8 7
[181]                         PUSHR HX                             9 7
[183]                         POPR FX                              8 5
[185]                         PUSHR HX                             9 7
[187]                         PUSH -1                              0 -1
[189]                         MUL                                  4
[190]                         POPR GX                              8 6
[192]                         JMP :5                               11 250
[194]                         PUSH 1                               0 1
[196]                         POPR EX                              8 4
[198]                         PUSH 0                               0 0
[200]                         POPR FX                              8 5
[202]                         JMP :5                               11 250
[204]                     PUSH 0 ; Количество корней 0 0
[206]                     POPR EX                                  8 4
[208]                     JMP :5                                   11 250
[210]             PUSH 0                                           0 0
[212]             PUSHR BX                                         9 1
[214]             JE 232                                           16 232
[216]             PUSH 1 ; Количество корней       0 1
[218]             POPR EX                                          8 4
[220]             PUSHR BX                                         9 1
[222]             PUSH -1                                          0 -1
[224]             PUSHR CX                                         9 2
[226]             MUL                                              4
[227]             DIV                                              3
[228]             POPR FX                                          8 5
[230]             JMP :5                                           11 250
[232]             PUSH 0                                           0 0
[234]             PUSHR CX                                         9 2
[236]             JE 244                                           16 244
[238]             PUSH 0                                           0 0
[240]             POPR EX                                          8 4
[242]             JMP :5                                           11 250
[244]             PUSH 9999                                        0 9999
[246]             POPR EX                                          8 4
[248]             JMP :5                                           11 250
[250] :5                                                           
[250] PUSHR EX                                                     9 4
[252] OUT                                                          6
[253] PUSH 2                                                       0 2
[255] PUSHR EX                                                     9 4
[257] JNE 266                                                      17 266
[259]     PUSHR FX                                                 9 5
[261]     OUT                                                      6
[262]     PUSHR GX                                                 9 6
[264]     OUT                                                      6
[265]     HLT                                                      7
[266] PUSH 1                                                       0 1
[268] PUSHR EX                                                     9 4
[270] JNE 275                                                      17 275
[272]     PUSHR FX                                                 9 5
[274]     OUT                                                      6
[275]     HLT                                                      7
[276] HLT                                                          7
