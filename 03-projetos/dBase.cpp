#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<windows.h>

#define MAX_TAMANHO 100

union UnValor
{
    float ValorN;
    char ValorD[10], ValorL[10];
    char ValorC[50], ValorM[50];
};

struct TpValor
{
    UnValor Valor;
    TpValor *prox;
};

struct TpStatus
{
    char status;
    TpStatus *prox;
};

struct TpCampo
{
    TpValor *Patual, Pdados;
    char FieldName[20];
    char Type;
    int Width, Dec;
    TpCampo *prox;
};

struct TpArq
{
    TpArq *ant, *prox;
    char NomeDBF[30];
    char Data[11];
    char Hora[6];
    TpStatus *PtrStatus;
    TpCampo *PtrCampos;
};

struct dbase
{
    dbase *top, *bottom;
    char und;
    TpArq *arqs;
}; 
typedef struct dbase dBase;

struct TpComando
{
    char info[25];
    TpComando *prox;
};

//==============================================================================

void caixaAlta(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
        str[i] = toupper(str[i]);
}

char isEmpty(dBase ptr)
{
//	return ptr == NULL;
}

void CLEAR()
{
	system("cls");
}

dBase *criaUnidades()
{
	dBase *undC = (dBase*)malloc(sizeof(dBase));
	dBase *undD = (dBase*)malloc(sizeof(dBase));
	
	undC->top = NULL;
	undC->und = 'C';
	undC->arqs = NULL;
	undC->bottom = undD;
	
	undD->top = undC;
	undD->und = 'D';
	undD->arqs = NULL;
	undD->bottom = NULL;
	
	printf("Unidades %c: e %c: criadas.\n", undC->und, undD->und);
	
	return undC;
}

dBase *SET_DEFAULT_TO(char linha[], dBase **unidade)
{
    if (strlen(linha) > 14)
    {
        char pula = linha[15];
        
        if (pula == 'C' && (*unidade)->und == 'D')
            unidade = &(*unidade)->top;
        else if (pula == 'D' && (*unidade)->und == 'C')
            unidade = &(*unidade)->bottom;
    }
    else
        printf("Erro: comando SET DEFAULT TO incompleto!\n");

	printf("Unidade atual: %c:\n", (*unidade)->und);
    return *unidade;
}


//dBase *CREATE()

int verificaComando(char linha[], char comando[])
{
    return strncmp(linha, comando, strlen(comando)) == 0;
}

int main(void)
{
    char linha[MAX_TAMANHO];
    dBase *unidade = criaUnidades();
    printf("Digite um comando dBase: ");
    fflush(stdin);

    // Usando fgets ao invés de gets
    gets(linha);
    
    caixaAlta(linha);
	
	while(strcmp(linha,"EXIT")!=0)
	{
	    if (verificaComando(linha, "SET DEFAULT TO"))
	        unidade = SET_DEFAULT_TO(linha,&unidade);
	        
	    else if (verificaComando(linha, "CREATE"))
	    {
	        //printf("Comando CREATE detectado!\n");
	        // unidade = CREATE(linha, &unidade);
	    }
	    else if (verificaComando(linha, "DIR"))
	    {
	        printf("Comando DIR detectado!\n");
	        // DIR(linha);
	    }
	    else if(verificaComando(linha,"CLEAR"))
	    	CLEAR();
	    else
	    {
	        printf("Nenhum comando detectado\n");
	    }
	    fflush(stdin);
	    gets(linha);
	    caixaAlta(linha);
	}
}

