///Pedro Henrique Macedo de Oliveira

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define TAM 104

typedef struct registro
{
    char nome[20];
    struct lista *mao;
    struct pilha *morto;
    struct lista *mesa[20];
    int bateu;
} registro;

typedef struct cartas
{
    int naipe;
    int numeros;
    int coringa;
} cartas;

typedef struct elemento
{
    cartas carta;
    struct cartas baralho;
    struct elemento *prox;
} elemento;

typedef struct pilha
{
    struct elemento *topo;
    int tam;
} pilha;

typedef struct lista
{
    elemento *inicio;
    int tamanho;
} lista;

int sorteiojogador(registro *jogador);

void sorteiocartas(int *sorteio, int a);

void ordena_embaralha(cartas bolo[]);

pilha *criapilha();

int pilhavazia(pilha *p);

int empilha_monte(pilha *p, cartas c);

int desempilha_monte(pilha *p, cartas *c);

void imprimir_pilha(pilha *p);

lista *crialista();

int listavazia(lista *l);

int inserir_mao(lista *l, cartas v);

int remover_mao(lista *l, cartas *v, int pos);

int empilha_lixo(pilha *p, cartas c);

int desempilha_lixo(pilha *p, cartas *c);

void retira_lixo(registro *jogador, pilha *p, FILE *arquivo);

void imprimir_lixo(pilha *lixo);

int tamanho(lista *l);

int busca_por_chave(lista *l, int naipe, int numero, int *pos);

void entrega_cartas(lista *l, pilha *p, FILE *arquivo);

void compra_monte(registro *jogador, pilha *p, FILE *arquivo);

void imprimi_lista(lista *l);

int empilha_morto(pilha *morto1, cartas c);

int desempilha_morto(pilha *morto1, cartas *c);

void imprimi_pilhamorto(pilha *morto);

void imprime_morto(registro *jogador, pilha *p, FILE *arquivo);

void retira_da_mao_lixo(registro *jogador, pilha *lixo, FILE *arquivo);

void retira_da_mao_pra_mesa(registro *jogador);

int remover_mesa(lista *mesa, cartas *v, int pos);

int inserir_mesa_inicio(lista *mesa, cartas v);

int inserir_mesa_fim(lista *mesa, cartas v);

void remover_mao_inicio(registro *jogador);

void mesa_ordenada(cartas Car[], int tam);

int Conta_ponto_lista(lista *l);

int Pontos_Jogador_Mao(lista *l);

int Pontos_Jogador(registro *jogador);

