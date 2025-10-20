//VERSAO final
//CAIO EMERICK E JOAO EDUARDO
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_TAMANHO 100
#define MAX_CAMPOS 50

typedef union {
    float ValorN;
    char ValorD[10];
    char ValorL[10];
    char ValorC[50];
    char ValorM[50];
} UnValor;

typedef struct TpValor {
    UnValor Valor;
    struct TpValor *prox;
} TpValor;

typedef struct TpStatus {
    char status;
    struct TpStatus *prox;
} TpStatus;

typedef struct TpCampo {
    char FieldName[20];
    char Type;
    int Width;
    int Dec;
    struct TpCampo *prox;
} TpCampo;

typedef struct TpRegistro {
    TpValor *valores;
    struct TpRegistro *prox;
    struct TpRegistro *ant;
    char deleted; //T para marcado, F para não marcado
} TpRegistro;


typedef struct TpArq {
    struct TpArq *ant, *prox;
    char NomeDBF[30];
    char Data[11];
    char Hora[9];
    TpStatus *PtrStatus;
    TpCampo *PtrCampos;
    TpRegistro *PtrRegistros;
    TpRegistro *registroAtual;
    int numCampos;
    int numRegistros;
} TpArq;

typedef struct dbase {
    struct dbase *top, *bottom;
    char und;
    TpArq *arqs;
    TpArq *arquivoAtual;
    char deletedStatus; //O significa OFF, N significa ON
} dBase;

void caixaAlta(char *str);
void CLEAR();
dBase *SET_DEFAULT_TO(char linha[], dBase **unidade);
dBase *CREATE(char linha[], dBase *unidade);
void DIR(dBase *unidade);
int USE(char linha[], dBase *unidade);
void liberaMemoria(dBase **unidade);
void adicionarCampo(TpArq *arquivo, char nome[], char tipo, int width, int dec);
void LIST_STRUCTURE(dBase *unidade);
void ADD_FIELD(char linha[], dBase *unidade);
void APPEND(dBase *unidade);
void LIST(dBase *unidade);
int verificaComando(char linha[], char comando[]);
void Skip(dBase *unidade, int n);

void SKIP(dBase *unidade, int n) {
    if (unidade == NULL || unidade->arquivoAtual == NULL) {
        printf("Erro: Nenhum arquivo em uso.\n");
        return;
    }

    TpArq *arq = unidade->arquivoAtual;
    if (arq->PtrRegistros == NULL) {
        printf("Nenhum registro para navegar.\n");
        return;
    }

    TpRegistro *reg = arq->PtrRegistros;
    int posicao = 1; 
    
    while (reg->prox != NULL) {
        reg = reg->prox;
        posicao++;
    }

    while (n > 0 && reg->prox != NULL) {
        reg = reg->prox;
        posicao++;
        n--;
    }
    while (n < 0 && reg->ant != NULL) {
        reg = reg->ant;
        posicao--;
        n++;
    }

    printf("Registro atual: %d\n", posicao);
    
    arq->registroAtual = reg;
}

void DELETE(char linha[], dBase *unidade) {
    if (unidade == NULL || unidade->arquivoAtual == NULL) {
        printf("Erro: Nenhum arquivo em uso.\n");
        return;
    }

    TpArq *arq = unidade->arquivoAtual;
    if (arq->PtrRegistros == NULL) {
        printf("Nenhum registro para marcar.\n");
        return;
    }

    int Num = 1;
    for (int i = 0; linha[i] != '\0'; i++) {
        if (!isdigit(linha[i])) {
            Num = 0;
            break;
        }
    }

    if (!Num) {
        printf("Formato incorreto. Use apenas o ID numérico.\n");
        return;
    }

    float idBusca = atof(linha);
    
    TpRegistro *reg = arq->PtrRegistros;
    int encontrado = 0;
    int posicao = 1;

    while (reg != NULL) {
        TpValor *val = reg->valores;
        TpCampo *campo = arq->PtrCampos;
        
        if (campo != NULL && val != NULL && strcmp(campo->FieldName, "ID") == 0) {
            if (val->Valor.ValorN == idBusca) {
                reg->deleted = 'T';
                printf("Registro com ID %.2f marcado para exclusao.\n", idBusca);
                encontrado = 1;
                break;
            }
        }
        
        reg = reg->prox;
        posicao++;
    }

    if (!encontrado) {
        printf("Registro com ID %.2f nao encontrado.\n", idBusca);
    }
}

