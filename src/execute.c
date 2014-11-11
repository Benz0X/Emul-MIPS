#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "define.h"
#include "pipeline.h"
#include "fonctions.h"
#include "emul.h"
#include "common/notify.h"
#include <stdint.h>

/*
    switch pipestep{
        case EX:
        break;
    }
    return OK;
*/
//SIGN EXTEND
int32_t sign_extend(int16_t A){
    if (A >= 0){
        return (int32_t) A;
    }else{
        return (0xFFFF0000+A);
    }
}


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
        *tmp=reg_mips[ins.r.rs] + reg_mips[ins.r.rt];
        break;

    case WB:
    printf("ADD: put %d in %d \n",*tmp,ins.r.rd );
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
        printf("ADDI: put %d in %d \n",*tmp,ins.i.rt );
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
        *tmp=reg_mips[ins.r.rs] + reg_mips[ins.r.rt];
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
        *tmp=reg_mips[ins.r.rs] & reg_mips[ins.r.rt];
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
    }else {*tmp=reg_mips[PC]+8;}
        return flush;
        break;
    case MEM:
        writeRegindex(PC,*tmp-4);
        break;
    }
    return 0;
}

int BGEZ(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
    if(reg_mips[ins.i.rs]>=0) {
        *tmp=reg_mips[PC]+4*ins.i.immediate;
    }else {*tmp=reg_mips[PC]+8;}
    return flush;
        break;
    case MEM:
        writeRegindex(PC,*tmp-4);
        break;
    }
    return 0;
}

int BGTZ(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
    if(reg_mips[ins.i.rs]>0) {
        *tmp=reg_mips[PC]+4*ins.i.immediate;
    }else {*tmp=reg_mips[PC]+8;}
    return flush;
        break;
    case MEM:
        writeRegindex(PC,*tmp-4);
        break;
    }
    return 0;
}

int BLEZ(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
    if(reg_mips[ins.i.rs]<=0) {
        *tmp=reg_mips[PC]+4*ins.i.immediate;
    }else {*tmp=reg_mips[PC]+8;}
    return flush;
        break;
    case MEM:
        writeRegindex(PC,*tmp-4);
        break;
    }
    return 0;
}

int BLTZ(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
    if(reg_mips[ins.i.rs]<0) {
        *tmp=reg_mips[PC]+4*ins.i.immediate;
    } else {*tmp=reg_mips[PC]+8;}
    return flush;
        break;
    case MEM:
        writeRegindex(PC,*tmp-4);
        break;
    }
    return 0;
}

int BNE(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        if(reg_mips[ins.i.rs]!=reg_mips[ins.i.rt]) {
        *tmp=reg_mips[PC]+4*ins.i.immediate;
//        printf("INSID and INSIFflushed\n");
        //insID.value=0;
        //insIF.value=0;
        return flush;
    } else {*tmp=reg_mips[PC]+8;}
        break;
    case MEM:
        printf("BNE : set PC at %8.8X\n", *tmp-4);
        writeRegindex(PC,*tmp-4);
        break;
    }
    return 0;
}








//BREAK
int BREAKprog(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case WB:
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
            vpipeline[EX].tmp2=reg_mips[HI];
            *tmp=reg_mips[LO];

            INFO_MSG("Division by 0");
        }
        else {
            *tmp=reg_mips[ins.r.rs]/reg_mips[ins.r.rt];
            vpipeline[EX].tmp2=reg_mips[ins.r.rs] % reg_mips[ins.r.rt];
        }
        break;
    case WB:
        reg_mips[LO]=*tmp;
        reg_mips[HI]=vpipeline[WB].tmp2;
        break;
    }
    return 0;
}








//JUMP
int J(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        return flush;
        break;
    case MEM:
        reg_mips[PC]= ((reg_mips[PC] & 0xF0000000) | 4*ins.j.target);    //-8 ?
        break;
    }
    
    return OK;
}

