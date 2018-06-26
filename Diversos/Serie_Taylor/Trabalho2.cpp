#include <stdio.h>
#include <math.h>

float f(float t)
        {
            return exp(t) ;
        }

float part(float x,int n)
	{
		int aux=n;
		float res=1;
		
		while(aux > 0)
		{
			res= res*x/aux;
			aux = aux-1;
		}
		return res;
	}

float serietaylor(float x)
    {
	int n = 0;          
	float parcela = 1; 
	float soma = 0; 
	float impacto = 1;
   
	while (fabs(impacto) >= 0.000001) {
    	n = n + 1;
    	soma = soma + parcela;
    	parcela = part(x,n);
    	impacto = (soma - parcela)/ soma - 1;
  	}
  	return soma;
	}

int main(int) {
   int num;
   printf("Insira um valor para x que deseja saber o valor de f(x), onde f eh e^x: ");
   scanf("%d",&num);
   printf("\nAproximacao para x=%d %f e Valor funcao e^x %f",num,f(num),serietaylor(num));
   return 0;
}
