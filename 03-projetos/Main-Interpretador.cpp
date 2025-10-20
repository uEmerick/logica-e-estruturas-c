#include <stdio.h>
#include <conio.h>
#include <conio2.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define TF 100

//#include "TAD-PilhaLg.h" //TAD da pilha para manipular nossa lista generalizada

//estrutura que vai armazenar nossos tokens
struct TpToken
{
	char info[TF];
	struct TpToken *prox;
};
typedef struct TpToken Token;

//lista que vai apontar para a lista de token de cada linha do programa
struct TpPrograma
{
	Token *token;
	struct TpPrograma *ant, *prox;
};
typedef struct TpPrograma Programa;

//estrutura que vai conter as informacoes das nossas variaveis e fazer o papel da RAM
struct TpVariavel
{
	char identificador[TF], valor[TF];
	Programa *ponteiro;
	struct TpVariavel *prox;
	int tipo; // 0 = LET 1 = CONST
};
typedef struct TpVariavel Variavel;

//estrutura que vai conter as informacao para o controle de exibicao do nosso codigo
struct TpControle
{
	int chave, l, c;
	Programa *local;
	struct TpControle *prox;
};
typedef struct TpControle Controle;

//estrutura que vai conter as informacao das functions do nosso programa
struct TpFuncoes
{
	int l;
	Programa *local;
	char function[TF];
	struct TpFuncoes *prox;
};
typedef struct TpFuncoes Funcoes;

//union que vai guardar o valor de retorno de uma funcao
union valorRetorno
{
	char valorString[100];
	float valorFloat;
};

//estrutura que vai guardar todas as informacoes da chamada da function
struct TpRetornoFuncoes
{
	Programa *programa;
	Token *token;
	valorRetorno valorRet;
	int tipoRetorno; // 0 - sem retorno | 1 - float | 2 - string
	struct TpRetornoFuncoes *prox;
};
typedef struct TpRetornoFuncoes retornoFuncoes;

//estrutura que vai controlar a manipulacao de bloco for no nosso programa
struct TpControleFor
{
	int start, stop, step, atual;
	char variavel[30];
	struct TpControleFor *prox;
};
typedef struct TpControleFor controleFor;

//estrutura que vai conter nosso console.log
struct TpListaEncadeada
{
	char info[100];
	struct TpListaEncadeada *prox;
};
typedef struct TpListaEncadeada listaEncadeada;

//estrutura da Lista Generalizada
union info_lista
{
	int valor;
	char operador[3];
	//implementar função tbm!!!
};

struct TpListaGen
{
	char terminal;
	union info_lista no;
	struct TpListaGen *cabeca, *cauda;
};
typedef struct TpListaGen ListaGen;

struct TpChaves
{
	int cont;
	struct TpChaves *prox;
};
typedef struct TpChaves Chaves;


// -- TAD CHAVES -- 
void initChaves(Chaves **chaves)
{
	*chaves = NULL;
}

char isEmptyChaves(Chaves *chaves)
{
	return chaves == NULL;
}

void pushChaves(Chaves **chaves, int cont)
{
	Chaves *novo = (Chaves*)malloc(sizeof(Chaves));
	novo->cont = cont;
	novo->prox = *chaves;
	*chaves = novo;
}

void popChaves(Chaves **chaves, int *cont)
{
	Chaves *aux;
	if(!isEmptyChaves(*chaves))
	{
		aux = *chaves;
		*cont = aux->cont;
		*chaves = aux->prox;
		free(aux);
	}
}


// -- TAD LISTA GEN ---
void initLG(ListaGen **lista)
{
	*lista = NULL;
}


// -- TAD PILHA CONTROLE --

//funcao que vai inicializar nossa pilha de controle
void initC(Controle **c)
{
	*c = NULL;
}

//funcao que verifica se nossa pilha de controle estï¿½ vazia
char isEmptyC(Controle *c)
{
	return c == NULL;
}

//funcao que insere informacao na nossa pilha de controle
void pushC(Controle **c, int chave, Programa *p, int lin, int col)
{
	Controle *nova = (Controle*)malloc(sizeof(Controle));
	nova->chave = chave;
	nova->l = lin;
	nova->c = col;
	nova->local = p;
	nova->prox = *c;
	*c = nova;
}

//funcao que vai retirar as informacao da nossa pilha de controle
Controle popC(Controle **c)
{
	Controle *aux, cAtual;
	if(!isEmptyC(*c))
	{
		cAtual.chave = (*c)->chave;
		cAtual.l = (*c)->l;
		cAtual.c = (*c)->c;
		cAtual.local = (*c)->local;
		aux = *c;
		*c = (*c)->prox;
		free(aux);
	}
	else
		cAtual.chave = -1;
		
	return cAtual;
}

//funcao que envia o ultimo elemento da pilha sem modificar ela
void ultimoC(Controle *c, Controle **auxC)
{
	if(!isEmptyC(c))
	{
		while(!isEmptyC(c->prox))
			c = c->prox;
	}
	*auxC = c;
}

void topC(Controle *c, Controle **auxC)
{
	*auxC = c;
}


// -- TAD FILA LISTA ENCADEADA --

//funcao que inicializa a fila de LE
void initLE(listaEncadeada **le)
{
	*le = NULL;
}

//funcao que aloca uma nova caixinha para fila de LE
listaEncadeada *criaCaixaLE(char *info)
{
	listaEncadeada *nova = (listaEncadeada*)malloc(sizeof(listaEncadeada));
	strcpy(nova->info,info);
	nova->prox = NULL;
	return nova;
}

//funcao que insere informacao na fila de LE
void enqueueLE(listaEncadeada **le, char *info)
{
	listaEncadeada *aux = *le, *nova = criaCaixaLE(info);
	
	if(aux == NULL)
		*le = nova;
	else
	{
		while(aux->prox != NULL)
			aux = aux->prox;
			
		aux->prox = nova;
	}
}

//funcao que retira uma informacao da fila de LE
void dequeueLE(listaEncadeada **le, char *info)
{
	listaEncadeada *aux;
	
	if((*le) == NULL)
		strcpy(info, "");
	else
	{
		strcpy(info, (*le)->info);
		aux = *le;
		*le = (*le)->prox;
		free(aux);
	}
}

//funcao que exibe o conteudo da fila LE
void exibirLE(listaEncadeada *le)
{
	listaEncadeada *aux = le;
	
	if(aux == NULL)
		printf("\nLista encadeada de (console.log) esta vazia!");
	else
	{
		printf("Conteudo lista encadeada: ");
		while(aux != NULL)
		{
			printf("console.log(%s)\n",aux->info);
			aux = aux->prox;
		}
		printf("\n");
	}
	getch();
}


// -- TAD FILA LISTA ENCADEADA --

void initF(Funcoes **f)
{
	*f = NULL;
}

char isEmptyF(Funcoes *f)
{
	return f == NULL;
}

void enqueueF(Funcoes **f, char *fun, int l, Programa *local)
{
	Funcoes *nova = (Funcoes*)malloc(sizeof(Funcoes));
	
	strcpy(nova->function, fun);
	nova->l = l;
	nova->local = local;
	nova->prox = NULL;
	
	if(isEmptyF(*f))
		*f = nova;
	else
	{
		Funcoes *aux = *f;
		while(aux->prox != NULL)
			aux = aux->prox;
		aux->prox = nova;
	}
}


// -- TAD DA PILHA QUE GUARDA O RETORNO DAS FUNCOES

//funcao que inicializa nossa pilha de rotorno das function
void initRF(retornoFuncoes **rf)
{
	*rf = NULL;
}

