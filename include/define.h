#ifndef _define_h
#define _define_h
#include "mem.h"
#include "elf/elf.h"
#include "elf/syms.h"

#define INPUT_SIZE 1024
#define PROMPT_STRING "EmulMips : > "
#define MAX_INSTR_ARG 3

extern int32_t reg_mips[35];
extern mem memory;
extern stab symtab;
extern int scriptmode;

typedef struct {
	uint b4:8,
	b3:8,
	b2:8,
	b1:8;
} struct_word; //ATTENTION AU SENS, PT A CHANGER

typedef struct {
char* name;
uint32_t mask;
uint32_t instr;
short type;
short nb_arg;
char* argname[MAX_INSTR_ARG];
}dico_info;

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