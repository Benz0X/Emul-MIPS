#ifndef _pipeline_h
#define _pipeline_h

enum{IF,ID,EX,MEM,WB};
enum{stop,running,step,stepinto};

int pipeline(instruction instrID, instruction instrEX, instruction instrMEM, instruction instrWB, uint32_t end, int running, int affichage);
int fetch(instruction insIF);


#endif