void DELETE_ALL(dBase *unidade) {
    int erro = 0;
    
    if (unidade == NULL || unidade->arquivoAtual == NULL) {
        printf("Erro: Nenhum arquivo em uso.\n");
        erro = 1;
    }
    
    if (!erro) {
        TpArq *arq = unidade->arquivoAtual;
        TpRegistro *reg = arq->PtrRegistros;
        int count = 0;
        
        while (reg != NULL) {
            reg->deleted = 'T';
            count++;
            reg = reg->prox;
        }
        
        printf("%d registros marcados para exclusao.\n", count);
    }
}

void RECALL(dBase *unidade) {
    int erro = 0;
    
    if (unidade == NULL || unidade->arquivoAtual == NULL) {
        printf("Erro: Nenhum arquivo em uso.\n");
        erro = 1;
    }
    
    if (!erro) {
        TpArq *arq = unidade->arquivoAtual;
        if (arq->PtrRegistros == NULL) {
            printf("Nenhum registro para reativar.\n");
            erro = 1;
        }
    }
    
    if (!erro) {
        TpArq *arq = unidade->arquivoAtual;
        TpRegistro *reg = arq->PtrRegistros;
        
        while (reg->prox != NULL) {
            reg = reg->prox;
        }
        
        if (reg->deleted == 'T') {
            reg->deleted = 'F';
            printf("Registro reativado.\n");
        } else {
            printf("Registro nao estava marcado para exclusao.\n");
        }
    }
}

void RECALL_ALL(dBase *unidade) {
    int erro = 0;
    
    if (unidade == NULL || unidade->arquivoAtual == NULL) {
        printf("Erro: Nenhum arquivo em uso.\n");
        erro = 1;
    }
    
    if (!erro) {
        TpArq *arq = unidade->arquivoAtual;
        TpRegistro *reg = arq->PtrRegistros;
        int count = 0;
        
        while (reg != NULL) {
            if (reg->deleted == 'T') {
                reg->deleted = 'F';
                count++;
            }
            reg = reg->prox;
        }
        
        printf("%d registros reativados.\n", count);
    }
}

void SET_DELETED(char linha[], dBase *unidade) {
    int erro = 0;
    
    if (strlen(linha) < 12) {
        printf("Formato: SET DELETED ON|OFF\n");
        erro = 1;
    }
    
    if (!erro) {
        char status[4];
        strncpy(status, linha + 11, 3);
        status[3] = '\0';
        caixaAlta(status);
        
        if (strcmp(status, "ON") == 0) {
            unidade->deletedStatus = 'N';
            printf("Registros marcados serao ocultados.\n");
        } else if (strcmp(status, "OFF") == 0) {
            unidade->deletedStatus = 'O';
            printf("Registros marcados serao exibidos.\n");
        } else {
            printf("Parametro invalido. Use ON ou OFF.\n");
        }
    }
}


void PACK(dBase *unidade) {
    int erro = 0;
    
    if (unidade == NULL || unidade->arquivoAtual == NULL) {
        printf("Erro: Nenhum arquivo em uso.\n");
        erro = 1;
    }
    
    if (!erro) {
        TpArq *arq = unidade->arquivoAtual;
        TpRegistro *anterior = NULL;
        TpRegistro *atual = arq->PtrRegistros;
        int count = 0;
        
        while (atual != NULL) {
            if (atual->deleted == 'T') {
                TpRegistro *prox = atual->prox;
                
                // Libera valores
                TpValor *val = atual->valores;
                while (val != NULL) {
                    TpValor *proxVal = val->prox;
                    free(val);
                    val = proxVal;
                }
                
                if (anterior == NULL) {
                    arq->PtrRegistros = prox;
                    if (prox != NULL) {
                        prox->ant = NULL;
                    }
                } else {
                    anterior->prox = prox;
                    if (prox != NULL) {
                        prox->ant = anterior;
                    }
                }
                
                free(atual);
                atual = prox;
                count++;
                arq->numRegistros--;
            } else {
                anterior = atual;
                atual = atual->prox;
            }
        }
        
        printf("%d registros removidos fisicamente.\n", count);
    }
}

