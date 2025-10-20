#include <stdio.h>
#include <stdlib.h>

typedef struct no
{
	int info;
	struct pilha *prox;
}No;

typedef struct {
    No *topo;
} Pilha

void init(Pilha *p) {
    p->topo = NULL;
}

char isEmpty(Pilha *p) {
    return p->topo == NULL;
}

void push(Pilha *p, int valor) {
    No *novo = (No*)malloc(sizeof(No));
    
    novo->info = valor;
    novo->prox = p->topo;
    p->topo = novo;
}

void pop(Pilha *p, int *valor) {
    if (vazia(p))
    	printf("\nPilha Vazia!\n");
    else
    {
	    No *temp = p->topo;
	    *valor = temp->info;
	    p->topo = temp->prox;
	    free(temp);
    }
}

void top(Pilha *p, int *valor) {
    if (vazia(p))
    	printf("\nPilha Vazia!\n");
    else
    	*valor = p->topo->info;
}