//funcao que empilha o retorno das function
void pushRF(retornoFuncoes **rf, Programa *programa, Token *token)
{
	retornoFuncoes *nova = (retornoFuncoes*)malloc(sizeof(retornoFuncoes));
	nova->programa = programa;
	nova->token = token;
	nova->tipoRetorno = 0; //indica que nao existe retorno
	nova->prox = *rf;
	*rf = nova;
}

//funcao que verifica se a pilha de retorno de funcoes ta vazia
char isEmptyRF(retornoFuncoes *rf)
{
	return rf == NULL;
}

//funcao que retira os enderecos da nossa pilha de retorno
void popRF(retornoFuncoes **rf, Programa **programa, Token **token)
{
	if(!isEmptyRF(*rf))
	{
		retornoFuncoes *aux;
		aux = *rf;
		*programa = (*rf)->programa;
		*token = (*rf)->token;
		
		*rf = (*rf)->prox;
		free(aux);
	}
}


// -- TAD PILHA VARIAVEL --

//funcao que inicializa a pilha da variavel
void initPV(Variavel **pilhaVar)
{
	*pilhaVar = NULL;
}

//funcao que verifica se a pilha de variavel e nula ou vazia
char isEmptyPV(Variavel *pilhaVar)
{
	return pilhaVar == NULL;
}

//funcao que insere a variavel dentro da nossa pilha de variavel
void pushPV(Variavel **pilhaVar, Variavel auxVar)
{
	Variavel *nova = (Variavel*)malloc(sizeof(Variavel));
	
	strcpy(nova->valor, auxVar.valor);
	strcpy(nova->identificador, auxVar.identificador);
	nova->ponteiro = auxVar.ponteiro;
	nova->tipo = auxVar.tipo;
		
	nova->prox = *pilhaVar;
	*pilhaVar = nova;
}

//funcao que vai retirar uma variavel da nossa pilha de variavel
void popPV(Variavel **pv, Variavel **pvAux)
{
	if(!isEmptyPV(*pv))
	{
		*pvAux = (Variavel*)malloc(sizeof(Variavel));
		Variavel *aux; //vai ser usado para deletar a caixinha
		
		//fazendo o auxiliar que vem por parametro receber as informacoes
		strcpy((*pvAux)->valor, (*pv)->valor);
		strcpy((*pvAux)->identificador, (*pv)->identificador);
		(*pvAux)->ponteiro = (*pv)->ponteiro;
		(*pvAux)->prox = NULL;
		
		aux = *pv;
		*pv = (*pv)->prox;
		free(aux);
	}
}

//funcao que vai tirar uma variavel da pilha pelo identificador (indepentemente se e topo ou nao)
void popIdentificadorPV(Variavel **pv, char *identificador)
{
	Variavel *atual = *pv, *ant = NULL;
	
	while(atual != NULL)
	{
		if(strcmp(atual->identificador, identificador) == 0)
		{
			if(ant == NULL) //primeiro caso
				*pv = atual->prox;
			else
				ant->prox = atual->prox;
			
			free(atual);
			atual = NULL; //para encerrar a repeticao
		}
		else
		{
			ant = atual;
			atual = atual->prox;
		}
	}
}

//funcao que vai retornar o topo da nossa pilha de variavel
void topPV(Variavel *pv, Variavel **pvAux)
{
	*pvAux = pv;
}

//funcao que busca uma variavel na pilha de variavel pelo identificador
Variavel *buscarIdentPV(Variavel *pv, char *identificador)
{
	while(pv != NULL && strcmp(pv->identificador, identificador) != 0)
		pv = pv->prox;
		
	if(pv != NULL)
		return pv;
	return NULL; //nao achou
}

//funcao que busca um token dentro da lista
Programa *buscarToken(Programa *programa, char *tokenBusca)
{
	Token *token;
	
	while(programa != NULL)
	{
		token = programa->token;
		while(token != NULL)
		{
			if(strcmp(token->info, tokenBusca) == 0)
				return programa; // lista do programa que tem o token dentro
			token = token->prox;
		}
		programa = programa->prox;
	}
	return NULL; //nao achou o token
}

//funcao que exibe o nome e o local das funcoes do programa
void exibeFuncoes(Funcoes *funcoes)
{
	int cont = 1;
	while(funcoes != NULL)
	{
		printf("\nFuncao %d: %s",cont,funcoes->function);
		printf("\nLocal: %p\n",funcoes->local);
		cont++;
		funcoes = funcoes->prox;
	}
}

//funcao que cria a caixa do nosso token
Token *CaixaToken(char *info) 
{
    Token *caixa = (Token*)malloc(sizeof(Token));
    strcpy(caixa->info, info);
    caixa->prox = NULL;
    return caixa;
}

//funcao que cria a caixa da nossa lista que vai apontar para lista de tokens
Programa *CaixaPrograma() 
{
    Programa *caixa = (Programa*)malloc(sizeof(Programa));
    caixa->prox = NULL;
    caixa->ant = NULL;
    caixa->token = NULL;
    return caixa;
}

//funcao que vai adicionar um novo token na lista de tokens
void AdicionarToken(Programa *l, char *info) 
{
    Token *caixa = CaixaToken(info);
    if(l->token == NULL)
        l->token = caixa;
	else
	{
        Token *aux = l->token;
        while(aux->prox != NULL)
            aux = aux->prox;
        aux->prox = caixa;
    }
}

//funcao que insere uma caixa de programa na lista para que cada caixa aponte para uma linha de tokens
void AdicionarPrograma(Programa **p, Programa *l)
{
    if(*p == NULL)
        *p = l;
    else
	{
        Programa *aux = *p;
        while(aux->prox != NULL)
            aux = aux->prox;
            
        aux->prox = l;
        l->ant = aux;
    }
}

//funcao que adiciona o token na linha
void AdicionarTokenLinha(Token **token, char *info)
{
	Token *caixa = CaixaToken(info);
	if(*token == NULL)
		*token = caixa;
	else
	{
		Token *aux = *token;
		while(aux->prox != NULL)
			aux = aux->prox;
		aux->prox = caixa;
	}
}

//funcao que verifica se caracter é um operador matematico (utilizado na hora verificação de expressão matematica)
int operadorMatematico(char *caracter)
{
	if(strcmp(caracter,"+")==0 || strcmp(caracter,"-")==0 || 
	strcmp(caracter,"*")==0 || strcmp(caracter,"/")==0 ||
	strcmp(caracter,"%") == 0 || strcmp(caracter,"**") == 0)
	{
		return 1;
	}
		
	return 0;
}

//funcao que verifica a existencia de um operador ou simbolo (utilizado na hora de tokenrizar)
int operador_simbolo(char caracter)
{
	if(caracter == '(' || caracter == ')' || caracter == '+' || caracter == '-' ||
	    caracter == '*' || caracter == '/' || caracter == '=' || caracter == ',' ||
	    caracter == '{' || caracter == '}' || caracter == ';' || caracter == '<' ||
	    caracter == '>' || caracter == '!' || caracter == '&' || caracter == '|')
	    return 1;
	return 0;
}

//funcao que verifica a existencia de operadores composto (utilizado na hora de tokenrizar)
int operadoresComposto(char caracter, char caracter2)
{
	if((caracter == '=' && caracter2 == '=') ||
	    (caracter == '!' && caracter2 == '=') ||
	    (caracter == '<' && caracter2 == '=') ||
	    (caracter == '>' && caracter2 == '=') ||
	    (caracter == '+' && (caracter2 == '=' || caracter2 == '+')) ||
	    (caracter == '-' && (caracter2 == '=' || caracter2 == '-')) ||
	    (caracter == '*' && (caracter2 == '=' || caracter2 == '*')) ||
	    (caracter == '/' && caracter2 == '=') ||
	    (caracter == '&' && caracter2 == '&') ||
	    (caracter == '|' && caracter2 == '|'))
	    return 1;
	return 0;
}

