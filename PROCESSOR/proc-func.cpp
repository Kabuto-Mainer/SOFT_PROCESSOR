#include <stdio.h>
#include <assert.h>
#include <cstdlib>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#include "stack_define.h"
#include "processor.h"
#include "stack.h"
#include "proc-func.h"

#include "../COMMON/support.h"
#include "../COMMON/comand.h"
#include "../COMMON/color.h"

// -------------------------------------------------------------------------------------------------------
int math_func(cpu_t* proc, int sign, int trash_2)
{
    (void) trash_2;

    assert(proc);

    int arg_1 = 0;
    int arg_2 = 0;

    stack_pop(&(proc->stack), &arg_1);
    stack_pop(&(proc->stack), &arg_2);
    switch (sign)
    {
        case '+': // '+'
        {
            stack_push(&(proc->stack), arg_1 + arg_2);
            break;
        }

        case '-': // '-'
        {
            stack_push(&(proc->stack), arg_1 - arg_2);
            break;
        }

        case '*': // '*'
        {
            stack_push(&(proc->stack), arg_1 * arg_2);
            break;
        }

        case '/': // '/'
        {
            if (arg_2 == 0) {
                printf(_R_ "ERROR: division by zero is not correct\n" _N_);
                return P_ERROR;
            }
            stack_push(&(proc->stack), arg_1 / arg_2);
            break;
        }

        default:
        {
            printf("ERROR: default math\n");
            return P_ERROR;
        }
    }
    return P_OK;
}
// -------------------------------------------------------------------------------------------------------
int j_func(cpu_t* proc, int type_jmp, int trash_2)
{
    (void) trash_2;

    assert(proc);

    if (type_jmp == OPER_JMP)
    {
        proc->C_E++;
        proc->C_E = proc->bin_code[proc->C_E] - 2;
        return P_OK;
    }

    int arg_1 = 0;
    int arg_2 = 0;

    stack_pop(&(proc->stack), &arg_1);
    stack_pop(&(proc->stack), &arg_2);

    proc->C_E++;
    switch (type_jmp)
    {
        case OPER_JB: // '<'
        {
            if (arg_1 < arg_2) {
                proc->C_E = proc->bin_code[proc->C_E] - 2; \
            }
            return P_OK;
        }

        case OPER_JBE: // '<='
        {
            if (arg_1 <= arg_2) {
                proc->C_E = proc->bin_code[proc->C_E] - 2; \
            }
            return P_OK;
        }

        case OPER_JA: // '>'
        {
            if (arg_1 > arg_2) {
                proc->C_E = proc->bin_code[proc->C_E] - 2; \
            }
            return P_OK;
        }

        case OPER_JAE: // '>='
        {
            if (arg_1 >= arg_2) {
                proc->C_E = proc->bin_code[proc->C_E] - 2; \
            }
            return P_OK;
        }

        case OPER_JE: // '=='
        {
            if (arg_1 == arg_2) {
                proc->C_E = proc->bin_code[proc->C_E] - 2; \
            }
            return P_OK;
        }

        case OPER_JNE: // '!='
        {
            if (arg_1 <= arg_2) {
                proc->C_E = proc->bin_code[proc->C_E] - 2; \
            }
            return P_OK;
        }

        default:
        {
            printf("ERROR: j_func default\n");
            return P_ERROR;
        }
    }
    return P_ERROR;
}
// -------------------------------------------------------------------------------------------------------
int reg_func(cpu_t* proc, int type_use, int type_data)
{
    assert(proc);

    if (type_data == RAW_TYPE)
    {
        if (type_use == PUSH_TYPE)
        {
            int arg = proc->RAM[proc->regs[proc->bin_code[++(proc->C_E)]]];
            stack_push(&(proc->stack), arg);

            return P_OK;
        }

        else
        {
            int arg = 0;
            stack_pop(&(proc->stack), &arg);
            proc->RAM[proc->regs[proc->bin_code[++proc->C_E]]] = arg;

            return P_OK;
        }
    }

    else
    {
        if (type_use == PUSH_TYPE)
        {
            int reg = proc->bin_code[++proc->C_E];
            int num = proc->regs[reg];

            stack_push(&(proc->stack), num);
            return P_OK;
        }

        else{
            int reg = proc->bin_code[++proc->C_E];
            int num = 0;

            stack_pop(&(proc->stack), &num);
            proc->regs[reg] = num;

            return P_OK;
        }
    }
    return P_ERROR;
}
// -------------------------------------------------------------------------------------------------------
int in_out_func(cpu_t* proc, int type_use, int trash_2)
{
    (void) trash_2;

    assert(proc);

    if (type_use == IN_TYPE)
    {
        int num = 0;

        printf(_G_ "Enter int num: " _N_);
        scanf("%d", &num);

        stack_push(&(proc->stack), num);
        return P_OK;
    }

    else
    {
        int arg = 0;

        stack_pop(&(proc->stack), &arg);
        printf("%d\n", arg);
        return P_OK;
    }
    return P_ERROR;
}
// -------------------------------------------------------------------------------------------------------
int push_func(cpu_t* proc, int trash_1, int trash_2)
{
    (void) trash_1;
    (void) trash_2;

    assert(proc);

    proc->C_E++;
    stack_push(&(proc->stack), proc->bin_code[proc->C_E]);
    return P_OK;
}
// -------------------------------------------------------------------------------------------------------
int vraw_func(cpu_t* proc, int type_use, int trash_2)
{
    (void) trash_2;

    assert(proc);

    if (type_use == OUT_TYPE)
    {
        int x = 0;
        int y = 0;

        stack_pop(&(proc->stack), &y);
        stack_pop(&(proc->stack), &x);

        int address = y * proc->disp_set.len + x;
        int color = proc->bin_code[++proc->C_E];

        (proc->VRAM)[address] = color;
        return P_OK;
    }

    else
    {
        int x = 0;
        int y = 0;

        stack_pop(&(proc->stack), &y);
        stack_pop(&(proc->stack), &x);

        int address = y * proc->disp_set.len + x;
        int color = (proc->VRAM)[address];

        stack_push(&(proc->stack), color);

        return P_OK;
    }
    return P_ERROR;
}
// -------------------------------------------------------------------------------------------------------
int time_func(cpu_t* proc, int trash_1, int trash_2)
{
    (void) trash_1;
    (void) trash_2;

    assert(proc);

    struct timeval tv;
    gettimeofday(&tv, NULL);

    long long milisec = ((long long)tv.tv_sec * 1000 + tv.tv_usec / 1000) % TIME_DIV;
    stack_push(&(proc->stack), (int) milisec);

    return P_OK;
}
// -------------------------------------------------------------------------------------------------------
int hlt_func(cpu_t* proc, int trash_1, int trash_2)
{
    (void) trash_1;
    (void) trash_2;

    assert(proc);
    return P_END;
}
// -------------------------------------------------------------------------------------------------------
int sqrt_func(cpu_t* proc, int trash_1, int trash_2)
{
    (void) trash_1;
    (void) trash_2;

    assert(proc);

    int arg = 0;
    stack_pop(&(proc->stack), &arg);

    if (arg < 0)
    {
        printf(_R_ "ERROR: sqrt in negative num is not correct\n" _N_);
        return P_ERROR;
    }
    stack_push(&(proc->stack), (int) sqrt(arg));
    return P_OK;
}
// -------------------------------------------------------------------------------------------------------
int draw_func(cpu_t* proc, int trash_1, int trash_2)
{
    (void) trash_1;
    (void) trash_2;

    assert(proc);

    system("clear");
    for (int i = 0; i < proc->disp_set.size; i++) {
        putchar(proc->VRAM[i]);

        if (i % proc->disp_set.len == 0) {
            putchar('\n');
        }
    }
    return P_OK;
}
// -------------------------------------------------------------------------------------------------------
int func_func(cpu_t* proc, int type_use, int trash_2)
{
    (void) trash_2;
    assert(proc);

    if (type_use == CALL_TYPE)
    {
        proc->C_E++;
        stack_push(&(proc->address), proc->C_E + 1);
        proc->C_E = proc->bin_code[proc->C_E] - 2;

        return P_OK;
    }

    else
    {
        int adr = 0;
        stack_pop(&(proc->address), &adr);
        proc->C_E = adr - 1;

        return P_OK;
    }
    return P_ERROR;
}
// -------------------------------------------------------------------------------------------------------
