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
        if ((int64_t)reg_mips[ins.r.rs] + (int64_t)reg_mips[ins.r.rt]>max || (int64_t)reg_mips[ins.r.rs] + (int64_t)reg_mips[ins.r.rt]<-max) {
            *tmp=reg_mips[ins.r.rd];
            return IntegerOverflow;
        }
        *tmp=reg_mips[ins.r.rs] + reg_mips[ins.r.rd];
        break;

    case WB:
    printf("met %d dans %d \n",*tmp,ins.r.rd );
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
        if ((int64_t)ins.i.immediate + (int64_t)reg_mips[ins.i.rs]>max || (int64_t)ins.i.immediate + (int64_t)reg_mips[ins.i.rs]<-max) {
            *tmp=reg_mips[ins.i.rt];
            return IntegerOverflow;
        }
        *tmp=ins.i.immediate + reg_mips[ins.i.rs];
  //      printf("temp= %d\n", *tmp);
        break;

    case WB:
//    printf("write %d in rt\n", *tmp);
        writeRegindex(ins.i.rt,*tmp);
        break;
    }
    return OK;
}
int ADDIU(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        *tmp=ins.i.immediate + reg_mips[ins.i.rs];
        break;

    case WB:
        writeRegindex(ins.i.rt,*tmp);
        break;
    }
    return OK;
}

int ADDU(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        *tmp=reg_mips[ins.r.rs] + reg_mips[ins.r.rd];
        break;

    case WB:
        writeRegindex(ins.r.rd,*tmp);
        break;
    }
    return OK;
}






//AND
int AND(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        *tmp=reg_mips[ins.r.rs] & reg_mips[ins.r.rd];
        break;

    case WB:
        writeRegindex(ins.r.rd,*tmp);
        break;
    }
    return OK;
}

int ANDI(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        *tmp=(uint32_t)ins.i.immediate & reg_mips[ins.i.rs]; //Cast pour zero extend
        break;

    case WB:
        writeRegindex(ins.i.rt,*tmp);
        break;
    }
    return OK;
}







//BRANCH
int BEQ(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
    if(reg_mips[ins.i.rs]==reg_mips[ins.i.rt]) {
        *tmp=reg_mips[PC]+4*ins.i.immediate;
    }
        break;
    case MEM:
        writeRegindex(PC,*tmp-8);
        break;
    }
    return 0;
}

int BGEZ(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
    if(reg_mips[ins.i.rs]>=0) {
        *tmp=reg_mips[PC]+4*ins.i.immediate;
    }
        break;
    case MEM:
        writeRegindex(PC,*tmp-8);
        break;
    }
    return 0;
}

int BGTZ(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
    if(reg_mips[ins.i.rs]>0) {
        *tmp=reg_mips[PC]+4*ins.i.immediate;
    }
        break;
    case MEM:
        writeRegindex(PC,*tmp-8);
        break;
    }
    return 0;
}

int BLEZ(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
    if(reg_mips[ins.i.rs]<=0) {
        *tmp=reg_mips[PC]+4*ins.i.immediate;
    }
        break;
    case MEM:
        writeRegindex(PC,*tmp-8);
        break;
    }
    return 0;
}

int BLTZ(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
    if(reg_mips[ins.i.rs]<0) {
        *tmp=reg_mips[PC]+4*ins.i.immediate;
    }
        break;
    case MEM:
        writeRegindex(PC,*tmp-8);
        break;
    }
    return 0;
}

int BNE(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        if(reg_mips[ins.i.rs]!=reg_mips[ins.i.rt]) {
        *tmp=reg_mips[PC]+4*ins.i.immediate;
        printf("jump à : %8.8X\n", *tmp-4);
    } else {*tmp=reg_mips[PC];}
        break;
    case MEM:
        writeRegindex(PC,*tmp-4);
        break;
    }
    return 0;
}








//BREAK
int BREAKprog(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        return BreakPoint;
        break;
    }
    return OK;
}








//DIV
int DIV(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        if(reg_mips[ins.r.rt]==0) {
            *tmp=reg_mips[LO];
            INFO_MSG("Division by 0");
        }
        else {
            *tmp=reg_mips[ins.r.rs]/reg_mips[ins.r.rt];
        }
        break;
    case MEM:
        reg_mips[LO]=*tmp;
        if(reg_mips[ins.r.rt]==0) {
            *tmp=reg_mips[HI];
        }
        else {
            *tmp=reg_mips[ins.r.rs] % reg_mips[ins.r.rt];
        }
        break;
    case WB:
        reg_mips[HI]=*tmp;
        break;
    }
    return 0;
}








//JUMP
int J(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case MEM:
        reg_mips[PC]=((reg_mips[PC] & 0xF0000000) | ins.j.target)-8;    //-8 ?
        break;
    }
    
    return OK;
}

int JAL(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        reg_mips[31]=reg_mips[PC]; 
        break;
    
    case MEM:
        reg_mips[PC]=((reg_mips[PC] & 0xF0000000) | ins.j.target)-8;    //-8 ?
        break;
    }
    
    return OK;
}

