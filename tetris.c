#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5   // tamanho fixo da fila
#define MAX_PILHA 5  // capacidade máxima da pilha

// Estrutura que representa uma peça
typedef struct {
    char nome;  // tipo da peça ('I', 'O', 'T', 'L')
    int id;     // identificador único
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[MAX_FILA];
    int frente, tras, qtd;
} Fila;

// Estrutura da pilha
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

// ---------- Funções auxiliares ----------

// Inicializa a fila
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->qtd = 0;
}

// Inicializa a pilha
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Gera uma peça aleatória
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// Enfileirar (enqueue)
int enqueue(Fila *f, Peca p) {
    if (f->qtd == MAX_FILA) return 0; // fila cheia
    f->tras = (f->tras + 1) % MAX_FILA;
    f->itens[f->tras] = p;
    f->qtd++;
    return 1;
}

// Desenfileirar (dequeue)
int dequeue(Fila *f, Peca *removida) {
    if (f->qtd == 0) return 0; // fila vazia
    *removida = f->itens[f->frente];
    f->frente = (f->frente + 1) % MAX_FILA;
    f->qtd--;
    return 1;
}

// Empilhar (push)
int push(Pilha *p, Peca x) {
    if (p->topo == MAX_PILHA - 1) return 0; // pilha cheia
    p->itens[++p->topo] = x;
    return 1;
}

// Desempilhar (pop)
int pop(Pilha *p, Peca *removida) {
    if (p->topo == -1) return 0; // pilha vazia
    *removida = p->itens[p->topo--];
    return 1;
}

// Exibir fila
void exibirFila(Fila *f) {
    printf("Fila de pecas:\t");
    if (f->qtd == 0) {
        printf("[vazia]");
    } else {
        for (int i = 0; i < f->qtd; i++) {
            int idx = (f->frente + i) % MAX_FILA;
            printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
        }
    }
    printf("\n");
}

// Exibir pilha
void exibirPilha(Pilha *p) {
    printf("Pilha de reserva (Topo -> Base):\t");
    if (p->topo == -1) {
        printf("[vazia]");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
    }
    printf("\n");
}

// ---------- Programa principal ----------
int main() {
    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    srand(time(NULL));

    int opcao, idCounter = 0;
    Peca removida;

    // Inicializa a fila com 5 peças
    for (int i = 0; i < MAX_FILA; i++) {
        enqueue(&fila, gerarPeca(idCounter++));
    }

    do {
        printf("\n=== Estado atual ===\n");
        exibirFila(&fila);
        exibirPilha(&pilha);

        printf("\nOpcoes de acao:\n");
        printf("1 - Jogar peca\n");
        printf("2 - Reservar peca\n");
        printf("3 - Usar peca reservada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: // Jogar peça
                if (dequeue(&fila, &removida)) {
                    printf("Peça jogada: [%c %d]\n", removida.nome, removida.id);
                    enqueue(&fila, gerarPeca(idCounter++)); // mantém fila cheia
                } else {
                    printf("Fila vazia!\n");
                }
                break;

            case 2: // Reservar peça
                if (dequeue(&fila, &removida)) {
                    if (push(&pilha, removida)) {
                        printf("Peca reservada: [%c %d]\n", removida.nome, removida.id);
                    } else {
                        printf("Pilha cheia! Nao foi possivel reservar.\n");
                    }
                    enqueue(&fila, gerarPeca(idCounter++)); // mantém fila cheia
                } else {
                    printf("Fila vazia!\n");
                }
                break;

            case 3: // Usar peça reservada
                if (pop(&pilha, &removida)) {
                    printf("Peca usada da reserva: [%c %d]\n", removida.nome, removida.id);
                    enqueue(&fila, gerarPeca(idCounter++)); // mantém fila cheia
                } else {
                    printf("Pilha vazia! Nenhuma peca reservada.\n");
                }
                break;

            case 0:
                printf("Encerrando o jogo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}
