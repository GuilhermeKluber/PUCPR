/******************************************************************************
* arquivo: <fifo>.c <22/05/2018>
* autor: <Guilherme Kluber Mercurio>
* curso: <Engenharia de controle e automação>
*******************************************************************************/
/* Estrutura para armazenar os frames de memória */

 #include <stdio.h>
 #include <stdlib.h>

struct defMemoria {

	int tamanho;
	int *dados;
	int ultimo;
	int carregado;
	int pageFaults;
	int requisicoes;

};

void criar( struct defMemoria *f, int tam ) { 

	f->tamanho = tam;
	f->dados = (int*) malloc (f->tamanho * sizeof(int));
	f->carregado = 0;
	f->ultimo = -1;
	f->pageFaults=0;
	f->requisicoes=0;

}

void inserir(struct defMemoria *f, int v) {
	if(f->ultimo == f->tamanho-1)
	{
		f->ultimo = -1;
	}
	
    if(f->requisicoes >= f->tamanho)
	{
    	f->pageFaults++;
	}
	else
	{
		f->carregado++;
	}
	
	f->ultimo++;
	f->dados[f->ultimo] = v; // incrementa ultimo e insere
}

int procura(struct defMemoria *f,int num)
{
	int j;
	for(j=0;j<f->tamanho;j++)
	{
		if (f->dados[j] == num)
		{
			return 1;
		}
	}
	return 0;
}

void lerArq(struct defMemoria *f,char *dir)
{
	int numLido,count=0;
	char info[9];
	FILE *arquivo;
	if ((arquivo = fopen(dir, "r")) == NULL)
	{
		printf("Erro ao abrir arquivo!!!\n\n");
		exit(1);
		return;
	}
	while (!feof(arquivo))
	{
			if (!feof(arquivo))
			{	
				fgets(info, 9, arquivo);
				numLido=atoi(info);
				if (!procura(f,numLido))
				{
					inserir(f,numLido);
				}
				f->requisicoes++; 
 				//printf("\n Numero lido: %d Quantidade de Paginas lidas: %d", f->dados[f->ultimo],f->requisicoes);
			}
	}
	fclose(arquivo);
	return;
}

void main(){
	int i=0;
	struct defMemoria FifoManager;
	char diretorio[10][100] ={"arquivo1.txt","arquivo2.txt","arquivo3.txt","arquivo3.txt","arquivo3.txt","arquivo3.txt","arquivo4.txt","arquivo4.txt","arquivo4.txt","arquivo4.txt"};
	int buffer[10]={4,4,64,256,1024,2048,64,256,1024,2048};
	for(i=0;i<10;i++) 
	{
		criar(&FifoManager,buffer[i]);
		lerArq(&FifoManager,diretorio[i]);
		printf ("\nArquivo %s, %5d Paginas, %7d Requisicoes, FIFO: %5d Page Faults",diretorio[i], buffer[i], FifoManager.requisicoes, FifoManager.pageFaults) ;
	}
}