int main()
{
    FILE *arquivo;
    arquivo = fopen("arquivo.txt", "w");
    if (arquivo == NULL)
    {
        printf("\nErro na abertura do arquivo\n\n");
        exit(1);
    }

    pilha *p = criapilha(); //Cria pilha!
    pilha *lixo = criapilha(); //Cria lixo!
    registro *jogador = (registro *)malloc(2 * sizeof(registro)); //Aloca a struct jogador!
    cartas bolo[TAM];

    printf("ENTRADA DE DADOS!!!\n");

    printf("Insira o nomes dos jogadores: \n");

    setbuf(stdin, NULL);
    gets(jogador[0].nome);

    printf("Insira o nomes dos jogadores: \n");

    setbuf(stdin, NULL);
    gets(jogador[1].nome);

    jogador[0].mao = crialista(); //Cria mao do jogador 1 e 2 tipo lista!
    jogador[1].mao = crialista();

    jogador[0].morto = criapilha(); //Cria morto do jogador 1 e 2 tipo pilha!
    jogador[1].morto = criapilha();

    for (int j = 0; j < 20; j++)
    {
        jogador[0].mesa[j] = crialista(); //Cria um vetor de lista para mesa!
        jogador[1].mesa[j] = crialista();
    }

    int bater = -1;

    int vez = sorteiojogador(jogador); //Variavel [Vez] recebe funcao sorteio de jogador!

    ordena_embaralha(bolo);

    int i;
    for (i = 0; i < TAM; i++) //Empilha monte
    {
        empilha_monte(p, bolo[i]);
    }

    int op;

    fprintf(arquivo, "\nMao 1 jogador:");
    entrega_cartas(jogador[0].mao, p, arquivo); //Entrega as cartas pros 2 jogadores!
    fprintf(arquivo, "\nMao 2 jogador:");
    entrega_cartas(jogador[1].mao, p, arquivo);

    do
    {
        printf("%s", jogador[vez].nome); //Imprime a mao dos jogadores!
        imprimi_lista(jogador[vez].mao);

        do
        {
            printf("\nLIXO");
            imprimir_pilha(lixo);
            printf("\n");

            printf("\nDigite (1) para retirar da Mao:\n");
            printf("\nDigite (2) para comprar Monte:\n");
            printf("\nDigite (3) para comprar Morto:\n");
            printf("\nDigite (4) para Comprar Lixo:\n");
            printf("\nDigite (5) para Colocar na Mesa:\n");
            printf("\nDigite (6) para Bater:\n");
            scanf("%d", &op);

            if (op == 1)
            {
                printf("\n%s\n", jogador[vez].nome);
                imprimi_lista(jogador[vez].mao);
                fprintf(arquivo, "\n%s Retira da mao:", jogador[vez].nome);
                retira_da_mao_lixo(&jogador[vez], lixo, arquivo);
            }

            else if (op == 2)
            {
                printf("\n%s\n", jogador[vez].nome);
                fprintf(arquivo, "\n%s Comprou:", jogador[vez].nome);
                compra_monte(&jogador[vez], p, arquivo);
                fprintf(arquivo, "\n%s Carta descartada apos comprar do monte:", jogador[vez].nome);
                retira_da_mao_lixo(&jogador[vez], lixo, arquivo);
                imprimi_lista(jogador[vez].mao);
            }

            else if (op == 3)
            {
                printf("\n%s\n", jogador[vez].nome);
                fprintf(arquivo, "\n%s Morto:", jogador[vez].nome);
                imprime_morto(&jogador[vez], p, arquivo);
                fprintf(arquivo, "\n%s Carta descartada apos comprar morto:", jogador[vez].nome);
                retira_da_mao_lixo(&jogador[vez], lixo, arquivo);
                imprimi_lista(jogador[vez].mao);
            }

            else if (op == 4)
            {
                fprintf(arquivo, "\n%s Lixo:", jogador[vez].nome);
                retira_lixo(jogador, lixo, arquivo);
                fprintf(arquivo, "\n%s Carta descartada apos comprar o lixo:", jogador[vez].nome);
                retira_da_mao_lixo(&jogador[vez], lixo, arquivo);
                imprimi_lista(jogador[vez].mao);
            }

            else if (op == 5)
            {
                printf("\n%s\n", jogador[vez].nome);
                imprimi_lista(jogador[vez].mao);
                retira_da_mao_pra_mesa(&jogador[vez]);
            }

            else if (op == 6)
            {
                int grupo = 0;
                for (i = 0; i < 20; i++)
                {
                    grupo += Conta_ponto_lista(jogador[vez].mesa[i]);
                }
                if (grupo > 100)
                {
                    printf("\nCONTAGUEM DE PONTOS: \n");
                    jogador[vez].bateu += 1;
                    op = 0;
                }
                else
                {

                    printf("\nNAO POSSUI UMA CANASTRA LIMPA\n\n");
                }
            }

            else if (op == 7)
            {
                op = 0;
            }

        } while (op != 0 && op < 7);

        if (vez == 1)
        {
            vez = 0;
        }
        else
        {
            vez = 1;
        }

    } while (bater == -1);
    fclose(arquivo);
    return 0;
}

int sorteiojogador(registro *jogador) //Sorteia Jogador!
{
    int i;
    srand(time(NULL));
    i = rand() % 2; //Sorteia 2 Jogadores!

    if (i == 0) //Supondo que Inseri o nome Edwaldo(1) e Patricia(2), vai sortear um dos dois e vai mandar em ordem!
    {
        printf("O jogador que ira comecar sera: [%s]!!!\n", jogador[i].nome);
        return 0;
    }
    else
    {
        printf("O jogador que ira comecar sera: [%s]!!!\n", jogador[i].nome);
        return 1;
    }
}

void sorteiocartas(int *sorteio, int a) //Sorteia as 104 Cartas!
{

    srand(time(NULL) + a);

    *sorteio = rand() % TAM;
}