void ZAP(dBase *unidade) {
    int erro = 0;
    
    if (unidade == NULL || unidade->arquivoAtual == NULL) {
        printf("Erro: Nenhum arquivo em uso.\n");
        erro = 1;
    }
    
    if (!erro) {
        TpArq *arq = unidade->arquivoAtual;
        TpRegistro *reg = arq->PtrRegistros;
        int count = 0;
        
        while (reg != NULL) {
            TpRegistro *prox = reg->prox;
            
            // Libera valores
            TpValor *val = reg->valores;
            while (val != NULL) {
                TpValor *proxVal = val->prox;
                free(val);
                val = proxVal;
            }
            
            free(reg);
            reg = prox;
            count++;
        }
        
        arq->PtrRegistros = NULL;
        arq->numRegistros = 0;
        printf("Todos os %d registros foram removidos.\n", count);
    }
}

void caixaAlta(char *str) {
    for (int i = 0; str[i] != '\0'; i++)
        str[i] = toupper(str[i]);
}

void CLEAR() {
    system("cls");
}

//destroi
void liberaMemoria(dBase **unidade) {
    if (*unidade == NULL) return;

    dBase *atual = *unidade;
    while (atual != NULL) {
        TpArq *arq = atual->arqs;
        while (arq != NULL) {
            TpArq *proxArq = arq->prox;
            
            // Liberar campos
            TpCampo *campo = arq->PtrCampos;
            while (campo != NULL) {
                TpCampo *proxCampo = campo->prox;
                free(campo);
                campo = proxCampo;
            }
            
            // Liberar registros
            TpRegistro *reg = arq->PtrRegistros;
            while (reg != NULL) {
                TpRegistro *proxReg = reg->prox;
                TpValor *val = reg->valores;
                while (val != NULL) {
                    TpValor *proxVal = val->prox;
                    free(val);
                    val = proxVal;
                }
                free(reg);
                reg = proxReg;
            }
            
            free(arq->PtrStatus);
            free(arq);
            arq = proxArq;
        }
        
        dBase *proxUnid = atual->bottom;
        free(atual);
        atual = proxUnid;
    }
    
    *unidade = NULL;
}


void adicionarCampo(TpArq *arquivo, char nome[], char tipo, int width, int dec) {
    if (arquivo == NULL) return;
    
	//o campo existe ?
    TpCampo *campoExistente = arquivo->PtrCampos;
    while (campoExistente != NULL) {
        if (strcmp(campoExistente->FieldName, nome) == 0) {
            printf("Erro: Campo %s já existe.\n", nome);
            return;
        }
        campoExistente = campoExistente->prox;
    }
    
    TpCampo *novoCampo = (TpCampo*)malloc(sizeof(TpCampo));
    if (novoCampo == NULL) {
        printf("Falha na alocacao de memoria para campo.\n");
        return;
    }
    
    strcpy(novoCampo->FieldName, nome);
    novoCampo->Type = tipo;
    novoCampo->Width = width;
    novoCampo->Dec = dec;
    novoCampo->prox = NULL;
    
    if (arquivo->PtrCampos == NULL) {
        arquivo->PtrCampos = novoCampo;
    } else {
        TpCampo *ultimo = arquivo->PtrCampos;
        while (ultimo->prox != NULL) {
            ultimo = ultimo->prox;
        }
        ultimo->prox = novoCampo;
    }
    
    arquivo->numCampos++;
    printf("Campo %s adicionado com sucesso.\n", nome);
}