//funcao que verifica a existencia de um identificador (utilizado na hora de tokenzirar)
int identificadorCarac(char caracter)
{
	if(caracter != ' ' && caracter != '\t' && caracter != '\n' &&
		caracter != '(' && caracter != ')' && caracter != '+' &&
	    caracter != '-' && caracter != '*' && caracter != '/' &&
	    caracter != '=' && caracter != ',' && caracter != '{' &&
	    caracter != '}' && caracter != ';' && caracter != '<' &&
	    caracter != '>' && caracter != '!' && caracter != '&' &&
	    caracter != '|' && caracter != '\0' && caracter != '.')
	    return 1;
	return 0;
}

//funcao que verifica que o caracter e um numero
int numeric(char *caracter)
{
	if(caracter[0] >= 48 && caracter[0] <= 57)
		return 1;
	return 0;
}

//funcao para saber o tipo da variavel 
int isTipoVariavel(char *info) 
{
	if(strcmp(info, "const") == 0) //e uma const
		return 1;
	if(strcmp(info, "let") == 0) //e uma let
		return 0;
	return -1; // nao e uma variavel
}


// -------------- FUNCOES QUE TRATA EXPRESSÃO MATEMATICA ------------------

struct pilhaLG
{
	ListaGen *caixa;
	struct pilhaLG *prox;
};
typedef struct pilhaLG PilhaLG;

struct no_pilhaValor
{
	float valor;
	struct no_pilhaValor *prox;
};
typedef struct no_pilhaValor NoPilhaValor;

struct pilhavalor
{
	struct no_pilhaValor *topo;	
};
typedef struct pilhavalor PilhaValor;

struct no_pilhaOperador
{
	char operador[3];
	struct no_pilhaOperador *prox;
};
typedef struct no_pilhaOperador NoPilhaOperador;

struct pilhaoperador
{
	struct no_pilhaOperador *topo;
};
typedef struct pilhaoperador PilhaOperador;

void initPLG(PilhaLG **P)
{
	*P = NULL;
}

char Nula(ListaGen *L)
{
	return L == NULL;
}

char isEmptyPilhaOperador(PilhaOperador * P)
{
	return P -> topo == NULL;
}

void pushPLG(PilhaLG **P, ListaGen *aux)
{
	PilhaLG *nova = (PilhaLG*)malloc(sizeof(PilhaLG));
	
	nova->caixa = aux;
	nova->prox = *P;
	*P = nova;
}

ListaGen popLG(PilhaLG **P, ListaGen **aux)
{
	PilhaLG *removido = (PilhaLG*)malloc(sizeof(PilhaLG));
	
	*aux = (*P)->caixa;
	removido = *P;
	*P = (*P)->prox;
	free(removido);
}

ListaGen *novaProf()
{
	ListaGen *nova = (ListaGen*)malloc(sizeof(ListaGen));
	nova->terminal = 'P';
	nova->no.valor = 0;
	nova->cabeca = nova->cauda = NULL;
	return nova;
}

ListaGen *novaV(int valor)
{
	ListaGen *nova = (ListaGen*)malloc(sizeof(ListaGen));
	nova->terminal = 'V';
	nova->no.valor = valor;
	nova->cabeca = nova->cauda = NULL;
	return nova;
}

ListaGen *novaO(char *operador)
{
	ListaGen *nova = (ListaGen*)malloc(sizeof(ListaGen));
	nova->terminal = 'O';
	strcpy(nova->no.operador,operador);
	nova->cabeca = nova->cauda = NULL;
	return nova;
}

void pushValor(PilhaValor **P, float valor)
{
	no_pilhaValor *novoNo = (no_pilhaValor *)malloc(sizeof(no_pilhaValor));
	novoNo -> valor = valor;
	novoNo -> prox = (*P) -> topo;
	(*P) -> topo = novoNo; 
}

void pushOperador(PilhaOperador **P, char *operador)
{
	NoPilhaOperador *novoNo = (NoPilhaOperador *)malloc(sizeof(NoPilhaOperador));
	strcpy(novoNo->operador, operador);
	novoNo->prox = (*P)->topo;
	(*P)->topo = novoNo;
}

void popValor(PilhaValor **P, float * valor)
{
	*valor = (*P) -> topo -> valor;
	NoPilhaValor *noRemovido = (*P) -> topo;
	(*P) -> topo = (*P) -> topo -> prox;
	free(noRemovido);
}

void popOperador(PilhaOperador **P, char *operador)
{
	strcpy(operador, (*P) -> topo -> operador);
	NoPilhaOperador *noRemovido = (*P) -> topo;
	(*P) -> topo = (*P) -> topo -> prox;
	free(noRemovido);
}

//funcao que busca operadores matematicos (utilizado para tratar caso seja uma expressão matematica)
int procuraOperador(Token* procura)
{
	while(procura != NULL && !operadorMatematico(procura->info))
		procura = procura->prox;
		
	if(procura != NULL)
		return 1;
	return 0;
}

//funcao que destroi a listagen de expressoes matematica
void destroiLista(ListaGen **lista)
{
	 if(*lista != NULL)
	 {
	 	destroiLista(&(*lista)->cabeca);
	 	destroiLista(&(*lista)->cauda);
	 	free(*lista);
	 	*lista = NULL;
	 }
}

//funcao que exibe a listagen de expressoes matematica
void exibeLG(ListaGen *lista)
{
	if(lista == NULL)
		printf("\n\nPonteiro engual a NULL");
	else 
	{
		if(lista->terminal == 'V')
			printf("%d",lista->no.valor);
		else if(lista->terminal == 'O')
			printf("%c",lista->no.operador);
		else if(lista->terminal == 'P')
			printf("(");
		exibeLG(lista->cabeca);
		exibeLG(lista->cauda);
	}		
}

//funcao que constroi a listagen para tratar expressoes matematica
void constroiLG(ListaGen **lista, Token *token)
{
	char auxO[3], auxF[20];
    int auxV;
	ListaGen *nova, *aux;
	PilhaLG *P;
	initPLG(&P);
	
	if(*lista != NULL)
		destroiLista(&(*lista)); //RESETA a lista caso ja foi usada para uma expressao anterior
	
	aux = (*lista);
	while(token!=NULL)
	{
		if(strcmp(token->info,"(")==0)
		{
			nova = novaProf();
			aux->cauda = nova;
			token = token->prox;
			if(numeric(token->info))
            {
                auxV = atoi(token->info);
                nova->cabeca = novaV(auxV);
            }
					
			aux = nova->cabeca;
			pushPLG(&P,nova);
		}
		else 
		if(numeric(token->info))
		{
            auxV = atoi((token)->info);
			nova = novaV(auxV);
			if(aux == NULL)
				(*lista) = aux = nova;
			else
			{
				aux->cauda = nova;
				aux = nova;
			}
		}
		else 
		if(operadorMatematico(token->info))
		{
			strcpy(auxO,token->info);
			nova = novaO(auxO);
			aux->cauda = nova;
			aux = nova;
		}
        /*
        else if(funcao(token->info)
        {
        }
        else if(variavel(token->info)
        {
        }
        */
		else 
		if(strcmp(token->info,")")==0)
		{
			popLG(&P,&aux);
		}	
		token = token->prox;
	}
	exibeLG(*lista);
}

//funcao que inicializa a pilha de operador
void initPOperador(PilhaOperador **P)
{
	*P = (PilhaOperador*)malloc(sizeof(PilhaOperador));
	(*P) -> topo = NULL;
}

//funcao que inicializa a pilha de valor
void initPValor(PilhaValor **P)
{
	*P = (PilhaValor*)malloc(sizeof(PilhaValor));
	(*P) -> topo = NULL;
}

