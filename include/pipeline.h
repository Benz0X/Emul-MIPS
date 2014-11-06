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
/*
typedef struct 
{
	instruction ins;
	pipestep step;

}pipe;
*/

typedef enum{IF,ID,EX,MEM,WB} pipestep;
typedef enum {OK,EmptyPipe,InvalidInstruction,InvalidExecution,IntegerOverflow,BreakPoint,ReservedInstruction,SysCall,quit} exception;

int pipeline(uint32_t end, state running, int affichage);

int exceptionHandler(exception number);

int fetch(instruction* pinsIF);
int decode(instruction insID, int* res);
int execute(instruction insEX, pipestep EX, int dico_entry, int* tmp);

#endif