// Só funciona se usar o use, se não entra em loop eterno
void LIST_STRUCTURE(dBase *unidade) {
    if (unidade == NULL || unidade->arquivoAtual == NULL) {
        printf("Erro: Nenhum arquivo em uso. Use USE primeiro.\n");
        return;
    }
    
    TpArq *arq = unidade->arquivoAtual;
    printf("Estrutura de %s:\n", arq->NomeDBF);
    printf("-------------------------------------------------\n");
    printf("Nome\t\tTipo\tTam\tDec\n");
    printf("-------------------------------------------------\n");
    
    TpCampo *campo = arq->PtrCampos;
    while (campo != NULL) {
        printf("%-10s\t%c\t%d\t%d\n", 
               campo->FieldName, campo->Type, campo->Width, campo->Dec);
        campo = campo->prox;
    }
    printf("-------------------------------------------------\n");
}


void ADD_FIELD(char linha[], dBase *unidade) {
    if (unidade == NULL || unidade->arquivoAtual == NULL) {
        printf("Erro: Nenhum arquivo em uso. Use USE primeiro.\n");
        return;
    }
    
    char nome[20], tipo;
    int width, dec;
    
    if (sscanf(linha + 9, "%19[^,],%c,%d,%d", nome, &tipo, &width, &dec) < 3) {
        printf("Formato invalido. Use: ADD FIELD nome,tipo,width,dec\n");
        printf("Ex: ADD FIELD IDADE,N,3,0\n");
        return;
    }
    
    tipo = toupper(tipo);
    if (tipo != 'N' && tipo != 'C' && tipo != 'D' && tipo != 'L' && tipo != 'M') {
        printf("Tipo invalido. Use: N (Number), C (Caractere), D (Data), L (Logic), M (Memo)\n");
        return;
    }
    
	//s for um character o dec é validado para 0
    if (tipo != 'N') dec = 0;
    
    adicionarCampo(unidade->arquivoAtual, nome, tipo, width, dec);
}


void APPEND(dBase *unidade) {
    if (unidade == NULL || unidade->arquivoAtual == NULL) {
        printf("Erro: Nenhum arquivo em uso. Use USE primeiro.\n");
        return;
    }
    
    TpArq *arq = unidade->arquivoAtual;
    TpCampo *campo = arq->PtrCampos;
    
    if (campo == NULL) {
        printf("Erro: Nenhum campo definido no arquivo.\n");
        return;
    }
    
    TpRegistro *novoReg = (TpRegistro*)malloc(sizeof(TpRegistro));
    if (novoReg == NULL) {
        printf("Erro: Falha na aloc de memoria para registro.\n");
        return;
    }
    
    novoReg->valores = NULL;
    novoReg->prox = NULL;
    novoReg->ant = NULL;
    novoReg->deleted = 'F'; 
    
    printf("Inserindo novo registro em %s:\n", arq->NomeDBF);
    
    TpCampo *campoAtual = arq->PtrCampos;
    TpValor *ultimoValor = NULL;
    int erro = 0;
    
    while (campoAtual != NULL && !erro) {
        TpValor *novoValor = (TpValor*)malloc(sizeof(TpValor));
        if (novoValor == NULL) {
            printf("Erro: Falha na alocacao de memoria para valor.\n");
            erro = 1;
        }
        
        if (!erro) {
            printf("%s (%c,%d): ", campoAtual->FieldName, campoAtual->Type, campoAtual->Width);
            switch (campoAtual->Type) {
                case 'N': 
                    if (scanf("%f", &novoValor->Valor.ValorN) != 1) {
                        printf("Valor invalido para campo num!\n");
                        erro = 1;
                    }
                    break;
                case 'C': 
                    if (scanf("%49s", novoValor->Valor.ValorC) != 1) {
                        printf("Valor invalido para campo char!\n");
                        erro = 1;
                    }
                    break;
                case 'D':
                    if (scanf("%9s", novoValor->Valor.ValorD) != 1) {
                        printf("Valor invalido para campo data!\n");
                        erro = 1;
                    }
                    break;
                case 'L': 
                    if (scanf("%9s", novoValor->Valor.ValorL) != 1) {
                        printf("Valor invalido para campo logico!\n");
                        erro = 1;
                    }
                    break;
                case 'M': 
                    if (scanf("%49s", novoValor->Valor.ValorM) != 1) {
                        printf("Valor invalido para campo memo!\n");
                        erro = 1;
                    }
                    break;
            }
            
            if (!erro) {
                novoValor->prox = NULL;
                
                if (novoReg->valores == NULL) {
                    novoReg->valores = novoValor;
                } else {
                    ultimoValor->prox = novoValor;
                }
                ultimoValor = novoValor;
            } else {
                free(novoValor);
            }
        }
        
        campoAtual = campoAtual->prox;
    }
    
    if (erro) {
        TpValor *val = novoReg->valores;
        while (val != NULL) {
            TpValor *prox = val->prox;
            free(val);
            val = prox;
        }
        free(novoReg);
        return;
    }
    
    if (arq->PtrRegistros == NULL) {
        arq->PtrRegistros = novoReg;
    } else {
        TpRegistro *ultimoReg = arq->PtrRegistros;
        while (ultimoReg->prox != NULL) {
            ultimoReg = ultimoReg->prox;
        }
        novoReg->ant = ultimoReg;
        ultimoReg->prox = novoReg;
    }
    
    // Atualiza o registro atual
    arq->registroAtual = novoReg;
    arq->numRegistros++;
    
    printf("Registro adicionado com sucesso. Total: %d\n", arq->numRegistros);
    
    while (getchar() != '\n');
}

