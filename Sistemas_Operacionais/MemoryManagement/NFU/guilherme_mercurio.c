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
	}
	else
	{
		list -> ultimo -> prox = novo;
		list -> ultimo = novo;
	}
 	return list;
}

int buscar (ListaNfu * lst, int v)
{
	Element * p;
 	for (p=lst->primeiro; p!=NULL; p = p->prox) 
	{
 		if (p->mem == v)
 		{
			p->idade = p->idade | global_number;
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
    return;
}

void envelhece(ListaNfu *lst)
{
    Element *aux = lst -> primeiro;
    while(aux!= NULL){
        aux->idade = (aux->idade >> 1);// | (aux->bitR << (bitNumber-1));
        aux = aux->prox;
    }

    return;
}

void print_list(ListaNfu * lst) {
    Element * p = NULL;
    if (lst) {
        for (p = lst -> primeiro; p; p = p -> prox) {
        	int a;
			char b[10];	
			itoa(p->idade,b,2);
            printf("Num: %d Age: %s || ",p->mem,b);
        }
        printf("\n");
    }
}

void lerArq(char *dir,int buffer,int ciclo)
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
	
	while (1)
	{
		if(feof(arquivo)) break;
		fgets(info, 9, arquivo);
		numLido=atoi(info);
		if (!buscar(lst,numLido))
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
			//printf("%d ",numLido);
			//print_list(lst);
			if (count == ciclo || ciclo==1)
			{
				count=0;
				envelhece(lst);
			}
			n_req++; 
			count++;
	}
	printf("\nArquivo %s\nQuadros de memoria: %5d ,Paginas requisitadas %7d: , PagesFault: %5d\n",dir,buffer,n_req,pageFault);
	fclose(arquivo);
	return;
}

void main(){
	int i=0;
	clock_t t0, tf;
	//Variaveis de incialização para a interação
	char diretorio[10][100] ={"arquivo1.txt","arquivo2.txt","arquivo3.txt","arquivo3.txt","arquivo3.txt","arquivo3.txt","arquivo4.txt","arquivo4.txt","arquivo4.txt","arquivo4.txt"};
	int buffer[10]={4,4,128,256,1024,2048,128,256,1024,2048};
	int ciclo[10] ={1,1,20,50,150,500,20,50,150,500}; 
	int bit[10] = {16,16,512,512,512,512,512,512,512,512};
	//------------------------------------------
	for(i=0;i<10;i++) 
	{
		t0 = clock(); //Start do cronometro para medir o turnround
		global_number = bit[i];
		lerArq(diretorio[i],buffer[i],ciclo[i]);
		tf = clock(); //Para cronometro 
		printf("Tempo gasto: %lf s\n", ( (double) (tf - t0) ) / CLOCKS_PER_SEC);
	}
}


