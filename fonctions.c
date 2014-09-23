#include "user_int.h"
#include "fonctions.h"
#include <string.h>



int getFromScript(FILE *fileptr,char input[])
{
    if( fgets( input, INPUT_SIZE, fileptr ) == NULL)
    {
        printf("end of file\n");
        return 0;
    }
    return 1;
}
int getFromUser(char input[])
{
    fgets(input,INPUT_SIZE,stdin);
    return 1;
}

command getCommand(char word[]){
if (!strcmp(word,"load"))
return LOAD;
if (!strcmp(word,"exit"))
return EXIT;
if (!strcmp(word,"disp"))
return DISP;
if (!strcmp(word,"disasm"))
return DISASM;
if (!strcmp(word,"set"))
return SET;
if (!strcmp(word,"assert"))
return ASSERT;
if (!strcmp(word,"debug"))
return DEBUG;
if (!strcmp(word,"resume"))
return RESUME;
if (!strcmp(word,"run"))
return RUN;
if (!strcmp(word,"step"))
return STEP;
if (!strcmp(word,"break"))
return BREAK;

return UNKNOWN;
}
