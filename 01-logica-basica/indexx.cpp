#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct TpRegistro 
{
	char autores[100],titulo_livro[100], editora[50];
	int ano, paginas;
};

struct TpAutor
{
	char nome[30], sobrenome[30];
	struct TpAutor *prox;
};

struct TpListaAutor 
{
	struct TpAutor *prox;
	struct TpListaAutor *pAutor;
};

struct TpLivro
{
	struct TpLivro *ant, *prox;
	char titulo_livro[100];
	int ano, paginas;
	struct TpListaAutor *pListaAutor;
};

struct TpEditora 
{
	char edit[50];
	struct TpEditora *prox;
	struct TpLivro *pLivro;
};

typedef struct TpAutor Autor;
typedef struct TpListaAutor ListaAutor;
typedef struct TpLivro Livro;
typedef struct TpEditora Editora;


Editora *NovaEditora (char editora[50])
{
	Editora *Nova = (Editora*)malloc(sizeof(Editora));
	Nova->prox = NULL;
	Nova->pLivro = NULL;
	strcpy(Nova->edit,editora);
	

	return Nova;
}

Editora *InserirEditora (Editora *inicio, Editora *editora)
{
	Editora *aux;
	if(inicio == NULL)
		inicio = editora;
	else
	{
		aux = inicio;
		while(aux->prox != NULL)
			aux = aux->prox;
		aux->prox =  editora;
	}
	
	return inicio;
}

Editora *BuscarEditora (Editora *inicio,char Nome[50])
{
	Editora *aux =  inicio;
	while(aux != NULL && stricmp(Nome,aux->edit)!= 0)
		aux = aux->prox;
	
	return aux;
		
}

Livro *NovoLivro (TpRegistro reg)
{
	Livro *NovoLivro = (Livro*)malloc(sizeof(Livro));
	NovoLivro->ant = NovoLivro->prox = NULL;
	NovoLivro->pListaAutor = NULL;
	NovoLivro->ano = reg.ano;
	NovoLivro->paginas = reg.paginas;
	strcpy(NovoLivro->titulo_livro,reg.titulo_livro);
	
	return NovoLivro;
}

Livro *InserirLivro (Livro *inicio, Livro *livro)
{
	Livro *aux;
	if(inicio == NULL)
		inicio = livro;
	else
	{
		aux = inicio;
		while(aux->prox != NULL)
			aux = aux->prox;
		aux->prox = livro;
		livro->ant = aux;
	}
	
	return inicio;
}

Livro *BuscarLivro (Editora *inicio,char livro[100])
{
	Editora *AuxEditora = inicio;
	Livro *AuxLivro;
	
	while (AuxEditora != NULL)
	{
		AuxLivro = AuxEditora->pLivro;
		while(AuxLivro != NULL && stricmp(AuxLivro->titulo_livro,livro)!=0)
			AuxLivro = AuxLivro->prox;
		
		if(AuxLivro != NULL) // achou na lista
			return AuxLivro;
			
		AuxEditora = AuxEditora->prox;
	}
	
	return NULL; // caso não encontre
}


Autor *NovoAutor (Autor *inicio,char nome[30],char sobrenome[30])
{
	Autor *aux;
	Autor *NovoAutor = (Autor*)malloc(sizeof(Autor));
	NovoAutor->prox = NULL;
	NovoAutor->nome = nome;
	NovoAutor->sobrenome = sobrenome;
	
	if(inicio == NULL)
		inicio = NovoAutor;
	else
	{
		aux = inicio;
		while(aux->prox != NULL)
			aux = aux->prox;
		
		aux->prox = NovoAutor;
	}
	
	return inicio;
}

char BuscarAutor (Autor *inicio, char nome[30], char sobrenome[30])
{
	Autor *aux = inicio;
	while(aux != NULL && stricmp(nome,aux->nome) != 0 && stricmp(sobrenome,aux->sobrenome)!= 0)
		aux = aux->prox;
	
	return aux != NULL; // true ou false
}



Editora *LerDados (Editora *inicio) // montar estrutura dinâmica
{
	TpRegistro Reg;
	FILE *PtrArq = fopen("livros.dat","wb");
	Editora *editora;
	Livro *livro;
	
	
	fread(&Reg,sizeof(TpRegistro),1,PtrArq);
	while (!feof(PtrArq))
	{
		editora = BuscarEditora(inicio,Reg.editora);
		if(editora == NULL) // sem nenhuma editora
		{
			editora = NovaEditora(Reg.editora);
			inicio = InserirEditora(inicio,editora);
		}
		
		livro = NovoLivro(Reg);
		editora->pLivro = InserirLivro(editora->pLivro,livro);
	}
}
