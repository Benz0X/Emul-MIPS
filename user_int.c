#include "user_int.h"
#include "fonctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

command decrypt(char input [])
{
    char* word;
    word = strtok(input, " ");
    command current_cmd=getCommand(word);

    switch (current_cmd){
case LOAD:
printf("%d \n",current_cmd);
    break;
case EXIT:
printf("%d \n",current_cmd);
    break;
case DISP:
printf("%d \n",current_cmd);
    break;
case DISASM:
printf("%d \n",current_cmd);
    break;
case SET:
printf("%d \n",current_cmd);
    break;
case ASSERT:
printf("%d \n",current_cmd);
    break;
case DEBUG:
printf("%d \n",current_cmd);
    break;
case RESUME:
printf("%d \n",current_cmd);
    break;
case RUN:
printf("%d \n",current_cmd);
    break;
case STEP:
printf("%d \n",current_cmd);
    break;
case BREAK:
printf("%d \n",current_cmd);
    break;
case UNKNOWN:
printf("%d \n",current_cmd);
    break;

default:

    break;
    }
    return 0;
}