void ordena_embaralha(cartas bolo[]) //Ordena Primeiro e depois Embaralha!
{
    int i, a = 0, pos;

    for (i = 0; i < TAM; i++) //Ordena os 104 tipos de carta!
    {
        if (a < 13)
        {

            bolo[i].naipe = 0 + 1;
            bolo[i].numeros = a + 1;
            a++;
        }
        else if (a < 26)
        {

            bolo[i].naipe = 1 + 1;
            bolo[i].numeros = (a - 13 + 1);
            a++;
        }
        else if (a < 39)
        {

            bolo[i].naipe = 2 + 1;
            bolo[i].numeros = (a - 26 + 1);
            a++;
        }
        else if (a < 52)
        {

            bolo[i].naipe = 3 + 1;
            bolo[i].numeros = (a - 39 + 1);
            a++;

            if (a == TAM / 2)
            {

                a = 0;
            }
        }
    }

    printf("\n");

    int n_repetido;
    cartas aux;

    for (i = 0; i < TAM; i++) //Embaralha as 104 e possiblita somente uma copia de cada!
    {
        do
        {

            sorteiocartas(&pos, a); //Puxa a funcao do sorteio de cartas!
            a++;

        } while (i == pos && n_repetido == pos);

        n_repetido = pos;

        aux.naipe = bolo[i].naipe;
        aux.numeros = bolo[i].numeros;

        bolo[i].naipe = bolo[pos].naipe;
        bolo[i].numeros = bolo[pos].numeros;

        bolo[pos].naipe = aux.naipe;
        bolo[pos].numeros = aux.numeros;
    }
}

pilha *criapilha() //Cria pilha!
{
    pilha *p = (pilha *)malloc(sizeof(pilha));

    if (p != NULL)
    {
        p->topo = NULL;
        p->tam = 0;
    }
    return p;
}

