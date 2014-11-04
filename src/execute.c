#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "define.h"
#include "pipeline.h"
#include "fonctions.h"
#include "common/notify.h"
#include <stdint.h>

/*
    switch pipestep{
        case EX:
        break;
    }
    return OK;
*/

//ADDITION
int ADD(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        ;
        int64_t max=0x0FFFFFFFF;
        if ((int64_t)reg_mips[ins.r.rs] + (int64_t)reg_mips[ins.r.rt]>max) {
            *tmp=reg_mips[ins.r.rd];
            return IntegerOverflow;
        }
        *tmp=reg_mips[ins.r.rs] + reg_mips[ins.r.rd];
        break;

    case WB:
        writeRegindex(ins.r.rd,*tmp);
        break;
    }
    return OK;
}

int ADDI(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        ;
        int64_t max=0xFFFFFFFF;
        if ((int64_t)reg_mips[ins.r.rs] + (int64_t)reg_mips[ins.r.rt]>max) {
            *tmp=reg_mips[ins.r.rd];
            return IntegerOverflow;
        }
        *tmp=reg_mips[ins.r.rs] + reg_mips[ins.r.rd];
        break;

    case WB:
        writeRegindex(ins.r.rd,*tmp);
        break;
    }
    return OK;
}
int ADDIU(instruction ins, int pipestep, int* tmp) {
    return writeRegindex(ins.i.rt,reg_mips[ins.i.rs]+(int16_t)ins.i.immediate);
}

int ADDU(instruction ins, int pipestep, int* tmp) {
    return writeRegindex(ins.r.rd,reg_mips[ins.r.rs]+ins.r.rt);
}


//AND
int AND(instruction ins, int pipestep, int* tmp) {
    return writeRegindex(ins.r.rd,reg_mips[ins.r.rs] & ins.r.rt);
}

int ANDI(instruction ins, int pipestep, int* tmp) {
    return writeRegindex(ins.i.rt,reg_mips[ins.i.rs] & (int16_t)ins.i.immediate);
}


//BRANCH
int BEQ(instruction ins, int pipestep, int* tmp) {
    if(reg_mips[ins.i.rs]==reg_mips[ins.i.rt]) {
        writeRegindex(PC,reg_mips[PC]+4*ins.i.immediate);
    }
    return 0;
}

int BGEZ(instruction ins, int pipestep, int* tmp) {
    if(reg_mips[ins.i.rs]>=0) {
        writeRegindex(PC,reg_mips[PC]+4*ins.i.immediate);
    }
    return 0;
}

int BGTZ(instruction ins, int pipestep, int* tmp) {
    if(reg_mips[ins.i.rs]>0) {
        writeRegindex(PC,reg_mips[PC]+4*ins.i.immediate);
    }
    return 0;
}

int BLEZ(instruction ins, int pipestep, int* tmp) {
    if(reg_mips[ins.i.rs]<=0) {
        writeRegindex(PC,reg_mips[PC]+4*ins.i.immediate);
    }
    return 0;
}

int BLTZ(instruction ins, int pipestep, int* tmp) {
    if(reg_mips[ins.i.rs]<0) {
        writeRegindex(PC,reg_mips[PC]+4*ins.i.immediate);
    }
    return 0;
}

int BNE(instruction ins, int pipestep, int* tmp) {
    if(reg_mips[ins.i.rs]!=reg_mips[ins.i.rt]) {
        writeRegindex(PC,reg_mips[PC]+4*ins.i.immediate);
    }
    return 0;
}


//BREAK
int BREAKprog(instruction ins, int pipestep, int* tmp) {
    return 0;
}


//DIV
int DIV(instruction ins, int pipestep, int* tmp) {
    if(reg_mips[ins.r.rt]!=0) {
        writeRegindex(HI,reg_mips[ins.r.rs] % reg_mips[ins.r.rt]);
        writeRegindex(LO,reg_mips[ins.r.rs] / (reg_mips[ins.r.rt]-reg_mips[HI]));
    }

    else INFO_MSG("Division by 0");
    return 0;
}


