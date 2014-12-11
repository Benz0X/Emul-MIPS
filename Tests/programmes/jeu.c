#include <stdio.h>
#include <string.h>

//Attention ça marche pas
int main(int argc, char const *argv[])
{
	char name[32];
	int length = 0;
	int aleatoire;
	int choice;
	int i;
	int max;
	int res;


		printf("Entrez votre nom :\n");
		scanf("%s",name);
		length = strlen(name);
		printf("Entrez un max\n");
		scanf("%d",&max);
	
	for (i=0; i<length-2; i++){
		aleatoire += name[i];
		}
	aleatoire = aleatoire * 419669081 % 999999989 ;
	aleatoire=(aleatoire % max+max)/2;
	printf("%d\n", aleatoire);
	printf("Trouvez le nombre entre 0 et %d\n",max );
	i=0;
	do{
		scanf("%d",&res);
		i++;
		if(res<aleatoire)printf("C'est +\n");
		if (res>aleatoire)printf("C'est -\n");
		if (res==aleatoire){printf("Vous avez trouvé en %d coups !\n",i);break;}
		}
	while(res!=aleatoire);

	return 0;
}
