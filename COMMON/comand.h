#ifndef COMAND_H
#define COMAND_H

const char NAME_ASM_FILE[] = "CODE-FILE/quad.asm";
const char NAME_BIN_FILE[] = "CODE-FILE/bin-code.bin";
const char NAME_TEXT_FILE[] = "CODE-FILE/text-code.txt";
const char NAME_DISASMB_FILE[] = "CODE-FILE/disasmb-text.asm";

const char STR_MASS_COMANDS[10][6] = {
                        "PUSH",
                        "ADD",
                        "SUB",
                        "DIV",
                        "MUL",
                        "SQRT",
                        "OUT",
                        "HLT"
};

enum INT_COMANDS{
    INT_PUSH = 0,
    INT_ADD = 1,
    INT_SUB = 2,
    INT_DIV = 3,
    INT_MUL = 4,
    INT_SQRT = 5,
    INT_OUT = 6,
    INT_HLT = 7
};

const int AMOUNT_SUP_NUM = 4;

const int OWN_SIGNATURE = 1388;
const int CURRENT_VERSION = 2;

#endif // COMAND_H
