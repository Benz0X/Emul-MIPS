#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
    FILE *script;
    char line[100];
    char* token;
    char* word;


    if( (script = fopen ("script.txt", "r")) != NULL ) {

        while( fgets( line, 100, script ) != NULL) {
            printf( "\nLigne : %s", line);
            token = strtok(line,"#");
            printf( "Sans commentaires : %s\n", token);

            word = strtok(token," ");
            printf( "  Decoupage : ");
            while( word != NULL ) {
                printf( " %s\t", word );
                word = strtok(NULL, " ");

            }
        }
        printf( "\nCan't get more lines\n" );

    }


    fclose( script );
    return 0;
}