//funcao que define a prioridade dos operadores a ser resolvidos
int prioridade(char *operador) 
{
    if(strcmp(operador, "+") == 0 || strcmp(operador, "-") == 0) 
        return 1;
    else 
	if(strcmp(operador, "*") == 0 || strcmp(operador, "/") == 0 || strcmp(operador, "%") == 0 || strcmp(operador, "//") == 0)
        return 2;
    else 
	if(strcmp(operador, "**") == 0)
        return 3;
    return 0; //para operadores não reconhecidos
}

// %(resto) int ou float só precisa do float pq o int usa %
float restoF(float num, float div)
{
	float res;
	
	res = num/div;
	res = div*(int)res;
	res = num-res;
	
	return res;
}

union  INFO
{
	char str[30];
	int i;
	float f;
};

float Expoen(float nume, float Ex)
{
	INFO res;
	res.f = nume;
	
	if(Ex != 0)
	{
		while(Ex > 1)
		{
			res.f =  res.f * nume;
			Ex--;
		}
	}
	else
	{
		res.f = 1;
	}
	
	return res.f;
}

//funcao que calcula a equacao matematica
float calculaEquacao(ListaGen *caixa) 
{
    PilhaOperador *POperador;
    PilhaValor *PValor;
    
    initPOperador(&POperador); 
    initPValor(&PValor);
    
    char op[3];
    float resultado, subResultado, val1, val2;
    
    while(!Nula(caixa)) 
	{
        if(caixa->terminal == 'V') 
            pushValor(&PValor, caixa->no.valor);
        else 
		if(caixa->terminal == 'P')
        {
        	float subResultado = calculaEquacao(caixa->cabeca);
    		pushValor(&PValor, subResultado);
        }
        else 
		if(caixa->terminal == 'O') 
		{
            
			while(!isEmptyPilhaOperador(POperador) && prioridade(caixa->no.operador) <= prioridade(POperador->topo->operador)) 
			{	
                popOperador(&POperador, op);
                popValor(&PValor, &val1);
                popValor(&PValor, &val2);
                
                if(strcmp(op, "+") == 0)
				    pushValor(&PValor, val2 + val1);
				else 
				if(strcmp(op, "-") == 0)
				    pushValor(&PValor, val2 - val1);
				else 
				if(strcmp(op, "*") == 0)
				    pushValor(&PValor, val2 * val1);
				else 
				if(strcmp(op, "/") == 0)
				    pushValor(&PValor, val2 / val1);
				else
				if(strcmp(op, "%") == 0)
					pushValor(&PValor, restoF(val1, val2)); 
				else 
				if(strcmp(op, "//") == 0)
					pushValor(&PValor, (int)val2/(int)val1);
				else 
				if(strcmp(op, "**") == 0)
				    pushValor(&PValor, Expoen(val2, val1));
            }
            pushOperador(&POperador, caixa->no.operador);
        }
        caixa = caixa->cauda;
    }
    
    while(!isEmptyPilhaOperador(POperador)) 
	{
        popOperador(&POperador, op);
        popValor(&PValor, &val1);
        popValor(&PValor, &val2);
     
        if(strcmp(op, "+") == 0)
		    pushValor(&PValor, val2 + val1);
		else 
		if(strcmp(op, "-") == 0)
		    pushValor(&PValor, val2 - val1);
		else 
		if(strcmp(op, "*") == 0)
		    pushValor(&PValor, val2 * val1);
		else 
		if(strcmp(op, "/") == 0)
		    pushValor(&PValor, val2 / val1);
		else 
		if(strcmp(op, "%") == 0)
			pushValor(&PValor, restoF(val1, val2));
		else 
		if(strcmp(op, "//") == 0)
			pushValor(&PValor, (int)val2/(int)val1);
		else 
		if(strcmp(op, "**") == 0)
		    pushValor(&PValor, Expoen(val2, val1));
    }
    popValor(&PValor, &resultado);
    return resultado;
}

//funcao que busca uma variavel na pilha pelo identificador
 Variavel* buscaVariavel(char *aux, Variavel *P)
{
	Variavel *andar = P;
	while(andar != NULL && strcmp(aux,andar->identificador)!=0)
		andar = andar->prox;
	return andar;
}


// -------------- FUNCOES QUE TRATA O CONSOLE.LOG ------------------

//funcao que verifica se e uma string
int ehString(char *token)
{
	int length = strlen(token);
	
	//verifica se uma string vazia com aspas duplas ou simples
	if(strcmp(token,"\"") == 0  || strcmp(token,"'") == 0)
		return 1;
	
	//verifica se comeca e termina com aspas duplas
	if(length >=2 && token[0] == '\"' && token[length-1] == '\"')
		return 1;
		
	//verifica se comeca e termina com aspas simples
	if(length >=2 && token[0] == '\'' && token[length-1] == '\'')
		return 1;
		
	return 0;
}

//funcao que pula os espacos que tiver
void pulaEspacos(Token **linha)
{
	while(*linha != NULL && (strcmp((*linha)->info," ") == 0 || strcmp((*linha)->info,"\t") == 0))
		*linha = (*linha)->prox;
}

//funcao que verifica qual o tipo da expressao presente no console.log
char tipoExpressao(Token *atual) 
{
    int encontrouOperadorMatematico = 0;
    int concatenacaoString = 0;

    Token *anterior = NULL;

    while(atual != NULL) 
    {
        if(strcmp(atual->info, " ") != 0)
            anterior = atual;

        //operador '+' indica concatenação de strings ou operação matemática
        if(strcmp(atual->info,"+") == 0 && anterior != NULL && atual->prox != NULL) 
        {
            Token *proximo = atual->prox;

            if(ehString(anterior->info) && ehString(proximo->info)) 
                concatenacaoString = 1;
            else
                encontrouOperadorMatematico = 1;
        }
        if(strcmp(atual->info,",") == 0)
		{
			//apenas marca fim do argumento, nada a fazer aqui
		} 
        else 
		if(operadorMatematico(atual->info))
			encontrouOperadorMatematico = 1;
            
        atual = atual->prox;
    }

    if(concatenacaoString == 1) 
        return 'S'; //string ou concatenação
    if(encontrouOperadorMatematico == 1)
        return 'M'; //matematica
    return 'S'; //simples (string, numero, variavel, etc.)
}

//funcao que resolve qual o tipo da expressão presente no meu console.log
Token *resolConLog(Programa *programa, Variavel *pv, Funcoes *funcoes)
{
	char tipoExp, bol, caracter[10];
	Token *novo, *novo2, *linha;
	linha = programa->token;
	Variavel *pvAux;
	initPV(&pvAux);
	
	tipoExp = tipoExpressao(linha);
	if(tipoExp == 'M')
	{
		strcpy(caracter,"R");
		novo = CaixaToken(caracter);
		
		strcpy(caracter,"=");
		novo2 = CaixaToken(caracter);
		novo2->prox = linha;
		
		novo->prox = novo2;
		programa->token = novo;
		
		//atribuicao(programa,&pv,funcao);
		
		popPV(&pv,&pvAux);
		strcpy(novo->info,pvAux->valor);
	}
	else
		novo = linha;
	return novo;
}

