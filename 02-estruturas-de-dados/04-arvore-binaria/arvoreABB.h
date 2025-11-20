#include <stdio.h>
#include <stdlib.h>
#include "pilhaTAD.h"

struct tree
{
	int info;
	struct tree *esq, *dir;
};
typedef struct tree Tree;

//Inicializa uma árvore
void initABB(Tree **raiz)
{
	*raiz = NULL;
}

//Cria um novo nó
Tree *criaNo(int info)
{
	Tree *nova = (Tree*)malloc(sizeof(Tree));
	nova->info = info;
	nova->esq = nova->dir = NULL;
	
	return nova;
}

//Verifica se árvore está vazia
int isEmpty(Tree *raiz)
{
	return raiz == NULL;
}

//Insere um novo nó na árvore
void insereABB(Tree **raiz, int info)
{
    Tree *novo = criaNo(info);

    if (*raiz == NULL)
    {
        *raiz = novo;
        return;
    }

    Tree *atual = *raiz;
    Tree *pai = NULL;

    while (atual != NULL)
    {
        pai = atual;
        if (info < atual->info)
            atual = atual->esq;
        else
            atual = atual->dir;
    }

    if (info < pai->info)
        pai->esq = novo;
    else
        pai->dir = novo;
}

// ----- Percurso em Árvores Binárias -----

//1-Raiz 2-Esquerda 3-Direita
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
			
			printf("%d ", aux->info);
			
			if(aux->dir != NULL)
				push(&P1, aux->dir);
			if(aux->esq != NULL)
				push(&P1, aux->esq);
		}
	}
}

//1-Esquerda 2-Raiz 3-Direita
void in_ordem(Tree *raiz)
{
	if(!isEmpty(raiz))
	{
		Pilha *P;
		init(&P);
		Tree *aux = raiz;
		
		while(!isEmpty(P) || aux != NULL)
		{
			
			while(aux != NULL)
			{
				push(&P, aux);
				aux = aux->esq;
			}
			
			pop(&P, &aux);
			printf("%d ", aux->info);
			
			aux = aux->dir;
		}
	}
}

//1-Esquerda 2-Direita 3-Raiz
void pos_ordem(Tree *raiz)
{
    if (!isEmpty(raiz))
    {
        Pilha *P1, *P2;
        init(&P1);
        init(&P2);

        push(&P1, raiz);
        Tree *aux;

        
        while (!isEmpty(P1))
        {
            pop(&P1, &aux);
            push(&P2, aux);

            if (aux->esq != NULL)
                push(&P1, aux->esq);
            if (aux->dir != NULL)
                push(&P1, aux->dir);
        }

        
        while (!isEmpty(P2))
        {
            pop(&P2, &aux);
            printf("%d ", aux->info);
        }
    }
}
// ----------------------------------

//Deletar todos os nós de uma árvore
void deleteABB(Tree**raiz)
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

//Busca um determinado nó e retorna seu endereço ou NULL caso não encontre
Tree *buscaABB(Tree *raiz, int alvo)
{
    while (raiz != NULL)
    {
        if (alvo == raiz->info)
            return raiz;
        else if (alvo < raiz->info)
            raiz = raiz->esq;
        else
            raiz = raiz->dir;
    }
    return NULL;
}

//Conta a distância até a raiz
int profundidade(Tree *raiz, int info)
{
	int profundidade = 1;
	
	while (raiz != NULL)
    {
        if (info == raiz->info)
            return profundidade;
        else if (info < raiz->info)
            raiz = raiz->esq;
        else
            raiz = raiz->dir;
            
        profundidade++;
    }
    return -1; //caso não ache o nó
}

//Calcula o número máximo de níveis abaixo
int altura(Tree *raiz, int info) {
    Tree *no = buscaABB(raiz, info);
    
    if(no == NULL)
    	return -1;
    
    
    Fila *F;
    initFila(&F);
    
    enqueue(&F, no);
    enqueue(&F, NULL); //NULL marca o fim do nível
    
    int altura = 0;
    
    while(!isEmptyFila(F))
    {
    	Tree *atual;
    	dequeue(&F, &atual);
    	
    	if(atual == NULL)
    	{
    		if(!isEmptyFila(F))
    		{
    			altura++;
    			enqueue(&F, NULL); //marca o próximo nivel
    		}
    	}
    	else
    	{
    		if(atual->esq != NULL)
    			enqueue(&F, atual->esq);
    			
    		if(atual->dir != NULL)
    			enqueue(&F, atual->dir);
    	}
    }
    
    return altura;
}

//Busca o nó cujo o filho tem o valor procurado
Tree *pai(Tree *raiz, int info)
{
	if(raiz == NULL || info == raiz->info)
		return NULL;
		
	else
	{
		Tree *aux = raiz;
		Tree *pai = NULL;
		while(aux != NULL)
		{	
			if(aux->info == info)
				return pai;
				
			pai = aux;
			
			if(info < aux->info)
				aux = aux->esq;
			
			else
				aux = aux->dir;
		}
		return NULL;
	}
}