void LIST(dBase *unidade) {
    if (unidade == NULL || unidade->arquivoAtual == NULL) {
        printf("Erro: Nenhum arquivo em uso. Use USE primeiro.\n");
        return;
    }
    
    TpArq *arq = unidade->arquivoAtual;
    
    if (arq->PtrCampos == NULL) {
        printf("Nenhum campo definido no arquivo.\n");
        return;
    }
    
    printf("Registros de %s:\n", arq->NomeDBF);
    printf("-------------------------------------------------\n");
    
    TpCampo *campo = arq->PtrCampos;
    while (campo != NULL) {
        printf("%-15s", campo->FieldName);
        campo = campo->prox;
    }
    printf("\n-------------------------------------------------\n");
    
    TpRegistro *reg = arq->PtrRegistros;
    int contador = 1;
    int registroMostrado = 0;
    
    while (reg != NULL) {
        //Pode mostrar os delete
        if (!(unidade->deletedStatus == 'N' && reg->deleted == 'T')) {
            printf("%d. ", contador++);
            
            TpValor *val = reg->valores;
            campo = arq->PtrCampos;
            
            while (val != NULL && campo != NULL) {
                switch (campo->Type) {
                    case 'N':
                        printf("%-15.2f", val->Valor.ValorN);
                        break;
                    case 'C':
                        printf("%-15s", val->Valor.ValorC);
                        break;
                    case 'D':
                        printf("%-15s", val->Valor.ValorD);
                        break;
                    case 'L':
                        printf("%-15s", val->Valor.ValorL);
                        break;
                    case 'M':
                        printf("%-15.15s...", val->Valor.ValorM);
                        break;
                }
                
                val = val->prox;
                campo = campo->prox;
            }
            printf("\n");
            registroMostrado = 1;
        }
        reg = reg->prox;
    }
    
    if (!registroMostrado) {
        printf("Nenhum registro encontrado.\n");
    }
    printf("-------------------------------------------------\n");
}


dBase *SET_DEFAULT_TO(char linha[], dBase **unidade) {
    if (strlen(linha) > 14) {
        char pula[MAX_TAMANHO];
        strcpy(pula, linha + 15);
        caixaAlta(pula);

        if (strcmp(pula, "C:") == 0 || strcmp(pula, "D:") == 0) {
            printf("Letra do diretorio: %c\n", pula[0]);

            dBase *auxUnd = (dBase *)malloc(sizeof(dBase));
            if (auxUnd == NULL) {
                printf("Erro: falha na alocacao de memoria!\n");
                return *unidade;
            }

            auxUnd->top = auxUnd->bottom = NULL;
            auxUnd->arqs = NULL;
            auxUnd->arquivoAtual = NULL;
            auxUnd->und = pula[0];

            printf("Unidade %c construida\n", pula[0]);

            if (*unidade != NULL && pula[0] == (*unidade)->und) {
                printf("Unidade ja existente!\n");
                free(auxUnd);
            }
            else if (*unidade == NULL) {
                *unidade = auxUnd;
            }
            else {
                dBase *temp = *unidade;
                while (temp->bottom != NULL) {
                    temp = temp->bottom;
                }
                temp->bottom = auxUnd;
                auxUnd->top = temp;
            }
        }
        else {
            printf("Erro: somente diretorios 'C:' ou 'D:' sao permitidos!\n");
        }
    }
    else {
        printf("Erro: comando SET DEFAULT TO incompleto!\n");
    }

    return *unidade;
}

