#ifndef _execute_h
#define _execute_h

//Fonction simulant les instructions du MIPS
//Prennent l'instruction en paramètre

//ADDITION
int ADD(instruction ins, int pipestep, int* tmp);
int ADDI(instruction ins, int pipestep, int* tmp);
int ADDIU(instruction ins, int pipestep, int* tmp);
int ADDU(instruction ins, int pipestep, int* tmp);

//AND
int AND(instruction ins, int pipestep, int* tmp);
int ANDI(instruction ins, int pipestep, int* tmp);

//BRANCH
int BEQ(instruction ins, int pipestep, int* tmp);
int BGEZ(instruction ins, int pipestep, int* tmp);
int BGTZ(instruction ins, int pipestep, int* tmp);
int BLEZ(instruction ins, int pipestep, int* tmp);
int BLTZ(instruction ins, int pipestep, int* tmp);
int BNE(instruction ins, int pipestep, int* tmp);

//BREAK
int BREAKprog(instruction ins, int pipestep, int* tmp);

//DIV
int DIV(instruction ins, int pipestep, int* tmp);

//JUMP
int J(instruction ins, int pipestep, int* tmp);
int JAL(instruction ins, int pipestep, int* tmp);
int JALR(instruction ins, int pipestep, int* tmp);
int JR(instruction ins, int pipestep, int* tmp);

//LOAD
int LB(instruction ins, int pipestep, int* tmp);
int LBU(instruction ins, int pipestep, int* tmp);
int LUI(instruction ins, int pipestep, int* tmp);
int LW(instruction ins, int pipestep, int* tmp);

//MF
int MFHI(instruction ins, int pipestep, int* tmp);
int MFLO(instruction ins, int pipestep, int* tmp);

//MULT
int MULT(instruction ins, int pipestep, int* tmp);

//NOP
int NOP(instruction ins, int pipestep, int* tmp);

//OR
int OR(instruction ins, int pipestep, int* tmp);
int ORI(instruction ins, int pipestep, int* tmp);
int XOR(instruction ins, int pipestep, int* tmp);

//STORE
int SB(instruction ins, int pipestep, int* tmp);
int SW(instruction ins, int pipestep, int* tmp);

//SIGN EXTEND
int SEB(instruction ins, int pipestep, int* tmp);

//SHIFT
int SLL(instruction ins, int pipestep, int* tmp);
int SRA(instruction ins, int pipestep, int* tmp);
int SRL(instruction ins, int pipestep, int* tmp);

//SET
int SLT(instruction ins, int pipestep, int* tmp);
int SLTI(instruction ins, int pipestep, int* tmp);
int SLTIU(instruction ins, int pipestep, int* tmp);
int SLTU(instruction ins, int pipestep, int* tmp);

//SUB
int SUB(instruction ins, int pipestep, int* tmp);
int SUBU(instruction ins, int pipestep, int* tmp);

//SYSCALL
int SYSCALL(instruction ins, int pipestep, int* tmp);







#endif