int JAL(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        reg_mips[31]=reg_mips[PC]; 
        return flush;
        break;
    
    case MEM:
            printf("JAL to %X\n",textStart + ((reg_mips[PC] & 0xF0000000) | 4*ins.j.target));

        reg_mips[PC]= ((reg_mips[PC] & 0xF0000000) | 4*ins.j.target);    //-8 ?
        break;
    }
    
    return OK;
}

int JALR(instruction ins, int pipestep, int* tmp) {     //NOTE : RD SHOULDN'T BE 31
    switch (pipestep) {
    case EX:
        reg_mips[ins.r.rd]=reg_mips[PC]; 
        return flush;
        break;
    
    case MEM:
        reg_mips[PC]= ((reg_mips[PC] & 0xF0000000) | 4*reg_mips[ins.r.rs]);    //-8 ?
        break;
    }

    return OK;
}

int JR(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        return flush;
        break;
    case MEM:
        reg_mips[PC]=((reg_mips[PC] & 0xF0000000) | reg_mips[ins.r.rs])-4;    //-8 ?
        break;
    }
    
    return OK;
}


//LOAD
int LB(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        *tmp=reg_mips[ins.i.rs]+sign_extend(ins.i.immediate); //GPR[base]+sign_extend(offset)
        break;
    
    case MEM:
        if(memRead(*tmp,0,tmp)!=0){return memFail;}
        break;
    
    case WB:
        writeRegindex(ins.i.rt,sign_extend(*tmp));
        break;
    }

    return OK;
}

int LBU(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        *tmp=reg_mips[ins.i.rs]+sign_extend(ins.i.immediate); //GPR[base]+sign_extend(offset)
        break;
    
    case MEM:
        if(memRead(*tmp,0,tmp)!=0){return memFail;}
        break;
    
    case WB:
        writeRegindex(ins.i.rt,(uint32_t)*tmp);
        break;
    }

    return OK;
}

int LUI(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case WB:
        writeRegindex(ins.i.rt,(int32_t)(ins.i.immediate<<16));
        break;
    }
    return OK;
}

int LW(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        *tmp=reg_mips[ins.i.rs]+sign_extend(ins.i.immediate); //GPR[base]+sign_extend(offset)
        break;
    
    case MEM:
        if(memRead(*tmp,1,tmp)!=0){return memFail;}
        break;
    
    case WB:
        writeRegindex(ins.i.rt,(uint32_t)*tmp);
        break;
    }

    return OK;
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
    switch (pipestep) {
    case EX:;
    int64_t prod=reg_mips[ins.r.rd]*reg_mips[ins.r.rs];
        vpipeline[EX].tmp2=(int32_t)(prod >> 32);
        *tmp=(int32_t)prod;
        break;
    case WB:
        reg_mips[LO]=vpipeline[EX].tmp2;
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
    switch (pipestep) {
    case EX:
        *tmp=reg_mips[ins.i.rs]+sign_extend(ins.i.immediate); //GPR[base]+sign_extend(offset)
        break;
    
    case MEM:
        if(memWrite(*tmp,0,reg_mips[ins.i.rt])!=0){return memFail;}
        break;
    }

    return OK;
}

int SW(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case EX:
        *tmp=reg_mips[ins.i.rs]+sign_extend(ins.i.immediate); //GPR[base]+sign_extend(offset)
        break;
    
    case MEM:
        if(memWrite(*tmp,1,reg_mips[ins.i.rt])!=0){return memFail;}
        break;
    }
    return OK;
}


//SIGN EXTEND
int SEB(instruction ins, int pipestep, int* tmp) {
    switch (pipestep) {
    case WB:
        writeRegindex(ins.r.rd,(int32_t)(0x000000FF&reg_mips[ins.r.rt]));
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
    printf("SLL\n");
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
    case WB:
        return SysCall;
        break;
    }
    return OK;
}
