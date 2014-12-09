#ifndef _pipeline_h
#define _pipeline_h

#if defined (WIN32) || defined (WIN64)
#include <windows.h>
#define DELAY(temps) Sleep(temps)
#else
#include <unistd.h>
#define DELAY(temps) usleep(temps)
#endif

typedef enum {stop,running,step,stepinto,step_return} state; 			//pipeline state
//All exception possible
typedef enum {OK,EmptyPipe,InvalidInstruction,InvalidExecution,IntegerOverflow,BreakPoint,ReservedInstruction,SysCall,quit,flush,memFail} exception;


//Main pipeline function	(now iterative)
int pipeiter(state running);


int exceptionHandler(exception number);									//Get exception from execution and process it

int fetch(instruction* pinsIF);											//put instruction in insIF from memory
int decode(instruction insID, int* res);								//decode the instruction
int execute(instruction insEX, pipestep EX, int dico_entry, int* tmp);	//execute the instruction

#endif