dBase *CREATE(char linha[], dBase *unidade) {
    if(unidade == NULL) {
        printf("Erro: Nenhuma unidade selecionada. Use SET DEFAULT TO.\n");
        return unidade;
    }
    
    if(strlen(linha) <= 7) {
        printf("Erro: Nome do arquivo nao especificado.\n");
        return unidade;
    }
    
    TpArq *arq = (TpArq*)malloc(sizeof(TpArq));
    if(arq == NULL) {
        printf("Erro: Falha na alocacao de memor.\n");
        return unidade;
    }
    
   
    char nomeArq[30];
    strncpy(nomeArq, linha + 7, 29);
    nomeArq[29] = '\0';
    

    int i = strlen(nomeArq) - 1;
    while(i >= 0 && isspace(nomeArq[i])) {
        nomeArq[i] = '\0';
        i--;
    }
    
    strcpy(arq->NomeDBF, nomeArq);
    strcat(arq->NomeDBF, ".DBF");
    
    //DH
    time_t agora = time(NULL);
    struct tm *info = localtime(&agora);
    strftime(arq->Data, sizeof(arq->Data), "%d/%m/%Y", info);
    strftime(arq->Hora, sizeof(arq->Hora), "%H:%M:%S", info);
    
    arq->PtrCampos = NULL;
    arq->PtrRegistros = NULL;
    arq->numCampos = 0;
    arq->numRegistros = 0;
    
    arq->PtrStatus = (TpStatus*)malloc(sizeof(TpStatus));
    if(arq->PtrStatus != NULL) {
        arq->PtrStatus->status = 'A'; // 'A' de aberto
        arq->PtrStatus->prox = NULL;
    }
    
    arq->ant = arq->prox = NULL;
    
   
    if(unidade->arqs == NULL) {
        unidade->arqs = arq;
    }
    else {
        TpArq *ultimo = unidade->arqs;
        while(ultimo->prox != NULL) {
            ultimo = ultimo->prox;
        }
        ultimo->prox = arq;
        arq->ant = ultimo;
    }
    
    printf("Arquivo %s criado com sucesso.\n", arq->NomeDBF);
    
    // so adiciona para não ficar vazio
    adicionarCampo(arq, "ID", 'N', 8, 0);
    adicionarCampo(arq, "NOME", 'C', 30, 0);
    
    return unidade;
}


void DIR(dBase *unidade) {
    if (unidade == NULL) {
        printf("Erro: Nenhuma unidade selecionada.\n");
        return;
    }
    
    printf("\nDatabase Files in %c:\n", unidade->und);
    printf("-------------------------------------------------\n");
    printf("Nome do Arquivo\t\tData Criacao\tHora\tRegistros\n");
    printf("-------------------------------------------------\n");
    
    TpArq *arq = unidade->arqs;
    while (arq != NULL) {
        printf("%-20s\t%s\t%s\t%d\n", 
               arq->NomeDBF, arq->Data, arq->Hora, arq->numRegistros);
        arq = arq->prox;
    }
    
    if (unidade->arqs == NULL) {
        printf("Nenhum arquivo encontrado.\n");
    }
    printf("-------------------------------------------------\n");
}

