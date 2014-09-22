#include "fonctions.h"
#include "user_int.h"
#include <string.h>



int getFromScript(FILE *fileptr,char input[])
{
    if( fgets( input, 100, fileptr ) == NULL)
    {
        printf("end of file\n");
        return 1;
    }
    return 0;
}
int getFromUser(char input[])
{
    return 1;
}
int decrypt(char input [])
{
    char* word;
    word = strtok(input, " ");

    //MEGA CASE
    return 0;
}
