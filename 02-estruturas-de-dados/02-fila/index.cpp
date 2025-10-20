#include <stdlib.h>
#include <stdio.h>

struct TpFilaPrioridade
{
	int priori, num;
	struct TpFilaPrioridade *prox;
};

typedef struct TpFilaPrioridade FilaP;


void Init(FilaP **F)
{
	*F = NULL;
}

char isEmpty (FilaP *F)
{
	return F == NULL;
}

void Enqueue (FilaP **F, int x, int p)
{
	FilaP *aux;
	FilaP *ant;
	FilaP *NC = (FilaP*)malloc(sizeof(FilaP));
	
	NC->num = x;
	NC->priori = p;
	NC->prox = NULL;
	
	if(isEmpty(*F))
		*F = NC;
	else
	{
		aux = *F;
		ant = aux;
		while(aux != NULL && p >= aux->priori)
		{
			ant = aux;
			aux = aux->prox;
		}
		
		if(aux != NULL)
		{
			ant->prox = NC;
			NC->prox = aux;
		}
		else
		{
			ant->prox = NC;
			NC->prox = NULL;
		}
	}
}

void Dequeue (FilaP **F,int *x, int *p)
{
	FilaP *aux = *F;
	*x = aux->num;
	*p = aux->priori;
	*F = (*F)->prox;
}

void Exibe (FilaP *F)
{
	FilaP *aux = F;
	while(aux!=NULL)
	{
		printf("%d - ",aux->num);
		aux = aux->prox;
	}
}

int main ()
{
	FilaP *F;
	int x,y;
	
	Init(&F);
	Enqueue(&F,3,1);
	Enqueue(&F,5,1);
	Enqueue(&F,7,1);
	Enqueue(&F,1,2);
	Enqueue(&F,6,2);
	Enqueue(&F,4,3);
	Enqueue(&F,10,10);
	
	Dequeue(&F,&x,&y);
	printf("Num: %d -  Prioridade : %d\n",x,y);
	Dequeue(&F,&x,&y);
	printf("Num: %d -  Prioridade : %d\n",x,y);
	
	Exibe(F);
}
