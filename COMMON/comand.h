#ifndef COMAND_H
#define COMAND_H

const char NAME_ASM_FILE[] = "TEXT_FILE/asm_code.asm";
const char NAME_BIN_FILE[] = "TEXT_FILE/bin_code.bin";
const char NAME_TEXT_FILE[] = "TEXT_FILE/text_code.txt";
const char NAME_DISASMB_FILE[] = "TEXT_FILE/disasmb_text.asm";

const char STR_MASS_COMANDS[10][6] = {
                        "PUSH",
                        "ADD",
                        "SUB",
                        "DIV",
                        "MUL",
                        "OUT",
                        "HLT"
};

enum INT_COMANDS{
    INT_PUSH = 0,
    INT_ADD = 1,
    INT_SUB = 2,
    INT_DIV = 3,
    INT_MUL = 4,
    INT_OUT = 5,
    INT_HLT = 6
};


#endif // COMAND_H
