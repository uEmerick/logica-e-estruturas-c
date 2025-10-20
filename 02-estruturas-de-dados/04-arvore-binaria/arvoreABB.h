#include <stdio.h>
#include "pilhaTAD.h"

struct tree
{
	int info;
	struct tree *esq, *dir;
};
typedef struct tree Tree;

void initABB(Tree **raiz)
{
	return *raiz == NULL;
}

int isEmpty(Tree *raiz)
{
	return raiz == NULL;
}

void insereABB(Tree **raiz, int info)
{
	
}

void pre_ordem(Tree *raiz)
{
	if(!isEmpty(raiz))
	{
		Pilha *P1;
		init(&P1);
		Tree *aux;
		
		push(&P1, raiz);
		
		while(!isEmpty(P1))
		{
			pop(&P1, &aux);
			
			printf("%d ", aux->valor);
			
			if(aux->dir != NULL)
				push(&P1, aux->dir);
			if(aux->esq != NULL)
				push(&P1, aux->esq);
		}
	}
}

void in_ordem(Tree *raiz)
{
	if(!isEmpty(raiz))
	{
		Pilha *P;
		intit(&P);
		Tree *aux = raiz;
		
		while(!isEmpty(P) || aux != NULL)
		{
			
			while(aux != NULL)
			{
				push(&P, aux);
				aux = aux->esq;
			}
			
			pop(&P, &aux);
			printf("%d ", aux->valor);
			
			aux = aux->dir;
		}
	}
}

void deleteTree(Tree**raiz)
{
	if(!isEmpty(*raiz))
	{
		Pilha *P, *P2;
		init(&P); init(&P2);
		
		push(&P, *raiz);
		
		Tree *aux;
		
		while(!isEmpty(P))
		{
			pop(&P, &aux);
			push(&P2, aux);
			
			if(aux->esq != NULL)
				push(&P, aux->esq);
			if(aux->dir != NULL)
				push(&P, aux->dir);
		}
		
		while(!isEmpty(P2))
		{
			pop(&P2, &aux);
			free(aux);
		}
		*raiz = NULL;
	}
}

Tree *buscaABB(Tree *raiz, int alvo)
{
    if (raiz != NULL)
    {
        Pilha *P;
        init(&P);
        push(&P, raiz);
        
        while (!isEmpty(P))
        {
            pop(&P, &raiz);
            
            if (raiz->valor == alvo)
                return raiz;
                
            if (raiz->dir != NULL)
                push(&P, raiz->dir);
            if (raiz->esq != NULL)
                push(&P, raiz->esq);		
        }
    }
    return NULL;
}

void exclusaoNo(Tree **raiz, Tree *e, Tree *pai)
{
    if(e->esq == NULL && e->dir == NULL) //e é folha
    {
        if(e!=pai)
            if(e->info > pai->info)
                pai->dir = NULL;
            else
                pai->esq = NULL;
        else
            *raiz = NULL;
        free(e);
    }
    else if(e->esq == NULL || e->dir == NULL)//e tem 1 filho
    {
        if(e!=pai)
            if(e->info > pai->info)
                if(e->esq!=NULL)
                    pai->dir = e->esq;
                else
                    pai->dir = e->dir;
             else
                if(e->esq!=NULL)
                    pai->esq = e->esq;
                else
                    pai->esq = e->dir;
        else
        {
            if(e->esq!=NULL)
            raiz = raiz->esq;
            else
                raiz = raiz->dir;
            *raiz = NULL;
        }
        free(e);
    }
    else//e tem 2 filhos
    {
        paiSub = e;
        sub = e->dir;
        while(e->esq!=NULL)
        {
            paiSub = sub;
            sub = sub->esq;
        }
        aux = sub->info;
        exclusao(&*raiz, sub, paiSub);
        e->info=aux;
    }

}

void exibe(Tree *raiz, int x, int y, int dist)
{
    if(raiz != NULL)
    {
        gotoxy(x, y);
        printf("%d", raiz->info);
        if(raiz->esq != NULL)
        {
            gotoxy(x-dist/2, y+1);
            printf("/");
        }
        if(raiz->dir != NULL)
        {
            gotoxy(x+dist/2, y+1);
            printf("\\");
        }
        exibe(raiz->esq, x-dist, y+2,dist/2);
        exibe(raiz-dir, x+dist, y+2, dist/2);
    }
}
        }
    }
}
