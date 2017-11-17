#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include <string.h>

typedef struct Pessoa cel;

struct Pessoa{
cel *prox;
cel *anterior;
int idade;
char nome[30];
float altura;
float peso;
};

struct No{
int chave;
int tamanho;
cel *head;
struct No *direita;
struct No *esquerda;
struct No *pai;
};

struct Arvore{
int tamanho;
struct No *raiz;
};


void AdcNo(struct Arvore *arvore, int chave);
struct No * buscaNo(struct No *raiz, int key);
struct No * buscaMaiorEsquerda(struct No *raiz);
struct No * buscaMenorDireita(struct No *raiz);
void imprimeArvore(struct No *raiz);
void RmvNo(struct Arvore *arvore, struct No *para_excluir);
void AdcPessoa(struct Arvore *arvore);
void RmvPessoa(struct Arvore *arvore, int idade, char nome[30]);
int stcmp(char nome[], char nome2[]);

int main()
{
    setlocale(LC_ALL,"portuguese");
    struct Arvore *arvore;
    arvore = (struct Arvore*)calloc(1,sizeof(struct Arvore));
    char pessoa[30];




    AdcNo(arvore, 10);

    imprimeArvore(arvore->raiz);

    fflush(stdin);
    printf("\npessoa: ");
    gets(pessoa);
    RmvPessoa(arvore, 10, pessoa);
    //AdcPessoa(arvore);
    //AdcPessoa(arvore);
    //RmvNo(arvore, buscaNo(arvore->raiz,10 ));
    printf("\n");
    imprimeArvore(arvore->raiz);
    //AdcPessoa(arvore);


    return 0;
}



void AdcNo(struct Arvore *arvore, int chave)
{
   int acoplou=0;   //variavel responsável por parar o while quando o nó for alocado

    struct No *aux;
    struct No *novo_no;
    novo_no = (struct No*)calloc(1, sizeof(struct No)); //alocação de memória pra um novo nó
    novo_no->chave = chave;     //o nó recebe sua identificação e tamanho 0
    novo_no->tamanho = 0;

    if(arvore->raiz == NULL)        //se a raiz da arvore ainda for nula o nó se torna a raiz
     {
        arvore->raiz = novo_no;
        arvore->tamanho++;
     }
    else{

        aux = arvore->raiz;     //aux recebe a raiz da arvore e será incrementado aos próximos endereços até que chegue a folha
        while(acoplou==0)
        {
            if(novo_no->chave > aux->chave)  //lógica da arvore, indice maior vai para a direita
            {
                if(aux->direita == NULL)        //se a direita for nula, o novo nó se torna filho da direita, senão...
                {
                    aux->direita = novo_no;
                    novo_no->pai = aux;
                    acoplou = 1;
                }else
                        aux = aux->direita;      //aux se torna o elemento a sua direita e retorna ao inicio do while

            } else if(novo_no->chave < aux->chave)  //mesma lógica aplicada acima, porém para o lado esquerdo
                {
                    if(aux->esquerda == NULL)
                    {
                        aux->esquerda = novo_no;
                        novo_no->pai = aux;
                        acoplou = 1;
                    }else
                        aux = aux->esquerda;
                }
        }
            arvore->tamanho++;
    }
}

struct No * buscaNo(struct No *raiz, int key) //busca um có para descobrir se existe, ser removido ou acessar sua lista
{
    if(raiz == NULL)            //caso chegue ao fim da arvore e não encontre retorna null;
        return NULL;
    if(raiz->chave == key)      //caso encontra retorna o endereço da variável
        return raiz;
    else{
        if(key > raiz->chave)           //se o número buscado for maior que o atual, direciona a busca pra direita, senão...
        buscaNo(raiz->direita, key);
        else
        buscaNo(raiz->esquerda, key);   //direciona a busca para a esquerda
    }
}


