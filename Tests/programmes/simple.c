int multiply(int a, int b);

int main (void)
{
        int x = 21;
        int y = 21;
        char tab[21]="AAAAAAAAZZEEEEE";
        tab[1]='e';
        return multiply(x,y) + x + y;
}


int multiply(int a, int b){
	return a*b;
}