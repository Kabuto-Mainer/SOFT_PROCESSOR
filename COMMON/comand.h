#ifndef COMAND_H
#define COMAND_H

const char NAME_ASM_FILE[] = "CODE-FILE/asm-code.asm";
const char NAME_HASH_FILE[] = "COMMON/cmd-hash.h";
const char NAME_BIN_FILE[] = "CODE-FILE/bin-code.bin";
const char NAME_TEXT_FILE[] = "CODE-FILE/listing.asm";
const char NAME_DISASMB_FILE[] = "CODE-FILE/disasmb-text.asm";

const char CHAR_CMD[][7] = {
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
                        "PUSHM",
                        "POPM",
                        "PAINT",
                        "COLOR",
                        "DRAW",
                        "CTIME"
};
// -------------------------------------------------------------------------------------------------------
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
    INT_RET = 20,
    INT_PUSHM = 21,
    INT_POPM = 22,
    INT_PAINT = 23,
    INT_COLOR = 24,
    INT_DRAW = 25,
    INT_CTIME = 26
};
// -------------------------------------------------------------------------------------------------------
enum INT_REG{
    AX = 0,
    BX = 1,
    CX = 2,
    DX = 3,
    EX = 4,
    FX = 5,
    GX = 6,
    HX = 7,
    R1 = 8,
    R2 = 9,
    R3 = 10,
    R4 = 11
};
// -------------------------------------------------------------------------------------------------------
const char CHAR_REG[][4] = {
    "AX",
    "BX",
    "CX",
    "DX",
    "EX",
    "FX",
    "GX",
    "HX",
    "R1",
    "R2",
    "R3",
    "R4"
};
// -------------------------------------------------------------------------------------------------------




#endif // COMAND_H
