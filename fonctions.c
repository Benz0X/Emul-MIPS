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
return EXIT;
}
