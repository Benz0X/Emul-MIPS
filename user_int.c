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

    break;
case EXIT:

    break;
case DISP:

    break;
case DISASM:

    break;
case SET:

    break;
case ASSERT:

    break;
case DEBUG:

    break;
case RESUME:

    break;
case RUN:

    break;
case STEP:

    break;
case BREAK:

    break;

default:

    break;
    }
    return 0;
}