//funcao que separa as expressoes dentro de um console.log
Programa *separaExpressoes(Programa *ant, Variavel **pv, Funcoes *funcoes)
{
    Programa *atual = ant;

    if(atual == NULL || atual->token == NULL)
        return atual;

    Token *linha = atual->token, *atr = NULL, *novoT = NULL, *c = NULL, *auxT = NULL, *token = NULL;
    Programa *novaP = NULL, *cabeca = NULL;

    char div[10], aspa, buffer[200];
    int length;

    //pulando console.log
    if(linha != NULL && linha->prox != NULL && strcmp(linha->info, "console") == 0 &&strcmp(linha->prox->info, ".log") == 0)
        linha = linha->prox->prox;

    //pula (
    if(linha != NULL && strcmp(linha->info, "(") == 0)
        linha = linha->prox;

    //pula espacos
    if(linha != NULL)
        pulaEspacos(&linha);

    //pula ' ou "
    if(linha != NULL && (strcmp(linha->info, "'") == 0 || strcmp(linha->info, "\"") == 0))
    {
        linha = linha->prox;
        if(linha != NULL)
            pulaEspacos(&linha);

        while(linha != NULL && strcmp(linha->info, "'") != 0 && strcmp(linha->info, "\"") != 0)
            linha = linha->prox;

        if(linha != NULL)
            linha = linha->prox;
    }

    if(linha != NULL)
        pulaEspacos(&linha);

    // Pulando até encontrar ,
    while(linha != NULL && strcmp(linha->info, ",") != 0)
        linha = linha->prox;

    if(linha != NULL)
        strncpy(div, linha->info, sizeof(div) - 1), div[sizeof(div) - 1] = '\0';

    atr = linha;

    if(linha != NULL)
        linha = linha->prox;
        
    //loop principal
    while(linha != NULL && linha->prox != NULL && strcmp(linha->prox->info, ")") != 0)
    {
        if(linha != NULL)
            pulaEspacos(&linha);
		
        if(strcmp(linha->info, "'") != 0 && strcmp(linha->info, "\"") != 0)
        {
            if(strcmp(linha->info, ",") == 0)
            {
                if(linha != NULL)
                    pulaEspacos(&linha);

                novaP = CaixaPrograma();
                novaP->token = c;
                cabeca = novaP;
                c = NULL;
				
                token = resolConLog(cabeca, *pv, funcoes);
                if(token != NULL && atr != NULL)
                {
                    atr->prox = token;
                    token->prox = linha;
                }

                if(linha != NULL)
                    strcpy(div, linha->info);

                atr = linha;
            }
            else
				AdicionarTokenLinha(&c,linha->info);
        }
        else // strings
		{
		    buffer[0] = '\0'; //zera o buffer
		
		    //guarda o tipo de aspa
		    aspa = linha->info[0];
		    linha = linha->prox;
		
		    //junta tudo ate achar a aspa final
		    while(linha != NULL && !(linha->info[0] == aspa && linha->info[1] == '\0'))
		    {
		        if(strlen(buffer) > 0)
		            strcat(buffer, " "); //adiciona espaco entre tokens
		        strcat(buffer, linha->info);
		        linha = linha->prox;
		    }
		
		    //pula a aspa final
		    if(linha != NULL)
		        linha = linha->prox;
		
		    //cria um unico token com a string completa
		    AdicionarTokenLinha(&c, buffer);
		
		    atr = linha;
		}

        if(linha != NULL)
            linha = linha->prox;
    }
    
    //ultimo token antes do )
    if(linha != NULL && strcmp(linha->info, ")") == 0)
    {
        novaP = CaixaPrograma();
        novaP->token = c;

        cabeca = novaP;
        token = resolConLog(cabeca, *pv, funcoes);
        if(token != NULL && atr != NULL)
        {
            atr->prox = token;
            token->prox = linha;
        }
    }

    return atual;
}

//funcao que trata o console.log
void tratarConLog(Programa *programa, Variavel *pv, char *mensagemPronta)
{
    strcpy(mensagemPronta, "");

    Token *atual = programa->token;
    Variavel *pvAux = NULL;
    char nomeVar[100];

    int flag = 1, length; // flag para controlar o loop

    //pula tabs ou espaços iniciais
    while(atual != NULL && strcmp(atual->info, "\t") == 0)
        atual = atual->prox;

    //verifica se e console.log
    if(atual != NULL && atual->prox != NULL && strcmp(atual->info, "console") == 0 && strcmp(atual->prox->info, ".log") == 0)
    {
        atual = atual->prox->prox; // pular console e .log

        //pular '('
        if(atual != NULL && strcmp(atual->info, "(") == 0)
            atual = atual->prox;

        //loop principal
        while(flag && atual != NULL)
        {
            //pular espacos e tabs
            while(atual != NULL && (strcmp(atual->info, " ") == 0 || strcmp(atual->info, "\t") == 0))
                atual = atual->prox;

            //checa se chegou no final dos argumentos
            if(atual != NULL && strcmp(atual->info, ")") == 0)
                flag = 0;

            if(atual != NULL && flag)
            {
                //ignorar virgula ou +
                if(strcmp(atual->info, ",") == 0 || strcmp(atual->info, "+") == 0)
                {
                    atual = atual->prox; // apenas pula
                }
                else
                {
                    //string literal
                    if(strcmp(atual->info, "\"") == 0 || strcmp(atual->info, "'") == 0)
                    {
                        atual = atual->prox;
                    }
                    else
                    {
                        //numero ou literal direto
                        if(isdigit(atual->info[0]) || atual->info[0] == '-' || atual->info[0] == '.')
                        {
                            strcat(mensagemPronta, atual->info);
                        }
                        else //variavel ou expressao
                        {
                            pvAux = buscaVariavel(atual->info, pv);
                            if(pvAux != NULL)
                                strcat(mensagemPronta, pvAux->valor);
                            else
                                strcat(mensagemPronta, atual->info);
                        }
                        atual = atual->prox;
                    }

                    //adiciona espaço se o próximo token não for ')'
                    if(atual != NULL && strcmp(atual->info, ")") != 0)
                        strcat(mensagemPronta, " ");
                }
            }
        }
        length = strlen(mensagemPronta);
		if(length > 0 && mensagemPronta[length-1] != '"' && mensagemPronta[length-1] != '\'')
		{
		    mensagemPronta[length] = '"';
		    mensagemPronta[length+1] = '\0';
		}
    }
}

//funcao que busca function pelo nome e retorna o endereco dela
Programa *buscaFuncoes(Funcoes *funcoes, char *token)
{
	while(funcoes != NULL && strcmp(funcoes->function,token) != 0)
		funcoes = funcoes->prox;
		
	if(funcoes != NULL && strcmp(funcoes->function,token) == 0)
		return funcoes->local;
	else
		return NULL;
}

//verifica se oque foi passado e uma variavel do programa
char isVariavel(char *aux, Variavel *P)
{
	while(P != NULL && strcmp(aux,P->identificador)!=0)
		P = P->prox;
	if(P != NULL)
		return 1;
	return 0;
}

//funcao que grava um valor float ou string na union
int gravarUnion(valorRetorno *valor, char *num)
{
    if(numeric(num) == 1) //1 float
    {
        valor->valorFloat = atof(num);
        return 1; //indica float
    }
    else //0 string
    {
        strcpy(valor->valorString, num);
        return 0; //indica string
    }		
}

