#ifndef _define_h
#define _define_h
#include "mem.h"
#include "elf/elf.h"
#include "elf/syms.h"

#define INPUT_SIZE 1024
#define PROMPT_STRING "EmulMips : > "


extern int32_t reg_mips[35];
extern mem memory;
extern stab symtab;
extern int scriptmode;



typedef enum {
    LOAD,
    EXIT,
    DISP,
    DISASM,
    SET,
    ASSERT,
    DEBUG,
    RESUME,
    RUN,
    STEP,
    BREAK,
    UNKNOWN
} command ;



#endif