int JALR(instruction ins, int pipestep, int* tmp) {     //NOTE : RD SHOULDN'T BE 31
    switch (pipestep) {
    case EX:
        reg_mips[ins.r.rd]=reg_mips[PC]; 
        break;
    
    case MEM:
        reg_mips[PC]=((reg_mips[PC] & 0xF0000000) | reg_mips[ins.r.rs])-8;    //-8 ?
        break;
    }

    return OK;
}

int JR(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case MEM:
        reg_mips[PC]=((reg_mips[PC] & 0xF0000000) | reg_mips[ins.r.rs])-8;    //-8 ?
        break;
    }
    
    return OK;
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
    switch (pipestep) {
    case WB:
        writeRegindex(ins.r.rd,reg_mips[HI]);
        break;
    }
    return OK;
}

int MFLO(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case WB:
        writeRegindex(ins.r.rd,reg_mips[LO]);
        break;
    }
    return OK; 
}


//MULT
int MULT(instruction ins, int pipestep, int* tmp) {
    int64_t prod=reg_mips[ins.r.rd]*reg_mips[ins.r.rs];
    switch (pipestep) {
    case EX:
        *tmp=(int32_t)prod;
        break;
    case MEM:
        reg_mips[LO]=*tmp;
        *tmp=(int32_t)(prod >> 32);
        break;
    case WB:
        reg_mips[HI]=*tmp;
        break;
    }
    return 0;
}



//NOP
int NOP(instruction ins, int pipestep, int* tmp) {
    return 0;
}


//OR
int OR(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        *tmp=reg_mips[ins.r.rt] | reg_mips[ins.r.rs];
        break;

    case WB:
        writeRegindex(ins.r.rd,*tmp);
        break;
    }
    return OK;
}

int ORI(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        *tmp=(uint32_t)ins.i.immediate + reg_mips[ins.i.rs];//cast for zero extend
        break;

    case WB:
        writeRegindex(ins.i.rt,*tmp);
        break;
    }
    return OK;
}

int XOR(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        *tmp=reg_mips[ins.r.rt] ^ reg_mips[ins.r.rs];
        break;

    case WB:
        writeRegindex(ins.r.rd,*tmp);
        break;
    }
    return OK;
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
    switch (pipestep) {
    case WB:
        writeRegindex(ins.r.rd,(int32_t)reg_mips[ins.r.rt]);
        break;
    }
    return OK; 
}


//SHIFT
int SLL(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        *tmp=reg_mips[ins.r.rt] << ins.r.sa;
        break;

    case WB:
        writeRegindex(ins.r.rd,*tmp);
        break;
    }
    return 0;
}

int SRA(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        if ((int)reg_mips[ins.r.rt]<0) {

            *tmp=(reg_mips[ins.r.rt] >> ins.r.sa)|0x80000000;
        }
        else *tmp=reg_mips[ins.r.rt] >> ins.r.sa;

        break;

    case WB:
        writeRegindex(ins.r.rd,*tmp);
        break;
    }
    return OK;
}

int SRL(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        *tmp=reg_mips[ins.r.rt] >> ins.r.sa;
        break;

    case WB:
        writeRegindex(ins.r.rd,*tmp);
        break;
    }
    return OK;
}



//SET
int SLT(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        if(reg_mips[ins.r.rs]>reg_mips[ins.r.rt]) {
            *tmp=1;
        }
        else {
            *tmp=0;
        }
        break;

    case WB:
        writeRegindex(ins.r.rd,*tmp);
        break;
    }
    return OK;
}

int SLTI(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        if(reg_mips[ins.i.rs]<ins.i.immediate) {
            *tmp=1;
        }
        else {
            *tmp=0;
        }
        break;

    case WB:
        writeRegindex(ins.i.rt,*tmp);
        break;
    }
    return OK;
}
int SLTIU(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        if((uint32_t)reg_mips[ins.i.rs]<(uint32_t)ins.i.immediate) {//a vérifier
            *tmp=1;
        }
        else {
            *tmp=0;
        }
        break;

    case WB:
        writeRegindex(ins.i.rt,*tmp);
        break;
    }
    return OK;
}

int SLTU(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        if((uint32_t)reg_mips[ins.r.rs]>(uint32_t)reg_mips[ins.r.rt]) {
            *tmp=1;
        }
        else {
            *tmp=0;
        }
        break;

    case WB:
        writeRegindex(ins.r.rd,*tmp);
        break;
    }
    return OK;
}


//SUB
int SUB(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        ;
        int64_t max=0x0FFFFFFFF;
        if ((int64_t)reg_mips[ins.r.rs] - (int64_t)reg_mips[ins.r.rt]>max || (int64_t)reg_mips[ins.r.rs] - (int64_t)reg_mips[ins.r.rt]<-max) {
            *tmp=reg_mips[ins.r.rd];
            return IntegerOverflow;
        }
        *tmp=reg_mips[ins.r.rs] - reg_mips[ins.r.rd];
        break;

    case WB:
        writeRegindex(ins.r.rd,*tmp);
        break;
    }
    return OK;
}


int SUBU(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        *tmp=reg_mips[ins.r.rt] - reg_mips[ins.r.rs];
        break;

    case WB:
        writeRegindex(ins.r.rd,*tmp);
        break;
    }
    return OK;
}


//SYSCALL
int SYSCALL(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        return SysCall;
        break;
    }
    return OK;
}