//funcao que controla a condicao de um bloco if
int controleCondicao(TpToken *atual, Variavel *pv)
{
	//vai guardar as duas condições que vamos comparar no if
    char vEsq[30], vDir[30];  
    valorRetorno valorEsq, valorDir;
	int tipoEsq, tipoDir;//vai guardar o tipo da variavel. 1-float | 0-string
    char op[3];
    TpToken *linha = atual;
    int negacao = 0; //vai indicar se a condicao e !num -> seria a negacao de num  
    int se = 0; //vai ser usado para retorna 1 se a condicao for verdadeira e 0 falsa
    
    Variavel *variavel;
	
	linha = linha->prox; //pulando o if
	linha = linha->prox; //pulando (
	
	//verificando se temos uma negacao
	if(linha != NULL && strcmp(linha->info,"!") == 0)
	{
		negacao = 1; 
		linha = linha->prox; //pulando o suposto !
	}
	
	variavel = buscarIdentPV(pv, linha->info);
	if(variavel != NULL) //achei a variavel na pilha
		strcpy(vEsq, variavel->valor);//guardando o primeiro valor para comparacao
	else
		strcpy(vEsq, linha->info);
	linha = linha->prox; //pulando o primeiro valor e chegando na primeira condicao
	
	strcpy(op, linha->info); //copiando o operador porque jaja vamos usar
	linha = linha->prox; //estou no segundo valor
	
	variavel = buscarIdentPV(pv, linha->info);
	if(variavel != NULL)
		strcpy(vDir, variavel->valor);
	else
		strcpy(vDir, linha->info);
	linha = linha->prox; //estamos no )
	
	tipoEsq = gravarUnion(&valorEsq,vEsq); //gravando na union o tipo da esquerda
	tipoDir = gravarUnion(&valorDir,vDir); //gravando na union o tipo da direita
	
	if(tipoEsq == tipoDir)
	{
		if(tipoEsq == 1) //vamos comparar normal
		{
			if(strcmp(op, ">") == 0)
			{
				if(valorEsq.valorFloat > valorDir.valorFloat)
					se = 1;
				else
					se = 0;
			}
			else
			if(strcmp(op, "<") == 0)
			{
				if(valorEsq.valorFloat < valorDir.valorFloat)
					se = 1;
				else
					se = 0;
			}
			else
			if(strcmp(op, ">=") == 0)
			{
				if(valorEsq.valorFloat >= valorDir.valorFloat)
					se = 1;
				else
					se = 0;
			}
			else
			if(strcmp(op, "<=") == 0)
			{
				if(valorEsq.valorFloat <= valorDir.valorFloat)
					se = 1;
				else
					se = 0;
			}
			else
			if(strcmp(op, "==") == 0)
			{
				if(valorEsq.valorFloat == valorDir.valorFloat)
					se = 1;
				else
					se = 0;	
			}
			else
			if(strcmp(op, "!=") == 0)
			{
				if(valorEsq.valorFloat != valorDir.valorFloat)
					se = 1;
				else
					se = 0;
			}
		}
		else //vamos comparar com strcmp
		{
			if(strcmp(op, ">") == 0)
			{
				if(strcmp(valorEsq.valorString, valorDir.valorString) > 0)
					se = 1;
				else
					se = 0;
			}
			else
			if(strcmp(op, "<") == 0)
			{
				if(strcmp(valorEsq.valorString, valorDir.valorString) < 0)
					se = 1;
				else
					se = 0;
			}
			else
			if(strcmp(op, ">=") == 0)
			{
				if(strcmp(valorEsq.valorString, valorDir.valorString) >= 0)
					se = 1;
				else
					se = 0;
			}
			else
			if(strcmp(op, "<=") == 0)
			{
				if(strcmp(valorEsq.valorString, valorDir.valorString) <= 0)
					se = 1;
				else
					se = 0;
			}
			else
			if(strcmp(op, "==") == 0)
			{
				if(strcmp(valorEsq.valorString, valorDir.valorString) == 0)
					se = 1;
				else
					se = 0;	
			}
			else
			if(strcmp(op, "!=") == 0)
			{
				if(strcmp(valorEsq.valorString, valorDir.valorString) != 0)
					se = 1;
				else
					se = 0;
			}	
		}
		return se;
	}
	else
		return -1; //nao podemos comparar uma string com um float
}

//funcao de menu para quando ja estiver executando o programa
char menuAdptado()
{
	printf("\n\n");
	printf("[F9] - Memoria RAM \t"); //F9 = 67
	printf("[F10] - Tela\t"); //F10 = 68
	return getch();
}

//funcao que exibe o codigo .js na tela
void exibeCodigo(char *nomeArquivo)
{
	char linha[200];
	
	FILE *ptr = fopen(nomeArquivo, "r");
	
	if(ptr == NULL)
		printf("\nErro na abertura do arquivo!\n");
	else
	{
		while(fgets(linha, sizeof(linha), ptr)) 
	        printf("%s",linha);
    	fclose(ptr);
	}
}

//funcao que exibe as variaveis e informacoes dentro da memoria ram
void ram(Variavel *pv)
{
	system("cls");
	
	if(pv != NULL)
	{
		printf("\n\t\t----------- MEMORIA RAM ----------\n");
		printf("\n| %-*s | %-*s | %-*s |",20,"Identificador",15,"Valor",20,"Ponteiro");
		//printf("\n|Identificador \t|\t Valor \t|\t Ponteiro|");
		while(pv != NULL)
		{
			printf("\n| %-*s | %-*s | %*p |",20,pv->identificador, 15,pv->valor, 20,pv->ponteiro);
			//printf("|%s \t|\t %s\t|\t %p \t|",pv->identificador,pv->valor, pv->ponteiro);
			pv = pv->prox;
		}
	}
	else
		printf("\nMemoria ram vazia!!!");
	getch();
}

