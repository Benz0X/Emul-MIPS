#ifndef _define_h
#define _define_h
#include "mem.h"
#include "elf/elf.h"
#include "elf/syms.h"

#define INPUT_SIZE 1024
#define PROMPT_STRING "EmulMips : > "
#define MAX_INSTR_ARG 3
#define MAX_NAME_SIZE 10
typedef struct {
	uint b4:8,
	b3:8,
	b2:8,
	b1:8;
} struct_word; //ATTENTION AU SENS, PT A CHANGER

typedef struct {
char name[MAX_NAME_SIZE];
uint32_t mask;
uint32_t instr;
short type;
short nb_arg;
char* argname[MAX_INSTR_ARG];
}dico_info;

typedef struct{
uint function:6,sa:5,rd:5,rt:5,rs:5,opcode:6;
} r_type;
typedef struct{
uint immediate:16,rt:5,rs:5,opcode:6;
} i_type;
typedef struct{
uint target:26,opcode:6;
} j_type;

typedef union{
r_type r;
i_type i;
j_type j;
}instruction;

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

extern int32_t reg_mips[35];
extern mem memory;
extern stab symtab;
extern int scriptmode;
extern dico_info* dico_data;


#endif