#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct reg_lista
{
	struct listagen *cabeca;
	struct listagen *cauda;
};

union info_lista
{
	char info[8];
	struct reg_lista lista;
};

struct listagen
{
	char terminal;
	union info_lista no;
};
typedef struct listagen ListaGen;

ListaGen *CriarT(char *info)
{
	ListaGen *L = (ListaGen*)malloc(sizeof(ListaGen));
	L->terminal = 1;
	strcpy(L->no.info, info);
	return L;
}

char Nula(ListaGen *L)
{
	return L==NULL;
}

char Atomo(ListaGen *L)
{
	return !Nula(L) && L->terminal;
}

ListaGen *Cons(ListaGen *H, ListaGen *T)
{
	if (Atomo(T))
	{
		printf("Cons: Segundo arqumento nao pode ser Atomo!");
		return NULL;
	}
	else
	{
		ListaGen *L = (ListaGen*)malloc(sizeof(ListaGen));
		L->terminal = 0;
		L->no.lista.cabeca = H;
		L->no.lista.cauda = T;
		return L;
	}
}

ListaGen *Head(ListaGen *L)
{
	if(Nula(L) || Atomo(L))
	{
		printf("Head: Nao pode ser nula e nem atomo");
		return NULL;
	}
	return L->no.lista.cabeca;
}

ListaGen *Tail(ListaGen *L)
{
	if(Nula(L) || Atomo(L))
	{
		printf("Tail: Nao pode ser nula e nem atomo");
		return NULL;
	}
	return L->no.lista.cauda;
}

void exibe(ListaGen *L)
{
	if(Atomo(L))
		printf("%s",L->no.info);
	else
	{
		printf("[");
		while(!Nula(L))
		{
			exibe(Head(L));
			L = Tail(L);
			if(!Nula(L))
				printf(",");		
		}
		printf("]");
	}
}

int main()
{
	//[aaa,[bbb,ccc]]
	ListaGen *L;
	L=Cons(CriarT("aaa"),Cons(Cons(CriarT("bbb"),Cons(CriarT("ccc"),NULL)),NULL));
	exibe(L);
}
