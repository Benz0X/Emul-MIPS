#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "define.h"
#include "fonctions.h"
#include "common/notify.h"



//ADDITION
int ADD(instruction ins) {
    return 0;
}

int ADDI(instruction ins) {
    return 0;
}

int ADDIU(instruction ins) {
    return writeRegindex(ins.i.rt,reg_mips[ins.i.rs]+(int16_t)ins.i.immediate);
}

int ADDU(instruction ins) {
    return writeRegindex(ins.r.rd,reg_mips[ins.r.rs]+ins.r.rt);
}


//AND
int AND(instruction ins) {
    return writeRegindex(ins.r.rd,reg_mips[ins.r.rs] & ins.r.rt);
}

int ANDI(instruction ins) {
    return writeRegindex(ins.i.rt,reg_mips[ins.i.rs] & (int16_t)ins.i.immediate);
}


//BRANCH
int BEQ(instruction ins) {
    return 0;
}

int BGEZ(instruction ins) {
    return 0;
}

int BGTZ(instruction ins) {
    return 0;
}

int BLEZ(instruction ins) {
    return 0;
}

int BLTZ(instruction ins) {
    return 0;
}

int BNE(instruction ins) {
    return 0;
}


//BREAK
int BREAKprog(instruction ins) {
    return 0;
}


//DIV
int DIV(instruction ins) {
    if(reg_mips[ins.r.rt]!=0){
    writeRegindex(HI,reg_mips[ins.r.rs] % reg_mips[ins.r.rt]);
    writeRegindex(LO,reg_mips[ins.r.rs] / (reg_mips[ins.r.rt]-reg_mips[HI]));}
    
    else INFO_MSG("Division by 0");
    return 0;
}


//JUMP
int J(instruction ins) {
    return 0;
}

int JAL(instruction ins) {
    return 0;
}

int JALR(instruction ins) {
    return 0;
}

int JR(instruction ins) {
    return 0;
}


//LOAD
int LB(instruction ins) {
    return 0;
}

int LBU(instruction ins) {
    return 0;
}

int LUI(instruction ins) {
    return 0;
}

int LW(instruction ins) {
    return 0;
}


//MF
int MFHI(instruction ins) {

    return writeRegindex(ins.r.rd,reg_mips[HI]);
}

int MFLO(instruction ins) {
    return writeRegindex(ins.r.rd,reg_mips[LO]);
}


//MULT
int MULT(instruction ins) {
    int64_t prod=reg_mips[ins.r.rd]*reg_mips[ins.r.rs];
    writeRegindex(LO,(int32_t)prod);
    writeRegindex(HI,(int32_t)(prod >> 32));
    return 0;
}


//NOP
int NOP(instruction ins) {
    return 0;
}


//OR
int OR(instruction ins) {
    return writeRegindex(ins.r.rd,reg_mips[ins.r.rs] | ins.r.rt);
}

int ORI(instruction ins) {
    return writeRegindex(ins.i.rt,reg_mips[ins.i.rs] | (int16_t)ins.i.immediate);
}

int XOR(instruction ins) {
    return writeRegindex(ins.r.rd,reg_mips[ins.r.rs] ^ ins.r.rt);
}


//STORE
int SB(instruction ins) {
    return 0;
}

int SW(instruction ins) {
    return 0;
}


//SIGN EXTEND
int SEB(instruction ins) {
    return 0;
}


//SHIFT
int SLL(instruction ins) {
    return writeRegindex(ins.r.rd,reg_mips[ins.r.rt] << ins.r.sa);
}

int SRA(instruction ins) {
    //return writeRegindex(ins.r.rd,reg_mips[ins.r.rt] >> ins.r.sa);
}

int SRL(instruction ins) {
    return writeRegindex(ins.r.rd,reg_mips[ins.r.rt] >> ins.r.sa);
}



//SET
int SLT(instruction ins) {
    return 0;
}

int SLTI(instruction ins) {
    return 0;
}

int SLTIU(instruction ins) {
    return 0;
}

int SLTU(instruction ins) {
    return 0;
}


//SUB
int SUB(instruction ins) {
    return 0;
}

int SUBU(instruction ins) {
    return writeRegindex(ins.r.rd,reg_mips[ins.r.rs]-ins.r.rt);
}


//SYSCALL
int SYSCALL(instruction ins) {
    return 0;
}
