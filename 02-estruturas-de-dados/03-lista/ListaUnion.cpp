#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct reg_lista{
	struct listagen *head;
	struct listagen *tail;
};

union info_lista{
	char info[8];
	reg_lista lista;
};

struct listagen{
	char terminal;
	union info_lista no;
};
typedef struct listagen ListaGen;

char Nula(ListaGen *L)
{
	return L==NULL;
}

char Atomo(ListaGen *L)
{
	return !Nula(L) && L->terminal;
}
ListaGen *CriaT(char *info)
{
	ListaGen *L = (ListaGen*)malloc(sizeof(ListaGen));
	L->terminal=1;
	strcpy(L->no.info, info);
	return L;
}

ListaGen *Cons(ListaGen *H, ListaGen *T)
{
	if(Atomo(T))
	{
		printf("Cons: segundo argumento nao pode ser atomo!");
		return NULL;
	}
	else
	{
		ListaGen *L = (ListaGen*)malloc(sizeof(ListaGen));
		L->terminal = 1;
		L->no.lista.head=H;
		L->no.lista.tail=T;
		return L;
	}
}
int main()
{
	ListaGen *L;
	L = Cons(CriaT("a"),Cons(Cons(CriaT("b"),Cons(Cons(CriaT("c"),NULL),NULL)),NULL));
}
