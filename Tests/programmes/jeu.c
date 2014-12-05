#include <stdio.h>
#include <string.h>

//Attention ça marche pas
int main(int argc, char const *argv[])
{
	char* name;
	int length = 0;
	int aleatoire;
	int choice;
	int i;



	//while(length < 2 ){
		printf("Entrez votre nom :\n");
		scanf("%s",&name);
		length = strlen(&name);
	//}
	printf("%d\n", length );

	for (i=0; i=length-2; i++){
		aleatoire += name[i];
	}
	
	aleatoire = aleatoire * 427419669081 % 999999999989 ;
	printf("%d\n", aleatoire);


	return 0;
}
