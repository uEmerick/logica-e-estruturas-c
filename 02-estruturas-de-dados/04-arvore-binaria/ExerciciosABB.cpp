//EXERCÍCIOS DE FIXAÇÃO (ÁRVORE ABB E AVL)
//FUNÇÕES ITERATIVAS

#include <stdio.h>
#include <stdlib.h>
#include "TreeABB.h"
#include "Pilha.h"

int main(void)
{
	Tree *raiz;
	init(&raiz);
	
	insereABB(&raiz,50);
	insereABB(&raiz,30);
	insereABB(&raiz,20);
	insereABB(&raiz,40);
	insereABB(&raiz,70);
	insereABB(&raiz,80);
	
	printf("EXERCÍCIOS DE FIXAÇÃO (ÁRVORE ABB E AVL)\n\n");
	printf("Exercicio 1: Nivel\n");
	printf("Digite info: ");
	int info; int cont;
	scanf("%d",&info);
	cont = nivel(raiz,info);
	if(cont !=0)
		printf("Nivel: %d\n", cont);
	else
		printf("Nao encontrado!\n");
		
	printf("\nExercicio 2: Pai\n");
	printf("Digite info: ");
	scanf("%d",&info);
	Tree *auxPai; init(&auxPai);
	auxPai = pai(raiz, info);
	printf("Pai de %d: %d\n",info, auxPai->info);
	
	printf("\nExercicio 3: Percursos\n");
	printf("Pre-Ordem:\n");
	pre_ordem(raiz);
	printf("In-Ordem:\n");
	in_ordem(raiz);
	printf("Pos-Ordem:\n");
	pos_ordem(raiz);
	
	return 0;
}
