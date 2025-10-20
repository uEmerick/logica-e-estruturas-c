#include <stdlib.h>
#include <stdio.h>

struct TpListaDuplaEnc
{
	int info;
	struct TpListaDuplaEnc *prox, *ant;
};
typedef struct TpListaDuplaEnc ListaDupla;

struct TpDescritor 
{	
	ListaDupla *inicio,*fim;
};

typedef struct TpDescritor Descritor;


void Init(Descritor *Descr)
{
	(*Descr).inicio = (*Descr).fim = NULL;
}

char isEmpty(Descritor Descr)
{
	return Descr.inicio == NULL;
}

void InserirInicio(Descritor *Descr,int x)
{
	ListaDupla *NC = (ListaDupla*)malloc(sizeof(ListaDupla));
	NC->ant = NULL;
	NC->info = x;
	NC->prox = (*Descr).inicio;
	
	if(isEmpty(*Descr))
		(*Descr).inicio = (*Descr).fim = NC;
	else
	{
		(*Descr).inicio->ant = NC;
		(*Descr).inicio = NC;
	}	
}

void InserirFim (Descritor *Descr, int x)
{
	ListaDupla *NC = (ListaDupla*)malloc(sizeof(ListaDupla));
	NC->prox = NULL;
	NC->info = x;
	NC->ant = (*Descr).fim;
	
	if(isEmpty(*Descr))
		(*Descr).inicio = (*Descr).fim = NC;
	else
	{
		(*Descr).fim->prox = NC;
		(*Descr).fim =  NC;
	}
}

ListaDupla *Busca (Descritor Descr, int x)
{
	ListaDupla *aux = Descr.inicio;
	
	while(aux != NULL && aux->info != x)
		aux = aux->prox;
		
	return aux;
}

void Excluir (Descritor *Descr,int x)
{
	ListaDupla *aux;
	aux = Busca(*Descr,x);	
		
	if(aux != NULL)
	{		
		if((*Descr).inicio->prox == NULL) //apenas um elemento
			(*Descr).inicio = (*Descr).fim = NULL;
		else
		{
			if((*Descr).inicio == aux) // primeiro da lista
			{
				(*Descr).inicio = aux->prox;
				(*Descr).inicio->ant = NULL;
			}
			else
			{
				if((*Descr).fim == aux)
				{
					(*Descr).fim = aux->ant;
					(*Descr).fim->prox = NULL;
				}
				else
				{
					aux->ant->prox = aux->prox;
					aux->prox->ant = aux->ant;
				}
			}
		}
		free(aux);
	}	
}

void Exibe (Descritor Descr)
{
	ListaDupla *aux = Descr.inicio;
	
	while(aux!=NULL)
	{
		printf("%d - ",aux->info);
		aux = aux->prox;
	}
}
