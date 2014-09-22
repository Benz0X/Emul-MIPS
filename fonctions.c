#include "user_int.h"
#include "fonctions.h"
#include <string.h>



int getFromScript(FILE *fileptr,char input[])
{
    if( fgets( input, 100, fileptr ) == NULL)
    {
        printf("end of file\n");
        return 0;
    }
    return 1;
}
int getFromUser(char input[])
{
    gets(input);//une entrée sécurisée serait pas mal pour eviter les buffer overflow
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
