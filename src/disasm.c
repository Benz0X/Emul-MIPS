#include <stdint.h>
#include "disasm.h"
#include "define.h"
#include "fonctions.h"
#include "common/notify.h"

int disasm(uint32_t start_addr,uint32_t size){
	if(memory==NULL) {
        WARNING_MSG("No memory loaded");
        return -1;
    }
    uint32_t i=0;
    uint32_t current_addr=start_addr;
    instruction current_instr;
    uint32_t instr_value;
    while (i<size) {
	getInstr(current_addr,&current_instr);
	memcpy(&instr_value,&current_instr,4);
        printf("\n%X :: %8.8X	",current_addr,instr_value);
            i+=4;
            current_addr+=4;
    }

	

	return 0;}