int pilhavazia(pilha *p) //Verifica se a pilha esta vazia!
{
    if (p->topo == NULL)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int empilha_monte(pilha *p, cartas c) //Empilha o Monte!
{
    elemento *novo = (elemento *)malloc(sizeof(elemento));
    if (novo == NULL)
    {
        return 1;
    }
    else
    {

        novo->baralho = c;
        novo->prox = p->topo;
        p->topo = novo;
        p->tam++;
        return 2;
    }
    //printf("\n[%d] [%d]",novo->baralho);
}

int desempilha_monte(pilha *p, cartas *c) //Desempilha Monte!
{
    elemento *temp;
    if (pilhavazia(p) == 0)
    {
        return 1;
    }
    else
    {

        temp = p->topo;
        *c = temp->baralho;
        p->topo = temp->prox;

        free(temp);
    }
    //printf("\n[%d] [%d]",temp->baralho);
    return 2;
}

void imprimir_pilha(pilha *p) //Imprimi pilha no executavel!
{
    elemento *alo;

    alo = p->topo;
    while (alo != NULL)
    {
        printf("\n[%d] [%d]", alo->baralho.naipe, alo->baralho.numeros);
        alo = alo->prox;
    }
}

lista *crialista() //Cria Lista!
{
    lista *l = (lista *)malloc(sizeof(lista));

    if (l != NULL)
    {
        l->inicio = NULL;
    }
    return l;
}

int listavazia(lista *l) //Verifica se esta vazia!
{
    if (l->inicio == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int inserir_mao(lista *l, cartas v) //Insere na mao(No caso isso é uma lista que insere na ultima posicao)!
{
    elemento *novo = (elemento *)malloc(sizeof(elemento));
    if (novo == NULL)
    {
        return 1;
    }
    novo->prox = NULL;
    novo->baralho = v;

    if (listavazia(l) == 1)
    {
        l->inicio = novo;
    }
    else
    {
        elemento *aux;

        aux = l->inicio;
        while (aux->prox != NULL)
        {
            aux = aux->prox;
        }
        aux->prox = novo;
    }
    return 2;
}

int remover_mao(lista *l, cartas *v, int pos) //Remove da mao(No caso é uma lista que retira por posicao)!
{
    if (l == NULL)
    {
        return 1;
    }
    else
    {
        int i = 1;
        elemento *aux, *ultimo;

        aux = l->inicio;

        if (i == pos && aux->prox != NULL)
        {
            *v = aux->baralho;
            l->inicio = aux->prox;

            free(aux);
        }
        else
        {
            while (aux->prox != NULL && i != pos)
            {
                ultimo = aux;
                aux = aux->prox;
                i++;
            }
            ultimo->prox = aux->prox;
            *v = aux->baralho;

            free(aux);
        }
        return 2;
    }
}

int empilha_lixo(pilha *p, cartas c) //Empilha o lixo()!
{
    elemento *lixo_mao = (elemento *)malloc(sizeof(elemento));
    if (lixo_mao == NULL)
    {
        return 1;
    }
    else
    {

        lixo_mao->baralho = c;
        lixo_mao->prox = p->topo;
        p->topo = lixo_mao;

        return 2;
    }
}

int desempilha_lixo(pilha *p, cartas *c) //Desempilha o lixo!
{
    elemento *lixo_off;
    if (pilhavazia(p) == 0)
    {
        return 1;
    }
    else
    {

        lixo_off = p->topo;
        *c = lixo_off->baralho;
        p->topo = lixo_off->prox;

    }
    free(lixo_off);
    return 2;
}

void retira_lixo(registro *jogador, pilha *p, FILE *arquivo) //Compra o lixo completamente!
{
    cartas g;
    int k;

    k = 0;
    while (k < 2)
    {
        for (int i = 0; i < p->tam; i++) //Fiz um for que inicia no zero e vai ate o tamanho do lixo naquele momento!
        {
            desempilha_lixo(p, &g); //Pego o valor das cartas do lixo e armazeno [Z]!
            fprintf(arquivo, "\n[%d] [%d]", g.naipe, g.numeros);
            inserir_mao(jogador[k].mao, g); //E envio para a mao do jogador!
            p->tam--;
        }
        imprimi_lista(jogador[k].mao); //Imprimo a mao do jogador!
        printf("\n");
        k++;
    }
}

void imprimir_lixo(pilha *lixo) //Printa o lixo no executavel!
{
    elemento *helo;

    helo = lixo->topo;
    while (helo != NULL)
    {
        printf("\n[%d] [%d]", helo->baralho.naipe, helo->baralho.numeros);
        helo = helo->prox;
    }
}

int tamanho(lista *l) //Funcao para saber o tamanho da lista!
{
    elemento *cont = l->inicio;
    int contador;

    while (cont != NULL)
    {
        l->inicio = l->inicio->prox;
        contador++;
    }
    return contador;
}

int busca_por_chave(lista *l, int naipe, int numero, int *pos) //Busca a carta e retorna a posicao!
{
    if (l->inicio == NULL)
    {
        return 1;
    }
    elemento *aux = l->inicio;
    int i = 1;

    while (aux != NULL && (aux->baralho.naipe != naipe || aux->baralho.numeros != numero)) //While que serve para controlar a passagem das cartas que sao iguais!
    {
        aux = aux->prox;
        i++;
    }
    if (aux == NULL)
    {
        return 1;
    }
    else
    {
        *pos = i;
        return 2;
    }
}

void entrega_cartas(lista *l, pilha *p, FILE *arquivo) //Funcao para entregar as 11 primeiras cartas!
{
    cartas m;
    int i;

    for (i = 0; i < 11; i++) //Desempilha 11 cartas do monte e inserem na mao!
    {
        desempilha_monte(p, &m);
        fprintf(arquivo, "[%d] [%d]  ", m.naipe, m.numeros);
        inserir_mao(l, m);
    }
}

void compra_monte(registro *jogador, pilha *p, FILE *arquivo) //Funcao para comprar somente uma carta do monte!
{
    cartas a;
    int i;

    for (i = 0; i < 1; i++)
    {
        desempilha_monte(p, &a);
        fprintf(arquivo, "\n[%d] [%d]", a.naipe, a.numeros);
        inserir_mao(jogador->mao, a);
    }
    imprimi_lista(jogador->mao);
    printf("\n");
}

void imprimi_lista(lista *l) //Imprimi lista no executavel!
{
    elemento *ola;

    ola = l->inicio;

    while (ola != NULL)
    {
        printf("\n[%d][%d]", ola->baralho.naipe, ola->baralho.numeros);
        ola = ola->prox;
    }
    printf("\n");
}

int empilha_morto(pilha *morto1, cartas c) //Empilha Morto!
{
    elemento *renovo = (elemento *)malloc(sizeof(elemento));

    if (renovo == NULL)
    {
        return 1;
    }
    else
    {
        renovo->baralho = c;
        renovo->prox = morto1->topo;
        morto1->topo = renovo;

        return 2;
    }
}

int desempilha_morto(pilha *morto1, cartas *c) //Desempilha Morto!
{
    elemento *retemp;

    if (pilhavazia(morto1) == 0)
    {
        return 1;
    }
    else
    {
        retemp = morto1->topo;
        *c = retemp->baralho;
        morto1->topo = retemp->prox;

        free(retemp);
    }
    return 2;
}

void imprimi_pilhamorto(pilha *morto) //Imprime morto no Executavel!
{
    elemento *oi;

    oi = morto->topo;

    while (oi != NULL)
    {
        printf("\n[%d] [%d]\n", oi->baralho.naipe, oi->baralho.numeros);
        oi = oi->prox;
    }
}

void imprime_morto(registro *jogador, pilha *p, FILE *arquivo) //Funcao para comprar as 11 cartas do morto!
{
    cartas r;
    int i;

    for (i = 0; i < 11; i++) //Desempilha do monte 11 cartas e empilho no morto, apos isso se insere na mao do jogador!
    {
        desempilha_monte(p, &r);
        fprintf(arquivo, "[%d] [%d] ", r.naipe, r.numeros);
        empilha_morto(jogador->morto, r);
        inserir_mao(jogador->mao, r);
    }

    imprimi_pilhamorto(jogador->morto); //Imprime morto!

    printf("\n");
}

void retira_da_mao_lixo(registro *jogador, pilha *lixo, FILE *arquivo) //Retira a cartas da mao e envia para o lixo!
{
    cartas b;

    int pos;

    int naipes, numeros; //Pergunta o naipe e numero!

    printf("\n\nInsira o naipe:\n1)-copas\n2)-Paus\n3)-Espadas\n4)-Ouros\n");
    scanf("%d", &naipes);
    printf("\nInsira o numero:\n1)-AS\n2)-Dois\n3)-Tres\n4)-Quatro\n5)-Cinco\n6)-Seis\n7)-Sete\n8)-Oito\n9)-Nove\n10)-Dez\n11)-[J]\n12)-[Q]\n13)-[K]\n");
    scanf("%d", &numeros);

    busca_por_chave(jogador->mao, naipes, numeros, &pos); //Busca e retorna posicao!

    remover_mao(jogador->mao, &b, pos); //Remove da mao e armazena em [B] e empilha no lixo!
    fprintf(arquivo, "[%d] [%d] ", b.naipe, b.numeros);
    empilha_lixo(lixo, b);

    printf("\nRETIRADO: ");
    imprimir_lixo(lixo); //Imprime lixo!
    printf("\n");
    imprimi_lista(jogador->mao); //Imprime lista!
    printf("\n");
}

int inserir_mesa_fim(lista *mesa, cartas v) //Insere no fim da mesa!
{
    elemento *novo = (elemento *)malloc(sizeof(elemento));
    if (novo == NULL)
    {
        return 1;
    }
    novo->prox = NULL;
    novo->baralho = v;

    if (listavazia(mesa) == 1)
    {
        mesa->inicio = novo;
    }
    else
    {
        elemento *aux;

        aux = mesa->inicio;
        while (aux->prox != NULL)
        {
            aux = aux->prox;
        }
        aux->prox = novo;
    }

    return 2;
}

int inserir_mesa_inicio(lista *mesa, cartas v) //Insere no inicio da mesa!
{
    elemento *novo = (elemento *)malloc(sizeof(elemento));

    if (novo == NULL)
    {
        return 1;
    }
    novo->prox = NULL;
    novo->baralho = v;

    if (listavazia(mesa) == 1)
    {
        mesa->inicio = novo;
        novo->prox = NULL;
    }
    else
    {
        novo->prox = mesa->inicio;
        mesa->inicio = novo;
    }
    return 2;
}

int remover_mesa(lista *mesa, cartas *v, int pos) //Remove por posicao da mao!
{
    if (mesa == NULL)
    {
        return 1;
    }
    else
    {
        int i = 1;
        elemento *aux, *ultimo;

        aux = mesa->inicio;

        if (i == pos && aux->prox != NULL)
        {
            *v = aux->baralho;
            mesa->inicio = aux->prox;

            free(aux);
        }
        else
        {
            while (aux->prox != NULL && i != pos)
            {
                ultimo = aux;
                aux = aux->prox;
                i++;
            }
            ultimo->prox = aux->prox;
            *v = aux->baralho;

            free(aux);
        }
        return 2;
    }
}

void retira_da_mao_pra_mesa(registro *jogador) //Menu da mesa!
{
    int op, i;
    int naipes, numeros;
    int pos, valor, opcion;

    printf("\nDigite (1) para inserir as 3 primeiras cartas: \n");
    printf("\nDigite (2) para inserir quantas cartas quiser: \n");
    scanf("%d", &opcion);

    cartas *Car = (cartas *)malloc(3 * sizeof(cartas));

    do
    {
        if (opcion == 1)
        {
            for (i = 0; i < 3; i++)
            {
                printf("\n\nInsira o naipe:\n1)-copas\n2)-Paus\n3)-Espadas\n4)-Ouros\n");
                scanf("%d", &naipes);
                printf("\nInsira o numero:\n1)-AS\n2)-Dois\n3)-Tres\n4)-Quatro\n5)-Cinco\n6)-Seis\n7)-Sete\n8)-Oito\n9)-Nove\n10)-Dez\n11)-[J]\n12)-[Q]\n13)-[K]\n");
                scanf("%d", &numeros);
                printf("\nInsira a mesa:\n");
                scanf("%d", &valor);

                busca_por_chave(jogador->mao, naipes, numeros, &pos); //Busca as cartas e retorna posicao!

                remover_mesa(jogador->mao, &Car[i], pos); //Remove da mao e insere no fim!
                Car[i].coringa = 0;
            }
            mesa_ordenada(Car, 3);
        }

        if (opcion == 2)
        {
            int tam;

            printf("\nDigite quantas cartas quer mandar pra mesa: \n");
            scanf("%d", &tam);
            for (i = 0; i < tam; i++)
            {
                printf("\n\nInsira o naipe:\n1)-copas\n2)-Paus\n3)-Espadas\n4)-Ouros\n");
                scanf("%d", &naipes);
                printf("\nInsira o numero:\n1)-AS\n2)-Dois\n3)-Tres\n4)-Quatro\n5)-Cinco\n6)-Seis\n7)-Sete\n8)-Oito\n9)-Nove\n10)-Dez\n11)-[J]\n12)-[Q]\n13)-[K]\n");
                scanf("%d", &numeros);
                printf("\nInsira a mesa:\n");
                scanf("%d", &valor);

                busca_por_chave(jogador->mao, naipes, numeros, &pos); //Busca as cartas e retorna posicao!

                remover_mesa(jogador->mao, &Car[i], pos); //Remove da mao e insere no fim!
                Car[i].coringa = 0;
            }
            mesa_ordenada(Car, tam);
        }
    } while (op > 3 && op < 0);

    if (Car[2].numeros == 3 && Car[1].numeros == 2 && Car[2].naipe == Car[1].naipe && Car[0].numeros == 2 && Car[0].naipe != Car[1].naipe)
    {
        Car[0].coringa = 1;
        inserir_mesa_fim(jogador->mesa[valor], Car[1]);
        inserir_mesa_fim(jogador->mesa[valor], Car[2]);
        do
        {
            printf("\nDigite (1) para inserir no fim:\n");
            printf("\nDigite (2) para inserir no inicio:\n");
            printf("\nDigite (0) para voltar ao menu:\n");
            scanf("%d", &op);

            if (op == 1)
            {
                for (i = 0; i < 3; i++)
                {
                    inserir_mesa_fim(jogador->mesa[valor], Car[1]);
                }
            }
            else if (op == 2)
            {
                for (i = 0; i < 3; i++)
                {
                    inserir_mesa_inicio(jogador->mesa[valor], Car[1]);
                }
            }

        } while (op > 3 && op < 0);
    }
    else if (Car[2].numeros == 3 && Car[0].numeros == 2 && Car[2].naipe == Car[0].naipe && Car[1].numeros == 2 && Car[1].naipe != Car[0].naipe)
    {
        Car[1].coringa = 1;
        inserir_mesa_fim(jogador->mesa[valor], Car[0]);
        inserir_mesa_fim(jogador->mesa[valor], Car[2]);
        do
        {
            printf("\nDigite (1) para inserir no fim:\n");
            printf("\nDigite (2) para inserir no inicio:\n");
            printf("\nDigite (0) para voltar ao menu:\n");
            scanf("%d", &op);

            if (op == 1)
            {
                for (i = 0; i < 3; i++)
                {
                    inserir_mesa_fim(jogador->mesa[valor], Car[i]);
                }
            }
            else if (op == 2)
            {
                for (i = 0; i < 3; i++)
                {
                    inserir_mesa_inicio(jogador->mesa[valor], Car[i]);
                }
            }

        } while (op > 3 && op < 0);
    }
    else if (Car[0].numeros == Car[2].numeros - 2 && Car[0].naipe == Car[2].naipe && Car[1].numeros == 2)
    {
        Car[1].coringa = 1;

        inserir_mesa_fim(jogador->mesa[valor], Car[0]);
        inserir_mesa_fim(jogador->mesa[valor], Car[1]);
        inserir_mesa_fim(jogador->mesa[valor], Car[2]);
    }
    else if (Car[1].numeros == Car[2].numeros - 2 && Car[1].naipe == Car[2].naipe && Car[0].numeros == 2)
    {
        Car[0].coringa = 1;
        inserir_mesa_fim(jogador->mesa[valor], Car[1]);
        inserir_mesa_fim(jogador->mesa[valor], Car[0]);
        inserir_mesa_fim(jogador->mesa[valor], Car[2]);
    }
    else if (Car[0].numeros == 2 && Car[2].numeros - 2 != Car[0].numeros)
    {
        Car[0].coringa = 1;
        if (Car[2].numeros == Car[1].numeros + 1 && Car[2].naipe == Car[1].naipe)
        {
            inserir_mesa_fim(jogador->mesa[valor], Car[1]);
            inserir_mesa_fim(jogador->mesa[valor], Car[2]);
            do
            {
                printf("\nDigite (1) para inserir no fim:\n");
                printf("\nDigite (2) para inserir no inicio:\n");
                printf("\nDigite (0) para voltar ao menu:\n");
                scanf("%d", &op);

                if (op == 1)
                {
                    for (i = 0; i < 3; i++)
                    {
                        inserir_mesa_fim(jogador->mesa[valor], Car[i]);
                    }
                }
                else if (op == 2)
                {
                    for (i = 0; i < 3; i++)
                    {
                        inserir_mesa_inicio(jogador->mesa[valor], Car[i]);
                    }
                }

            } while (op > 3 && op < 0);
        }
        else
        {
            printf("Digite o negocio Certo ae ta bao!");
        }
    }
    else if (Car[1].numeros == 2 && Car[2].numeros - 2 != Car[1].numeros)
    {
        Car[1].coringa = 1;
        if (Car[2].numeros == Car[0].numeros + 1 && Car[2].naipe == Car[0].naipe)
        {
            inserir_mesa_fim(jogador->mesa[0], Car[0]);
            inserir_mesa_fim(jogador->mesa[0], Car[2]);
            do
            {
                printf("\nDigite (1) para inserir no fim:\n");
                printf("\nDigite (2) para inserir no inicio:\n");
                printf("\nDigite (0) para voltar ao menu:\n");
                scanf("%d", &op);

                if (op == 1)
                {
                    for (i = 0; i < 3; i++)
                    {
                        inserir_mesa_fim(jogador->mesa[valor], Car[i]);
                    }
                }
                else if (op == 2)
                {
                    for (i = 0; i < 3; i++)
                    {
                        inserir_mesa_inicio(jogador->mesa[valor], Car[i]);
                    }
                }

            } while (op > 3 && op < 0);
        }
        else
        {
            printf("Digite o negocio Certo ae ta bao!");
        }
    }
    else if (Car[0].naipe == Car[1].naipe && Car[1].naipe == Car[2].naipe)
    {
        do
        {
            printf("\nDigite (1) para inserir no fim:\n");
            printf("\nDigite (2) para inserir no inicio:\n");
            printf("\nDigite (0) para voltar ao menu:\n");
            scanf("%d", &op);

            if (op == 1)
            {
                for (i = 0; i < 3; i++)
                {
                    inserir_mesa_fim(jogador->mesa[valor], Car[i]);
                }
            }
            else if (op == 2)
            {
                for (i = 0; i < 3; i++)
                {
                    inserir_mesa_inicio(jogador->mesa[valor], Car[i]);
                }
            }

        } while (op > 3 && op < 0);
    }
    imprimi_lista(jogador->mesa[valor]);
}

void mesa_ordenada(cartas Car[], int tam) //Ordena as cartas utilizando metodo de ordenacao por insercao!
{

    int i, j;
    cartas *aux = (cartas *)malloc(sizeof(cartas));

    for (i = 0; i < tam; i++)
    {
        aux->naipe = Car[i].naipe;
        aux->numeros = Car[i].numeros;
        j = i - 1;

        while (aux->numeros < Car[j].numeros && j >= 0)
        {
            Car[j + 1] = Car[j];
            j--;
        }
        Car[j + 1].naipe = aux->naipe;
        Car[j + 1].numeros = aux->numeros;
    }
}

int Conta_ponto_lista(lista *l) //Conta as cartas e retorna se e canastra limpa,suja,real,meia real!
{

    int coringas = 0;
    elemento *novo = (elemento *)malloc(sizeof(elemento));

    novo = l->inicio;
    printf("\n");

    while (novo->prox != NULL)
    {

        if (novo->carta.coringa > 0)
        {
            coringas++;
        }

        novo = novo->prox;
    }

    if (novo->carta.coringa > 0)
    {
        coringas++;
    }

    if (l->tamanho == 14 && coringas == 0)
    {
        return 3; //REAL!!!
    }
    else if (l->tamanho == 13 && coringas == 0)
    {
        return 2; //MEIA REAL!!!
    }
    else if (coringas == 0 && l->tamanho >= 7)
    {
        return 1; //LIMPA!!!
    }
    else if (l->tamanho >= 7)
    {
        return 0; //SUJA!!!
    }
    else
    {
        return 4; //NAO CANASTRA!!!
    }
}

int Pontos_Jogador_Mao(lista *l) //Conta os pontos das cartas da mao!
{

    int pontos = 0;

    elemento *novo = (elemento *)malloc(sizeof(elemento));

    novo = l->inicio;
    printf("\n");

    while (novo->prox != NULL)
    {

        if (novo->carta.numeros == 1)
        {
            pontos -= 15;
        }
        else if (novo->carta.numeros == 2)
        {
            pontos -= 10;
        }
        else if (novo->carta.numeros > 2 && novo->carta.numeros < 8)
        {
            pontos -= 5;
        }
        else
        {
            pontos -= 10;
        }

        novo = novo->prox;
    }

    if (novo->carta.numeros == 1)
    {
        pontos -= 15;
    }
    else if (novo->carta.numeros == 2)
    {
        pontos -= 10;
    }
    else if (novo->carta.numeros > 2 && novo->carta.numeros < 8)
    {
        pontos -= 5;
    }
    else
    {
        pontos -= 10;
    }

    return pontos;
}

int Pontos_Jogador(registro *jogador) //Adiciona os pontos para cada jogador!
{
    int i, grupo = 0, pontos = 0, start = 0;

    for (i = 0; i < 20; i++)
    {
        printf("oi");
        grupo = Conta_ponto_lista(jogador->mesa[i]);

        if (grupo == 0)
        {
            printf("ESTE GRUPO É UMA CANASTRA SUJA");
            start += 100;
        }
        else if (grupo == 1)
        {
            printf("ESTE GRUPO É UMA CANASTRA LIMPA");
            start += 200;
        }
        else if (grupo == 2)
        {
            printf("ESTE GRUPO  É UMA CANASTRA MEIA REAL");
            start += 500;
        }
        else if (grupo == 3)
        {
            printf("ESTE GRUPO NAO É UMA CANASTRA REAL");
            start += 1000;
        }
        else
        {
            printf("ESTE GRUPO DE CARTAS NAO É UMA CANASTRA");
            start += 0;
        }
    }

    if (start > 0)
    {

        pontos += start;

        pontos = Pontos_Jogador_Mao(jogador->mao);

        if (jogador->bateu == 1)
        {
            pontos += 100;
        }

        if (jogador->morto)
        {
            pontos -= 100;
        }

        return pontos;
    }
    else
    {

        return -1;
    }
}
