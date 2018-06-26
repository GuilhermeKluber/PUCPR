#include "stdio.h"

void binario (int num)
{
	if (num /2 != 0 )
	    binario(num/2);
	printf("%d",num%2);
}

int main(int argc, char* argv[])
{
	int a;
	printf("Digite um numero: ");
	scanf("%d",&a);
	printf("\nO numero %d em binario e: ",a);
    binario(a);
	return 0;
}