//JUMP
int J(instruction ins, int pipestep, int* tmp) {
    return 0;
}

int JAL(instruction ins, int pipestep, int* tmp) {
    return 0;
}

int JALR(instruction ins, int pipestep, int* tmp) {
    return 0;
}

int JR(instruction ins, int pipestep, int* tmp) {
    return 0;
}


//LOAD
int LB(instruction ins, int pipestep, int* tmp) {
    return 0;
}

int LBU(instruction ins, int pipestep, int* tmp) {
    return 0;
}

int LUI(instruction ins, int pipestep, int* tmp) {
    return 0;
}

int LW(instruction ins, int pipestep, int* tmp) {
    return 0;
}


//MF
int MFHI(instruction ins, int pipestep, int* tmp) {

    return writeRegindex(ins.r.rd,reg_mips[HI]);
}

int MFLO(instruction ins, int pipestep, int* tmp) {
    return writeRegindex(ins.r.rd,reg_mips[LO]);
}


//MULT
int MULT(instruction ins, int pipestep, int* tmp) {
    int64_t prod=reg_mips[ins.r.rd]*reg_mips[ins.r.rs];
    writeRegindex(LO,(int32_t)prod);
    writeRegindex(HI,(int32_t)(prod >> 32));
    return 0;
}


//NOP
int NOP(instruction ins, int pipestep, int* tmp) {
    return 0;
}


//OR
int OR(instruction ins, int pipestep, int* tmp) {
    return writeRegindex(ins.r.rd,reg_mips[ins.r.rs] | ins.r.rt);
}

int ORI(instruction ins, int pipestep, int* tmp) {
    return writeRegindex(ins.i.rt,reg_mips[ins.i.rs] | (int16_t)ins.i.immediate);
}

int XOR(instruction ins, int pipestep, int* tmp) {
    return writeRegindex(ins.r.rd,reg_mips[ins.r.rs] ^ ins.r.rt);
}


//STORE
int SB(instruction ins, int pipestep, int* tmp) {
    return 0;
}

int SW(instruction ins, int pipestep, int* tmp) {
    return 0;
}


//SIGN EXTEND
int SEB(instruction ins, int pipestep, int* tmp) {
    return 0;
}


//SHIFT
int SLL(instruction ins, int pipestep, int* tmp) {
    return writeRegindex(ins.r.rd,reg_mips[ins.r.rt] << ins.r.sa);
}

int SRA(instruction ins, int pipestep, int* tmp) {
    if ((int)reg_mips[ins.r.rt]>0) {
        return writeRegindex(ins.r.rd,(reg_mips[ins.r.rt] >> ins.r.sa)|0x80000000);
    }
    return writeRegindex(ins.r.rd,reg_mips[ins.r.rt] >> ins.r.sa);
}

int SRL(instruction ins, int pipestep, int* tmp) {
    return writeRegindex(ins.r.rd,reg_mips[ins.r.rt] >> ins.r.sa);
}



//SET
int SLT(instruction ins, int pipestep, int* tmp) {
    if(reg_mips[ins.r.rs]>reg_mips[ins.r.rt]) {
        writeRegindex(ins.r.rd,1);
    }
    else {
        writeRegindex(ins.r.rd,0);
    }
    return 0;
}

int SLTI(instruction ins, int pipestep, int* tmp) {
    if(reg_mips[ins.i.rs]>ins.i.immediate) {
        writeRegindex(ins.i.rt,1);
    }
    else {
        writeRegindex(ins.i.rt,0);
    }
    return 0;
}

int SLTIU(instruction ins, int pipestep, int* tmp) {
    return 0;
}

int SLTU(instruction ins, int pipestep, int* tmp) {
    return 0;
}


//SUB
int SUB(instruction ins, int pipestep, int* tmp) {
    return 0;
}

int SUBU(instruction ins, int pipestep, int* tmp) {
    return writeRegindex(ins.r.rd,reg_mips[ins.r.rs]-ins.r.rt);
}


//SYSCALL
int SYSCALL(instruction ins, int pipestep, int* tmp) {
    return 0;
}


