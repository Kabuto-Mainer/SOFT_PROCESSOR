#ifndef COMAND_H
#define COMAND_H

const char NAME_ASM_FILE[] = "CODE-FILE/asm-code.asm";
const char NAME_BIN_FILE[] = "CODE-FILE/bin-code.bin";
const char NAME_TEXT_FILE[] = "CODE-FILE/listing.asm";
const char NAME_DISASMB_FILE[] = "CODE-FILE/disasmb-text.asm";

const char CHAR_CMD[30][6] = {
                        "PUSH",
                        "ADD",
                        "SUB",
                        "DIV",
                        "MUL",
                        "SQRT",
                        "OUT",
                        "HLT",
                        "POPR",
                        "PUSHR",
                        "IN",
                        "JMP",
                        "JB",
                        "JBE",
                        "JA",
                        "JAE",
                        "JE",
                        "JNE",
                        "HACK",
                        "CALL",
                        "RET",
                        "if",
                        "else",
                        "end"
};

enum INT_HASH {
    HASH_PUSH = 15743,
    HASH_ADD = 11816,
    HASH_SUB = 12905,
    HASH_DIV = 12674,
    HASH_MUL = 13037,
    HASH_SQRT = 16073,
    HASH_OUT = 13367,
    HASH_HLT = 12839,
    HASH_POPR = 15776,
    HASH_PUSHR = 18449,
    HASH_IN = 10166,
    HASH_JMP = 12806,
    HASH_JB = 9803,
    HASH_JBE = 12080,
    HASH_JA = 9770,
    HASH_JAE = 12047,
    HASH_JE = 9902,
    HASH_JNE = 12476,
    HASH_HACK = 14390,
    HASH_CALL = 14555,
    HASH_RET = 12938,
    HASH_if = 12014,
    HASH_else = 19208,
    HASH_end = 15446
};

enum INT_CMD {
    INT_PUSH = 0,
    INT_ADD = 1,
    INT_SUB = 2,
    INT_DIV = 3,
    INT_MUL = 4,
    INT_SQRT = 5,
    INT_OUT = 6,
    INT_HLT = 7,
    INT_POPR = 8,
    INT_PUSHR = 9,
    INT_IN = 10,
    INT_JMP = 11,
    INT_JB = 12,
    INT_JBE = 13,
    INT_JA = 14,
    INT_JAE = 15,
    INT_JE = 16,
    INT_JNE = 17,
    INT_HACK = 18,
    INT_CALL = 19,
    INT_RET = 20
};

enum INT_REG{
    AX = 0,
    BX = 1,
    CX = 2,
    DX = 3,
    EX = 4,
    FX = 5,
    GX = 6,
    HX = 7,
};

const char CHAR_REG[][4] = {
    "AX",
    "BX",
    "CX",
    "DX",
    "EX",
    "FX",
    "GX",
    "HX"
};
const int AMOUNT_POINTS = 8;

const int AMOUNT_CODS = 4;
const int AMOUNT_SUP_NUM = 4;

const int OWN_SIGNATURE = 1388;
const int CURRENT_VERSION = 3;

//
// const int INT_CODS[AMOUNT_CODS] = {
//     -13,
//     999,
//     41,
//     333,
//     2001,
//     1001
// };
//
// const char STR_CODS[AMOUNT_CODS][100] = {
//     "DEADFACE\n",
//     "Infinite amount roots\n",
//     "No reality root\n",
//     _P_ "  /\\_/\\\n ( o.o )\n  > ^ <\n" _N_,
//     "2 roots",
//     "1 roots"
// };
//
// ONE_CODE CODE_STR[AMOUNT_CODS] = {
//     {-13, "DEADFACE\n"},
//     {999, "Infinite amount roots\n"},
//     {41, "No reality roots\n"},
//     {333, _P_ "  /\\_/\\\n ( o.o )\n  > ^ <\n" _N_}
// };





#endif // COMAND_H