void RmvNo(struct Arvore *arvore, struct No *para_excluir)
{
    struct No *pai;
    struct No *substituto;
    pai = para_excluir->pai;

    if(para_excluir->direita == NULL && para_excluir->esquerda == NULL) //verifica se o nó a ser excluído é folha
    {
            if(pai->chave < para_excluir->chave)        //caso o nó seja folha, remove e retorna
                pai->direita = NULL;
            else
                pai->esquerda = NULL;
        free(para_excluir);
        arvore->tamanho--;
        return;
    }

    if(para_excluir->direita != NULL)
      {
       substituto = buscaMenorDireita(para_excluir);

        if(substituto->direita != NULL)
        {
           substituto->pai->esquerda = substituto->direita;
            substituto->direita->pai = substituto->pai;
        }else
            substituto->pai->esquerda = NULL;
      }
    else{
            substituto = buscaMaiorEsquerda(para_excluir);

            if(substituto->esquerda != NULL)
            {
               substituto->pai->direita = substituto->esquerda;
                substituto->esquerda->pai = substituto->pai;
            }else
                substituto->pai->direita = NULL;
    }

     if(para_excluir == arvore->raiz)
    {
        substituto->pai = NULL;
        arvore->raiz = substituto;
    }else{

     substituto->pai = para_excluir->pai;

     if(para_excluir->pai->direita == para_excluir)
        para_excluir->pai->direita = substituto;
     else
        para_excluir->pai->esquerda = substituto;

    }

     if(para_excluir->esquerda != NULL && para_excluir->esquerda != substituto)
     {
      substituto->esquerda = para_excluir->esquerda;
        para_excluir->esquerda->pai = substituto;
     }

     if(para_excluir->direita != NULL && para_excluir->direita != substituto)
    {
        para_excluir->direita->pai = substituto;
        substituto->direita = para_excluir->direita;
    }
        arvore->tamanho--;
       free(para_excluir);
}

struct No * buscaMaiorEsquerda(struct No *raiz)
{
  if(raiz->esquerda->direita == NULL)
        return raiz->esquerda;

  raiz = raiz->esquerda;
  while(1==1)
  {
   if(raiz->direita == NULL)
        return raiz;
    raiz = raiz->direita;
  }
}

struct No * buscaMenorDireita(struct No *raiz)
{
    if(raiz->direita->esquerda == NULL)
        return raiz->direita;

  raiz = raiz->direita;
  while(1==1)
  {
   if(raiz->esquerda == NULL)
        return raiz;
    raiz = raiz->esquerda;
  }
}

void imprimeArvore(struct No *raiz)
{
    struct Pessoa *aux;
    if(raiz == NULL)
        return;
    imprimeArvore(raiz->esquerda);
    printf("\n Nó:%d ", raiz->chave);
    aux = raiz->head;
    while(aux != NULL)
    {
        printf("\nIdade: %d  Nome: %s  Altura: %f   Peso: %f", aux->idade, aux->nome, aux->altura, aux->peso);
        aux = aux->prox;
    }
    imprimeArvore(raiz->direita);
}

void AdcPessoa(struct Arvore *arvore)
{
    struct No *lista;
    struct Pessoa *aux;
    struct Pessoa *ferramenta;
    aux = (struct Pessoa*)calloc(1,sizeof(struct Pessoa));
    int acoplou = 0;

    printf("\nIdade: ");
    scanf("%d", &aux->idade);
    fflush(stdin);
    printf("\nNome: ");
    gets(aux->nome);
    printf("\nAltura: ");
    scanf("%f", &aux->altura);
    printf("\nPeso: ");
    scanf("%f", &aux->peso);

    lista = buscaNo(arvore->raiz, aux->idade);

    if(lista == NULL)
    {
       AdcNo(arvore, aux->idade);
       lista = buscaNo(arvore->raiz, aux->idade);
    }
    if(lista->head == NULL)
        lista->head = aux;
    else{
        ferramenta = lista->head;
        while(acoplou==0)
        {
            if(ferramenta->prox == NULL)
             {
                ferramenta->prox = aux;
                aux->anterior = ferramenta;
                acoplou=1;
                lista->tamanho+=1;
             }else
                ferramenta = ferramenta->prox;
        }

    }
}

void RmvPessoa(struct Arvore *arvore, int idade, char nome[30])
{
    struct No *galho;
    struct Pessoa *aux;

    galho = buscaNo(arvore->raiz, idade);
    if(galho == NULL)
    {
        printf("\nO registro não existe.");
        return;
    }
    aux = galho->head;

    while(aux != NULL)
    {
        if(stcmp(aux->nome, nome)==0)
        {
            if(aux == galho->head)
                galho->head = NULL;
            else
                aux->anterior->prox = aux->prox;

            free(aux);
            return;
        }else
            aux = aux->prox;

    }
    printf("\nRegistro não encontrado.");
}

int stcmp(char nome[], char nome2[])
{
    int i=0;
    while(nome[i] != '\0')
    {
        toupper(nome[i]);
        i++;
    }
    i=0;
    while(nome2[i] != '\0')
    {
        toupper(nome2[i]);
        i++;
    }
    return (stricmp(nome, nome2)== 0)? 0: 1;
}
