#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5   // capacidade da fila
#define MAX_PILHA 3  // capacidade da pilha

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

// Trocar peça da frente da fila com o topo da pilha
void trocarTopo(Fila *f, Pilha *p) {
    if (f->qtd == 0 || p->topo == -1) {
        printf("Nao e possivel trocar: fila ou pilha vazia.\n");
        return;
    }
    int idxFrente = f->frente;
    Peca temp = f->itens[idxFrente];
    f->itens[idxFrente] = p->itens[p->topo];
    p->itens[p->topo] = temp;
    printf("Troca realizada entre frente da fila e topo da pilha.\n");
}

// Troca múltipla: 3 primeiros da fila <-> 3 da pilha
void trocaMultipla(Fila *f, Pilha *p) {
    if (f->qtd < 3 || p->topo < 2) {
        printf("Nao e possivel realizar troca multipla (faltam pecas).\n");
        return;
    }
    for (int i = 0; i < 3; i++) {
        int idxFila = (f->frente + i) % MAX_FILA;
        Peca temp = f->itens[idxFila];
        f->itens[idxFila] = p->itens[p->topo - i];
        p->itens[p->topo - i] = temp;
    }
    printf("Troca multipla realizada entre fila e pilha.\n");
}

int reservarPeca(Fila *f, Pilha *p, int *idCounter) {
    if (p->topo == MAX_PILHA - 1) {
        printf("Pilha cheia! Nao foi possivel reservar.\n");
        return 0;
    }
    Peca x;
    if (!dequeue(f, &x)) {
        printf("Fila vazia!\n");
        return 0;
    }
    if (push(p, x)) {
        printf("Peca reservada: [%c %d]\n", x.nome, x.id);
        enqueue(f, gerarPeca((*idCounter)++)); // mantém a fila cheia
        return 1;
    }
    return 0;
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

        printf("\nOpcoes disponiveis:\n");
        printf("1 - Jogar peca da frente da fila\n");
        printf("2 - Enviar peca da fila para a pilha de reserva\n");
        printf("3 - Usar peca da pilha de reserva\n");
        printf("4 - Trocar peca da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 pecas da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: // Jogar peça
                if (dequeue(&fila, &removida)) {
                    printf("Peca jogada: [%c %d]\n", removida.nome, removida.id);
                    enqueue(&fila, gerarPeca(idCounter++));
                } else {
                    printf("Fila vazia!\n");
                }
                break;

            case 2: // Reservar peça
                reservarPeca(&fila, &pilha, &idCounter);
                break;

            case 3: // Usar peça reservada
                if (pop(&pilha, &removida)) {
                    printf("Peca usada da reserva: [%c %d]\n", removida.nome, removida.id);
                } else {
                    printf("Pilha vazia!\n");
                }
                break;

            case 4: // Trocar frente da fila com topo da pilha
                trocarTopo(&fila, &pilha);
                break;

            case 5: // Troca múltipla
                trocaMultipla(&fila, &pilha);
                break;

            case 0:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}