//funcao que executa o programa
void executaPrograma(Programa *programa, Variavel **pv, Funcoes *funcoes, char nomeArquivo[50], listaEncadeada **le)
{
	Variavel auxVar, *auxPilha=NULL;
	
	Token *auxToken, *auxProcura, *linhaAux, *atualToken, *retToken;
	Programa *auxPrograma, *pontConLog, *auxLocalFun, *atualProgm, *retProg; //pontConLog = endereço de onde tem um console.log
	auxPrograma = programa;
	
	//lista encadeada que vai guardar as informacoes dos console.log
	listaEncadeada *listaConLog;
	initLE(&listaConLog);
	
	//lista generalizada para calcular expressoes matematicas
	ListaGen *listaCalcula;
	initLG(&listaCalcula);
	
	//pilha que guarda os retorno das function
	retornoFuncoes *rf;
	initRF(&rf);
	
	//pilha que guarda o contador de chaves para cada estrutura
	Chaves *chaves;
	initChaves(&chaves);
	
	//salvar o tipo de variavel quando declarada
	char auxTipo[7], mensagemPronta[200], ident[TF], valor[TF];
	char nomeVarFun[30], op;
	
	int chave=0, flagFun = 0, flagIF = 0, chaveIF = 0; //contator de chaver para saber quando sair de uma função
	int condicao;
	
	while(auxPrograma != NULL)
	{
		auxToken = auxPrograma->token;
		while(auxToken != NULL)
		{
			//funcao que busca uma function e retorna o local dela
			auxLocalFun = buscaFuncoes(funcoes, auxToken->info);
			
			//verifica se o token é DECLARAÇÃO de variavel LET ou CONST
			if(isTipoVariavel(auxToken->info) == 1 || isTipoVariavel(auxToken->info) == 0) 
			{
				strcpy(auxTipo, auxToken->info); //salvar o tipo da variavel para posteriormente validar e tratar de forma adequada cada tipo
				auxToken = auxToken->prox;
				strcpy(auxVar.identificador, auxToken->info); //atribui o nome da variavel que SEMPRE estara na proxima caixa. Ou seja sempre será: <<tipo>> nome =
				strcpy(nomeVarFun, auxToken->info); //guardando o nome da variavel
				auxToken = auxToken->prox; //esta apontando para o =
				
				auxVar.ponteiro = auxPrograma;
							
				if(strcmp(auxTipo,"let")==0)
					auxVar.tipo = 0;
				else //entao e CONST
					auxVar.tipo = 1;

				auxLocalFun = buscaFuncoes(funcoes, auxToken->prox->info);
				if(auxLocalFun == NULL) //nao achei um function
				{
					auxToken = auxToken->prox;
					auxProcura = auxToken;
					//se tiver um operador(+ - * /) apartir do '=' significa que é uma EXPRESSÃO/CONTA.
					if(procuraOperador(auxProcura))
					{	
						constroiLG(&listaCalcula, auxToken);//preciso construir a listagen a partir do token
						//converte o float para string usando sprintf()
	    				sprintf(auxVar.valor, "%2.f", calculaEquacao(listaCalcula));
					}
					else
					{
						strcpy(auxVar.valor, auxToken->info);
						auxToken = auxToken->prox;
					}

					//estou empilhando tudo menos o valor, o valor vai ser adicionado depois
					pushPV(pv,auxVar);
				}
				else
				{
					//passando um valor vazio por enquanto
					strcpy(auxVar.valor, "");
					//estou empilhando tudo menos o valor, o valor vai ser adicionado depois
					pushPV(pv,auxVar);
				}
			}
		 	else
		 	if(strcmp(auxToken->info,"if") == 0)
			{
			    //retorna 1 - verdadeira || 0 - falsa
			    condicao = controleCondicao(auxToken,*pv); //auxToken == if
			
			    if(condicao == 1) //condicao verdadeira
			    {
			        //flag para saber que executou o if por tanto não vai fazer o else
			        flagIF = 1;
			
			        auxPrograma = auxPrograma->prox; //pulei a condicao do if
			        auxToken = auxPrograma->token;
			    }
			    else
			    {
			        while(auxToken != NULL && strcmp(auxToken->info,"}") != 0)
			        {
			            auxPrograma = auxPrograma->prox;
			            auxToken = auxPrograma->token;
			        }
			
			        auxPrograma = auxPrograma->prox;
			        auxToken = auxPrograma->token; //estamos no suposto else!
			         
			        if(strcmp(auxToken->info, "else") == 0)
			        {
			            auxPrograma = auxPrograma->prox;
			            auxToken = auxPrograma->token;
			        }
			        else
			        	auxPrograma = auxPrograma->ant;
			    }
			}
			else //flagIF == 1 indica que executou o if anterior e precisa pular o else
			if(strcmp(auxToken->info, "else") == 0 && flagIF == 1)
			{
			    //pular o bloco do else inteiro
			    auxPrograma = auxPrograma->prox;
			    if(strcmp(auxPrograma->token->info, "if") != 0)
			    {
			        while(auxPrograma != NULL && strcmp(auxPrograma->token->info, "}") != 0)
			            auxPrograma = auxPrograma->prox; 
			    }
			    auxToken = auxPrograma->token;
			
			    flagIF = 0;
			}
			else
			if(auxToken->prox != NULL && strcmp(auxToken->info,"console") == 0 && strcmp(auxToken->prox->info,".log") == 0) 
			{
				pontConLog = separaExpressoes(auxPrograma, &*pv, funcoes);
				
				linhaAux = pontConLog->token;
				while(linhaAux != NULL && strcmp(linhaAux->info,"\t") == 0 )
					linhaAux = linhaAux->prox;
					
				if(linhaAux != NULL && linhaAux->prox != NULL)
				{
					tratarConLog(pontConLog, *pv, mensagemPronta);
					enqueueLE(&listaConLog, mensagemPronta);
					
					//printf("\n\nMensagem console.log: %s",mensagemPronta);
				}
				pontConLog = NULL;	
			}
			else 
			if(strcmp(auxToken->info,"return") == 0)//return
            {
                auxToken = auxToken->prox; //pulando "return"
                
				//informacoes da variavel que esta sendo retornada na function
                auxPilha = buscaVariavel(auxToken->info,*pv);
                strcpy(ident, auxPilha->identificador);
                strcpy(valor, auxPilha->valor);
                
                //retirando a chamada de function da pilha e voltando para onde chamou
				popRF(&rf,&retProg,&retToken);
				popChaves(&chaves,&chave);
				auxPrograma = retProg;
                auxToken = auxPrograma->token;
                
                //pulando a declaracao da variavel ja que pode ser const num = soma();
                if(strcmp(auxToken->info,"const") == 0 || strcmp(auxToken->info,"let") == 0)
                	auxToken = auxToken->prox;
                
				//buscando a variavel na pilha
				auxPilha = buscaVariavel(auxToken->info,*pv); 
                if(auxPilha != NULL) //achei a variavel
					strcpy(auxPilha->valor, valor);
					
				printf("\nValor existente na varivel %s = %s",auxPilha->identificador,auxPilha->valor);
				getch();
				
				//saindo da chamada da function
				while(strcmp(auxToken->info,")") != 0)
					auxToken = auxToken->prox;
					
            }
            else
			if(auxLocalFun != NULL || !isEmptyRF(rf)) //function - retToken e retProg
            {
                if(auxLocalFun != NULL)
                {
                    pushRF(&rf, auxPrograma, auxToken);
                    auxPrograma = auxLocalFun->prox;
                    auxToken = auxPrograma->token;
                    chave = 0;
                    pushChaves(&chaves,chave);
                    auxLocalFun = NULL;
                }
                
                if(strcmp(auxToken->info,"{") == 0) 
					chaves->cont++;
                else 
				if(strcmp(auxToken->info,"}") == 0) 
					chaves->cont--;

                if(chaves->cont == 0)
                {
                    popRF(&rf, &retProg, &retToken);
                    popChaves(&chaves,&chave);
                    auxPrograma = retProg;
                    auxToken = retToken->prox;
                }
            }
			else
			if(isVariavel(auxToken->info, *pv)) //Busca na pilha para verificar se o token é uma variavel
			{
				auxPilha = buscaVariavel(auxToken->info,*pv);
				if(numeric(auxPilha->valor)) //Variavel é numero, "Int".
				{
					printf("\n\nEntrou no numeric");
						auxToken = auxToken->prox->prox; //Pula o '='
						
					//	if(procuraFuncao())// Procura função nos proximos tokens
					//	{
							
					//	}
					/*	else*/ if(procuraOperador(auxToken))//Procura operador matematico para ver se é expressão matematica
						{
							constroiLG(&listaCalcula, auxToken);//preciso construir a listagen a partir do token
							// Converte o float para string usando sprintf()
		    				sprintf(auxPilha->valor, "%2.f", calculaEquacao(listaCalcula));
						//	alteraValor(auxVariavel, *pv); //Atribuir novo valor a variavel
						}
						else if(numeric(auxToken->info))//SE Não é função e nem expressão então é apenas uma mudança de valor
						{
							strcpy(auxPilha->valor, auxToken->info);
							//alteraValor(auxVariavel, *pv); //Atribuir novo valor a variavel //NAO SEI SE PRECISA
						}
						
				}
		//		else if(array(auxVariavel))
		//		{
		//			auxToken = auxToken->prox;
		//			if(strcmp(auxToken->info,". ))
		//		}
		//		else //Se não é Int e nem Array, então é string
		//		{
		//			
		//		}
		//		auxToken = auxToken->prox;
				
			}
			auxToken = auxToken->prox;	
		}
		op = menuAdptado();//chamando para executar linha linha
		system("cls");
		switch(op)
		{
			case 67: // F9 - memoria ram
				ram(*pv);
					
				break;
					
			case 68:
				exibirLE(listaConLog);
					
				break;
		}
		system("cls");
		exibeCodigo(nomeArquivo);
		
		if(listaCalcula != NULL)
			destroiLista(&listaCalcula);
		auxPrograma = auxPrograma->prox;
	}
	*le = listaConLog;
}