// Função USE para selecionar um arquivo
int USE(char linha[], dBase *unidade) {
    if (unidade == NULL) {
        printf("Erro: Nenhuma unidade selecionada.\n");
        return 0;
    }
    
    if (strlen(linha) <= 4) {
        printf("Erro: Nome do arquivo não especificado.\n");
        return 0;
    }
    
    char nomeArq[30];
    strncpy(nomeArq, linha + 4, 29);
    nomeArq[29] = '\0';
    
    //tira o espaco em branco que sobra
    int i = strlen(nomeArq) - 1;
    while (i >= 0 && isspace(nomeArq[i])) {
        nomeArq[i] = '\0';
        i--;
    }
    
    // Adiciona extensão caso não tenha -> tive que colocar caio pq sempre esquecia
    if (strstr(nomeArq, ".DBF") == NULL) {
        strcat(nomeArq, ".DBF");
    }
    
    TpArq *arq = unidade->arqs;
    while (arq != NULL) {
        if (strcmp(arq->NomeDBF, nomeArq) == 0) {
            unidade->arquivoAtual = arq;
            printf("Arquivo %s selecionado para uso.\n", nomeArq);
            return 1;
        }
        arq = arq->prox;
    }
    
    printf("Erro: Arquivo %s  nao encontrado.\n", nomeArq);
    return 0;
}


void Menu() {
    printf("Sistema dBase - Comandos disponiveis:\n");
    printf("SET DEFAULT TO [C:|D:]\n CREATE\n USE\n DIR\n CLEAR\n");
    printf(" ADD FIELD nome,tipo,width,dec\n LIST STRUCTURE\n");
    printf(" APPEND\n LIST\n DELETE\n DELETE ALL\n RECALL\n RECALL ALL\n");
    printf(" SET DELETED ON|OFF\n PACK\n ZAP\n EXIT\n\n");
}


int main(void) {
    char linha[MAX_TAMANHO];
    dBase *unidade = NULL;
    
    Menu();
    
    while(1) {
        printf("\ndBase> "); //imita o prompt
        if (fgets(linha, MAX_TAMANHO, stdin) == NULL) break;
        
        linha[strcspn(linha, "\n")] = '\0'; // Remove a quebra de linha
        if (strlen(linha) == 0) continue;
        
        caixaAlta(linha);
        
        if (strcmp(linha, "EXIT") == 0) {
            break;
        }
        else if (verificaComando(linha, "SET DEFAULT TO")) {
            unidade = SET_DEFAULT_TO(linha, &unidade);
        }
        else if (verificaComando(linha, "CREATE")) {
            unidade = CREATE(linha, unidade);
        }
        else if (verificaComando(linha, "DIR")) {
            DIR(unidade);
        }
        else if (verificaComando(linha, "USE")) {
            USE(linha, unidade);
        }
        else if (verificaComando(linha, "CLEAR")) {
            CLEAR();
        }
        else if (verificaComando(linha, "LIST STRUCTURE")) {
            LIST_STRUCTURE(unidade);
        }
        else if (verificaComando(linha, "ADD FIELD")) {
            ADD_FIELD(linha, unidade);
        }
        else if (verificaComando(linha, "APPEND")) {
            APPEND(unidade);
        }
        else if (verificaComando(linha, "LIST")) {
            LIST(unidade);
        }
         else if (isdigit(linha[0])) {
            DELETE(linha, unidade);
        }
        else if (verificaComando(linha, "DELETE ALL")) {
            DELETE_ALL(unidade);
        }
        else if (verificaComando(linha, "RECALL")) {
            if (strstr(linha, "RECALL ALL") != NULL) {
                RECALL_ALL(unidade);
            } else {
                RECALL(unidade);
            }
        }
        else if (verificaComando(linha, "SET DELETED")) {
            SET_DELETED(linha, unidade);
        }
        else if (verificaComando(linha, "PACK")) {
            PACK(unidade);
        }
        else if (verificaComando(linha, "ZAP")) {
            ZAP(unidade);
        }	
        else {
            Menu();
        }
    }
    
    liberaMemoria(&unidade);
    printf("Sistema encerrado. memoria liberada.\n");
    return 0;
}

int verificaComando(char linha[], char comando[]) {
    return strncmp(linha, comando, strlen(comando)) == 0;
}
