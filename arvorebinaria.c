#include <stdio.h>
#include <stdlib.h>
#include "arvorebinaria.h"

struct no {
    int chave; // chave do nó da árvore, valor inteiro
    struct no *Fesq; // ponteiro para o filho da esquerda do nó
    struct no *Fdir; // ponteiro para o filho da direita do nó
    struct no *pai; // ponteiro para o pai do nó
};

struct arvore {
    struct no *sentinela; // ponteiro para a raiz da árvore, guarda o endereco em memoria da arvore. é um nó arvore também, o valor da sua chave deve ser -1000
    int numElementos; // número de elementos na árvore, guarda a quantidade de elementos armazenados na arvore
};

arvore *criaArvore() {
    // aloca espaço na memória para a estrutura árvore
    arvore *arv = (arvore *) malloc(sizeof(arvore));
    // aloca espaço na memória para o nó sentinela
    arv->sentinela = (no *) malloc(sizeof(no));
    arv->sentinela->chave = -1000;
    arv->sentinela->Fesq = NULL;
    arv->sentinela->Fdir = NULL;
    arv->sentinela->pai = NULL;
    arv->numElementos = 0;
    return arv;
}

int insereNo(arvore *arv, int valor) {
    // aloca espaço na memória para o nó a ser inserido
    no *novo = (no *) malloc(sizeof(no));
    if (novo == NULL)
        return 0;
    novo->chave = valor;
    novo->Fesq = NULL;
    novo->Fdir = NULL;
    novo->pai = NULL;
    // se a árvore estiver vazia, o nó inserido será a raiz
    if (arv->numElementos == 0) {
        arv->sentinela->Fesq = novo;
        novo->pai = arv->sentinela;
        arv->numElementos++;
        return 1;
    }
    // se a árvore não estiver vazia, procura a posição correta para inserir o nó
    no *aux = arv->sentinela->Fesq;
    no *auxPai = arv->sentinela;
    while (aux != NULL) {
        auxPai = aux;
        if (valor < aux->chave)
            aux = aux->Fesq;
        else
            aux = aux->Fdir;
    }
    // insere o nó na posição correta
    if (valor < auxPai->chave) {
        auxPai->Fesq = novo;
        novo->pai = auxPai;
        arv->numElementos++;
        return 1;
    } else {
        auxPai->Fdir = novo;
        novo->pai = auxPai;
        arv->numElementos++;
        return 1;
    }
}

int removeNo(arvore *arv, int valor) {
    // se a árvore estiver vazia, não é possível remover
    if (arv->numElementos == 0)
        return 0;
    // procura o nó a ser removido
    no *aux = arv->sentinela->Fesq;
    while (aux != NULL && aux->chave != valor) {
        if (valor < aux->chave)
            aux = aux->Fesq;
        else
            aux = aux->Fdir;
    }
    // se o nó não foi encontrado, não é possível remover
    if (aux == NULL)
        return -1;
    // se o nó a ser removido é uma folha
    if (aux->Fesq == NULL && aux->Fdir == NULL) {
        if (aux->pai->Fesq == aux)
            aux->pai->Fesq = NULL;
        else
            aux->pai->Fdir = NULL;
        free(aux);
        arv->numElementos--;
        return 1;
    }
    // se o nó a ser removido possui um filho
    if (aux->Fesq == NULL || aux->Fdir == NULL) {
        no *filho;
        if (aux->Fesq != NULL)
            filho = aux->Fesq;
        else
            filho = aux->Fdir;
        if (aux->pai->Fesq == aux)
            aux->pai->Fesq = filho;
        else
            aux->pai->Fdir = filho;
        filho->pai = aux->pai;
        free(aux);
        arv->numElementos--;
        return 1;
    }
    // se o nó a ser removido possui dois filhos
    if (aux->Fesq != NULL && aux->Fdir != NULL) {
        no *sucessor = aux->Fdir;
        while (sucessor->Fesq != NULL)
            sucessor = sucessor->Fesq;
        aux->chave = sucessor->chave;
        // Atualiza os ponteiros do pai e do filho do sucessor
        if (sucessor->pai->Fesq == sucessor)
            sucessor->pai->Fesq = sucessor->Fdir;
        else
            sucessor->pai->Fdir = sucessor->Fdir;
        if (sucessor->Fdir != NULL)
            sucessor->Fdir->pai = sucessor->pai;
        free(sucessor);
        arv->numElementos--;
        return 1;
    }
    return -1;
}

void imprimeOrdem(no *raiz) {
    if (raiz != NULL) {
        imprimeOrdem(raiz->Fesq);
        printf("%d - %d\n", raiz->chave, raiz->pai->chave);
        imprimeOrdem(raiz->Fdir);
    }
}

void imprimePreOrdem(no *raiz) {
    if (raiz != NULL) {
        printf("%d - %d\n", raiz->chave, raiz->pai->chave);
        imprimePreOrdem(raiz->Fesq);
        imprimePreOrdem(raiz->Fdir);
    }
}

no *getRaiz(arvore *arv) {
    return arv->sentinela->Fesq;
}

int getNumElementos(arvore *arv) {
    return arv->numElementos;
}

void processaCarga(arvore *arv, char *nomeArquivo) {
    FILE *arq = fopen(nomeArquivo, "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return;
    }
    int valor;
    while (fscanf(arq, "%d", &valor) != EOF)
        insereNo(arv, valor);
    fclose(arq);
}