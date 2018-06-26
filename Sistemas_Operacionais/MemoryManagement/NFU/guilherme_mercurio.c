/******************************************************************************
* arquivo: <nfu>.c <22/05/2018>
* autor: <Guilherme Kluber Mercurio>
* curso: <Engenharia de controle e automação>
*******************************************************************************/
/* Estrutura para armazenar os frames de memória */

 #include <stdio.h>
 #include <stdlib.h>
 #include <time.h>

int global_number;

struct elementos {
 int mem;
 struct elementos* prox;
 int idade;
 int bitR;
};
typedef struct elementos Element;

struct lista {
    Element * primeiro;
    Element * ultimo;
};

typedef struct lista ListaNfu;

ListaNfu * lst_cria (void)
{
	ListaNfu * p = malloc(1 * sizeof( * p));
	p -> primeiro = NULL;
	p -> ultimo = NULL;
 	return p;
}

ListaNfu * inserir (ListaNfu * list, int val)
{
 	Element * novo = (Element *) malloc(sizeof(Element));
	novo->mem = val;
	novo->prox = NULL;
	novo->idade = global_number; 
	if (list -> primeiro == NULL && list -> ultimo == NULL)
	{
		list -> primeiro = novo;
		list -> ultimo = novo;
		list -> ultimo ->bitR = 0;
	}
	else
	{
		list -> ultimo -> prox = novo;
		list -> ultimo = novo;
		list -> ultimo ->bitR = 0;
	}
 	return list;
}

int buscar (ListaNfu * lst, int v,int bitNumber)
{
	Element * p;
 	for (p=lst->primeiro; p!=NULL; p = p->prox) 
	{
 		if (p->mem == v)
 		{
			//p->idade = p->idade >> 1 | (1 << (bitNumber - 1)) ;
			p->bitR=1;
 			return 1;
 		}
 	}
 return 0; 
}

void substitui(ListaNfu *lst,int new)
{
	Element *aux = lst -> primeiro;
	Element *older;
	older->idade = global_number;
    while(aux != NULL){
        if (aux->idade == 0 )
        {
        	aux->mem = new;
			aux->idade = global_number;
			aux->bitR = 0;
			return;	
		}
		else
		{
			if (older->idade > aux->idade)
			{
				older = aux;
			}	
		}
        aux = aux->prox;
    }
    older->mem = new;
	older->idade = global_number;
	older->bitR = 0;
    return;
}

void envelhece(ListaNfu *lst,int bitNumber)
{
    Element *aux = lst -> primeiro;
    while(aux!= NULL){
        aux->idade = (aux->idade >> 1) | (aux->bitR << (bitNumber-1));
        aux->bitR = 0;
        aux = aux->prox;
    }

    return;
}

void print_list(ListaNfu * lst) {
    Element * p = NULL;
    if (lst) {
        for (p = lst -> primeiro; p; p = p -> prox) {
            printf("Num: %d Age: %d || ",p->mem,p->idade);
        }
        printf("\n");
    }
}

void lerArq(char *dir,int buffer,int ciclo,int bitNumber)
{
	int numLido,n_req=0,pageFault=0,Pages=0,count=1;
	ListaNfu * lst;
 	lst = lst_cria();
	char info[50];
	FILE *arquivo;
	
	if ((arquivo = fopen(dir, "r")) == NULL)
	{
		printf("\nErro ao abrir arquivo!!!\n\n");
		exit(1);
		return;
	}
	
	while (!feof(arquivo))
	{
				if(feof(arquivo)) break;
				fscanf(arquivo, "%d", &numLido);
				if (!buscar(lst,numLido,bitNumber))
				{
					if(Pages >= buffer)
					{
						substitui(lst,numLido);
						pageFault++;
					}
					else
					{
						inserir(lst,numLido);
						Pages++;
					}
				}
				if (count == ciclo || ciclo==1)
				{
					count=0;
					envelhece(lst,bitNumber);
				}
				n_req++; 
				count++;
				//print_list(lst);
	}
	printf("\nArquivo %s\nQuadros de memoria: %5d ,Paginas requisitadas %7d: , PagesFault: %5d\n",dir,buffer,n_req,pageFault);
	fclose(arquivo);
	return;
}

void main(){
	int i=0;
	clock_t t0, tf;
	char diretorio[10][100] ={"arquivo1.txt","arquivo2.txt","arquivo3.txt","arquivo3.txt","arquivo3.txt","arquivo3.txt","arquivo4.txt","arquivo4.txt","arquivo4.txt","arquivo4.txt"};
	int buffer[10]={4,4,128,256,1024,2048,128,256,1024,2048};
	int ciclo[10] ={1,1,20,50,150,500,20,50,150,500}; 
	int bit[10] = {5,5,10,10,10,10,10,10,10,10};
	for(i=0;i<10;i++) 
	{
		t0 = clock();
		global_number = pow(2,bit[i]-1);
		lerArq(diretorio[i],buffer[i],ciclo[i],bit[i]);
		tf = clock();
		printf("Tempo gasto: %lf s\n", ( (double) (tf - t0) ) / CLOCKS_PER_SEC);
	}
}


