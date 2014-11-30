#ifndef _define_h
#define _define_h
#include "mem.h"
#include "liste.h"
#include "elf/elf.h"
#include "elf/syms.h"


//Defines :

#define SYSCALL_LIBC 1                  //1 for LIBC, 0 for doc

#define INPUT_SIZE 1024                 //Maximum size of an input string
#define PROMPT_STRING "EmulMips : > "
#define DEFAULT_S_ADDR 0x3000
#define MAX_INSTR_ARG 3                 //Max argument an instruction can have
#define MAX_NAME_SIZE 10                //Maximum size of an instruction argument name string
#define DICONAME "dico.dico"            //name of the loaded dictionnary
#define NBREG 32                        //Number of register in the MIPS (not counting HI, LO, PC)
#define HI 32                           //index of HI, LO and PC register
#define LO 33
#define PC 34

//Typedef :


typedef struct {
    uint function:6,sa:5,rd:5,rt:5,rs:5,opcode:6;
} r_type;
typedef struct {
    int immediate:16;
    uint rt:5,rs:5,opcode:6;
} i_type;
typedef struct {
    uint target:26,opcode:6;
} j_type;

typedef union {
    r_type r;
    i_type i;
    j_type j;
    uint32_t value;
} instruction;                          //union allowing easy access of each member of R, I or J instr, and also the int value

typedef struct {
    uint b4:8,
         b3:8,
         b2:8,
         b1:8;
} struct_word;                          //word of 4 bytes

typedef struct {
    char name[MAX_NAME_SIZE];
    uint32_t mask;
    uint32_t instr;
    short type;                             //0=R, 1=I,2=J
    short nb_arg;
    char argname[MAX_INSTR_ARG][MAX_NAME_SIZE];
    int (*exec)(instruction,int,int*);
} dico_info;                            //Structure containing everything about an instruction


typedef enum {                          //Commandes de l'interpreteur
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
    CLOCK,
    VERB,
    UNKNOWN
} command ;




typedef enum {IF,ID,EX,MEM,WB} pipestep;
typedef struct
{
    instruction ins;    //Current instruction at this step
    pipestep step;      
    int dico_entry;     //Current decoded number at this step
    int tmp;            //Current value at this step
    int tmp2;           //used only for MULT and DIV
} pipeblock;                            // all information required for each pipeline state




//Global datas :
extern int scriptmode;                  //allow easy switch between interactive and script mode

extern int32_t reg_mips[NBREG+3];       //All mips registers
extern mem memory;                      //Virtual machine memory
extern stab symtab;                     //loaded ELF symtab
extern stab libcsymtab;

extern uint32_t textStart;              //Debut du segment .text
extern uint32_t textEnd;                //Fin du segment .text
extern uint32_t libcTextStart;          //Debut du segment libc.text
extern uint32_t libcTextEnd;            //Fin du segment libc.text

extern dico_info* dico_data;            //contain all info from the dictionnary
extern int nbinstr;                     //contain the number of instruction in the dictionnary

extern list breaklist;                  //Liste des points d'arrets
extern short verbose;                   //Carry verbose value [0..5]

extern pipeblock vpipeline[5];          //Virtual Pipeline
extern uint32_t return_addr;            //Allow step to get back after a function call
extern int clocktime;                   //Vitesse d'execution : 0 pour max




#endif