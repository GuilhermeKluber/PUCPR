
#include <stdio.h>
#include <math.h>

float f(float t)
        {
            //return 10.10*pow(t,4) - 5.12*pow(t,3) + 9.18*pow(t,2) -5.84*t -5.66 ;
            return exp(t)*log(t)-1;
        }
        
float Bisseccao(float inicio, float fim, float precisao, int iteracoes)
        {
            float Erro = 1;
            int Cont = 0;
            float Raiz = inicio;
            float Temp;
            
            while (Erro > precisao & Cont < iteracoes)
            {
                Temp = Raiz;
                Raiz = (inicio + fim) / 2;
                Erro = fabs((Raiz - Temp) / Raiz);
                if (f(inicio) * f(Raiz) < 0)
                    fim = Raiz;
                else
                    inicio = Raiz;
                Cont++;
            }
            return Raiz;
        }
        
int main(int) {
   
   // eps: erro
   // a0 e b0: pontos iniciais
   // tam: tamanho do intervlao
   // pm: ponto médio
   // it: número de iterações
   
   float eps, a0, b0, tam, pm, it; 
   int bolzano_ok = 0; // flag do teorema de Bolzano
   
   printf("Digite a precisao requerida (ex. 0.0001): \n");
   scanf("%f",&eps);

   printf("Digite o intervalo onde deve se buscar a raiz (ex. [0;5] = digite 0 5): \n");
   scanf("%f %f",&a0,&b0);

   bolzano_ok = (f(a0)*f(b0) > 0?0:1);

   while(bolzano_ok == 0) {
      printf("\nIntervalo [%f;%f] nao satisfaz as condicoes do teorema de Bolzano. Tente outro.\n Intervalo: ",a0,b0);
      scanf("%f %f", &a0, &b0);
      bolzano_ok = (f(a0)*f(b0) > 0?0:1);
   }

   it = log10(b0 - a0);
   it -= log10(eps);
   int interacoes = it / log(2);
   printf(" it %f ", it);

   float raiz = Bisseccao(a0, b0, eps, interacoes);
   printf("Intervalo final: [%f,%f]\n Solucao aproximada: %f",a0,b0,raiz);

   return 0;
}