//funcao que le um arquivo que foi passado e armazena os tokens linha a linha
void lerArquivo(char *nomeArquivo, Programa **programa)
{
    FILE *ptr;
    char linha[256];
    Programa *novaLinha;
    char token[256];
    int i, j, contConchete=0;

    ptr = fopen(nomeArquivo, "r");
    if(ptr == NULL) 
        printf("Erro ao abrir o arquivo\n");
    else
    {
    	while(fgets(linha, sizeof(linha), ptr)) 
		{
	        novaLinha = CaixaPrograma();
	        i = 0;
	
	        while(linha[i] != '\0') 
			{
	            //pula espacos, tabs e quebras de linha
	            while(linha[i] == ' ' || linha[i] == '\t' || linha[i] == '\n')
	                i++;
	
	            j = 0;
	
	            //verifica se e um operador ou simbolo isolado
	            if(linha[i] == '.' && !(linha[i+1] >= '0' && linha[i+1] <= '9'))
				{
				    j = 0;
				    token[j++] = linha[i++]; // guarda o ponto
				
				    //guarda até encontrar '(' ou algum delimitador
				    while(linha[i] != '(' && identificadorCarac(linha[i]) == 1)
				        token[j++] = linha[i++];
				
				    token[j] = '\0';
				    AdicionarToken(novaLinha, token);
				}
				else
	            if(operador_simbolo(linha[i]) == 1) 
	            {
	                //verifica operadores compostos (==, !=, <=, >=, +=, -=, *=, /=, **, ++, --, &&, ||)
	                if(operadoresComposto(linha[i], linha[i+1]) == 1) 
	                {
	                    token[0] = linha[i];
	                    token[1] = linha[i+1];
	                    token[2] = '\0';
	                    i += 2;
	                }
					else
					{
	                    token[0] = linha[i++];
	                    token[1] = '\0';
	                }
	
	                AdicionarToken(novaLinha, token);
	            }
	            else 
				if(linha[i] == '[')//Verifica se é um Array
				{
					contConchete++;
					token[j++] = linha[i++];
					while(contConchete != 0)
					{
						if(linha[i] == '[')
							contConchete++;
						if(linha[i] == ']')
							contConchete--;
						token[j++] = linha[i++];
					}
					token[j] = '\0';
					AdicionarToken(novaLinha, token);
				}
				else
				{
	                //le identificadores, numeros, etc.
	                while(identificadorCarac(linha[i]) == 1) 
	                    token[j++] = linha[i++];
	                if(j > 0)
					{
	                    token[j] = '\0';
	                    AdicionarToken(novaLinha, token);
	                }
	            }
	        }
	        AdicionarPrograma(programa, novaLinha);
	    }
    	fclose(ptr);
    }
}

//funcao que exibe o programa
void ExibirPrograma(Programa *programa) 
{
    Programa *linhaAtual = programa;
    Token *tokenAtual = linhaAtual->token;
    int numLinha = 1, contToken=1;

    while(linhaAtual != NULL)
	{
        while(tokenAtual != NULL)
		{
            printf("Token: %d - info: %s \t ",contToken, tokenAtual->info);
            tokenAtual = tokenAtual->prox;
            contToken++;
        }
        printf("\n");
        linhaAtual = linhaAtual->prox;
        tokenAtual = linhaAtual->token;
        numLinha++;
    }
}

//funcao que contem o menu do nosso simulador de execucao
char menu()
{
	printf("\n\n[F7] - Abrir \t"); //F7 = 65
	printf("[F8] - Executar \t"); //F8 = 66
	printf("[F9] - Memoria RAM \t"); //F9 = 67
	printf("[F10] - Tela\t"); //F10 = 68
	printf("\n[ESC] - Sair"); // ESC = 27
	return getch();
}

//funcao que simula a execucao do nosso programa (FALTA FINALIZAR)
void simulaExecucao(Programa **programa, Variavel **pv)
{
	//vai ser usado para o controle de toda a estrutura do programa que esta sendo compilado
	Controle *se, *Rep, *seAux, *repAux, *ifAux, *aux;
	int chaveAtual=0, lin=0, col=0, chave=0, flag=0, l=0, chaveFun=0, funL=0, cont=0;
	
	Programa *atual= NULL, *auxP = NULL;
		
	//ponteiro que vamos usar para ler os tokens e andar no codigo .js
	Variavel *pvAux = *pv;
	Token *linha, *nova, *linha2;
	Funcoes *funcoes, *funAux;
	
	initC(&se); 
	initC(&Rep);
	
	char seVar='0', rep='0', op, nomeArquivo[50];
	nomeArquivo[0] = '\0';
	
	//vai ser utilizado para o controle do for presente no programa .js
	Variavel *var;
	char *variavel;
	char valorStr[30]; //receber a string que foi passada
	int start, stop, step; //valores que vao dentro do for
	controleFor *pFor = NULL;
	
	listaEncadeada *listaPrint, *auxLE; 
	initLE(&listaPrint); 
	
	//chamando o menu de opcoes do programa
	op = menu();
	
	//inicia Fila de funções
	initF(&funcoes);
	
	//repeticao que vai simular a execucao do programa
	while(op != 27)
	{
		system("cls");
		switch(op)
		{
			case 65: //F7 - Abrir arquivo .js
				printf("\nNome do arquivo: ");
				fflush(stdin);
				gets(nomeArquivo);
				
				lerArquivo(nomeArquivo, &*programa);
				
				auxP = *programa;
				op = getch();

				chave = 1;
				while(auxP != NULL && chave > 0)
				{
					chave = 0;
					linha = auxP->token;
					if(linha != NULL && strcmp(linha->info,"function") == 0) //achou uma função
					{
						linha = linha->prox; //esta no nome da function
						enqueueF(&funcoes, linha->info, l, auxP);
						l++;
							
						auxP = auxP->prox;
						linha
						 = auxP->token;
							
						if(linha != NULL && strcmp(linha->info,"{") == 0)
						{
							chave++;
							auxP = auxP->prox;
						}
							
						while(chave > 0) //chave maior que 0 indica que ainda está na function
						{
							linha = auxP->token;
							if(strcmp(linha->info,"{") == 0)
								chave++;
							else
							if(strcmp(linha->info,"}") == 0)
								chave--;
								
							auxP = auxP->prox;
						}
							
						linha = auxP->token;
						if(linha != NULL && strcmp(linha->info,"function") == 0)
							chave = 1;
					}
				}
				atual = auxP; // volta pro início se não achou nada

				break;
			
			case 66: //F8 - executar programa
				
				//destroiPilha(&pvAux); //Tem que RESETAR A PILHA TODA VEZ QUE CHAMAR A FUNÇÃO DNV
				if(nomeArquivo[0] != '\0')
				{
					exibeCodigo(nomeArquivo);
					executaPrograma(atual, &pvAux, funcoes, nomeArquivo, &listaPrint);
				}
				else
					printf("\nVoce ainda nao abriu um arquivo!\n");
				
				op = getch();
				
				break;
				
			case 67: // F9 - memoria ram
			
				if(pvAux != NULL)
					ram(pvAux);
				else
					printf("\nMemoria ram esta vazia!!!\n");
				
				op = getch();
				
				break;
				
			case 68: 
				exibirLE(listaPrint);
				
				op = getch();
				
				break;
			
			case 27:
				printf("\nPrograma encerrado!");
				
				break;
		}
		
		//chamando o menu de opcoes do programa
		system("cls");
		op = menu();
	}
}

int main()
{
	//lista que vai apontar para a lista de tokens
    Programa *l = NULL; //vou passa l como null, porque vai ler o arquivo dentro da simulacao
    
    //lista do tipo pilha que vai guardar todas as informacoes das nossas variaveis
    Variavel *pilhaVar = NULL;

	//Chamando a funcao que vai executar todo nosso programa
	simulaExecucao(&l, &pilhaVar);
    
    return 0;
}
