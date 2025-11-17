#include<stdio.h>
#include<stdlib.h>

#define TF 20

//TAD da pilha para realizar o pre ordem
struct pilha
{
	int info;
	struct pilha *prox;
};
typedef struct pilha Pilha;

void initPilha(Pilha **p)
{
	*p = NULL;
}

char isEmptyPilha(Pilha *p)
{
	return p == NULL;
}

void push(Pilha **p, int info)
{
	Pilha *nova = (Pilha*)malloc(sizeof(Pilha));
	nova->info = info;
	nova->prox = *p;
	*p = nova;
}

void pop(Pilha **p, int *info)
{
	Pilha *aux;
	
	if(!isEmptyPilha(*p))
	{
		aux = *p;
		*info = (*p)->info;
		*p = aux->prox;
		free(aux);
	}
}

//funcao que inicializa o vetor para ser nossa arvore
void inicializarVetor(int vetor[TF])
{
	int i;
	for(i=0; i < TF; i++)
		vetor[i] = -1;
}

//funcao que insere os valores da arvore ABB
void inserirVetor(int vetor[TF], int info)
{
	int tl=0, flag=0;
	
	//verifica se a posicao 0(raiz) é vazia
	if(vetor[tl] == -1)
	{
		vetor[tl] = info;
		flag = 1;
	}
	else
	{
		while(tl < TF && flag == 0)
		{
			if(info < vetor[tl])
				tl = tl * 2+1;
			else
			if(info > vetor[tl])
				tl = tl * 2+2;
				
			if(tl < TF) //ainda pode inserir
			{
				if(vetor[tl] == -1) //posicao vazia para inserir
				{
					vetor[tl] = info;
					flag = 1;
				}
			}
		}
			
		if(flag == 0 && tl >= TF)
			printf("\nNao foi inserido. Vetor cheio!!!");
	}
}

void preOrdem(int vetor[TF])
{
	Pilha *p;
	initPilha(&p);
	int atual=0, esq=0, dir=0;
	
	push(&p, atual);
	
	while(!isEmptyPilha(p))
	{
		pop(&p, &atual);
		
		if(atual <= TF && vetor[atual] != -1)
		{
			printf("%d \t",vetor[atual]);
			
			//verifica se tem elemento na direita e empilha
			dir = atual * 2 + 2;
			esq = atual * 2 + 1;
			if(dir < TF && vetor[dir] != -1)
				push(&p, dir);
			
			//verifica se tem elemento na esquerda e empilha
			if(esq < TF && vetor[esq] != -1)
				push(&p, esq);
		}
	}
}

int main()
{
	int vetor[TF]; //declarando o vetor
	
	//inicializando todas as posições como vazias
	inicializarVetor(vetor);
		
	//inserindo elementos no vetor em forma de ABB
	inserirVetor(vetor, 7);
	inserirVetor(vetor, 4);
	inserirVetor(vetor, 9);
	inserirVetor(vetor, 3);
	inserirVetor(vetor, 5);
	inserirVetor(vetor, 8);
	inserirVetor(vetor, 10);
	
	preOrdem(vetor);
	
	return 0;
}
