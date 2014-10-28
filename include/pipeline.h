#ifndef _pipeline_h
#define _pipeline_h

#if defined (WIN32) || defined (WIN64)
#include <windows.h>
#define DELAY(temps) Sleep(temps)
#else
#include <unistd.h>
#define DELAY(temps) usleep(temps)
#endif

typedef enum{stop,running,step,stepinto} state;
typedef enum{IF,ID,EX,MEM,WB} pipestep;
typedef enum {OK,EmptyPipe,InvalidInstruction} exception;

int pipeline(instruction insID, instruction insEX, instruction insMEM, instruction insWB, uint32_t end, state running, int affichage);

void exceptionHandler(exception number);

int fetch(instruction* pinsIF);
int decode(instruction insID, int* res);


#endif
