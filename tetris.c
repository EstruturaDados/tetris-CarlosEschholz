#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 5  // tamanho fixo da fila

// Estrutura que representa uma peça
typedef struct {
    char nome;  // tipo da peça ('I', 'O', 'T', 'L')
    int id;     // identificador único
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[MAX];
    int frente, tras, qtd;
} Fila;

// Função para inicializar a fila
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->qtd = 0;
}

// Função que gera automaticamente uma peça
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];  // sorteia um tipo
    nova.id = id;
    return nova;
}

// Inserir peça no final da fila (enqueue)
int enqueue(Fila *f, Peca p) {
    if (f->qtd == MAX) {
        printf("Fila cheia! Nao e possivel inserir nova peca.\n");
        return 0;
    }
    f->tras = (f->tras + 1) % MAX;
    f->itens[f->tras] = p;
    f->qtd++;
    return 1;
}

// Remover peça da frente da fila (dequeue)
int dequeue(Fila *f, Peca *removida) {
    if (f->qtd == 0) {
        printf("Fila vazia! Nenhuma peca para jogar.\n");
        return 0;
    }
    *removida = f->itens[f->frente];
    f->frente = (f->frente + 1) % MAX;
    f->qtd--;
    return 1;
}

// Exibir estado atual da fila
void exibirFila(Fila *f) {
    printf("\nFila de pecas:\n");
    if (f->qtd == 0) {
        printf("[vazia]\n");
        return;
    }
    int i, idx;
    for (i = 0; i < f->qtd; i++) {
        idx = (f->frente + i) % MAX;
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
    }
    printf("\n");
}

int main() {
    Fila fila;
    inicializarFila(&fila);
    srand(time(NULL));

    int opcao, idCounter = 0;
    Peca removida;

    // Inicializa a fila com 5 peças
    for (int i = 0; i < MAX; i++) {
        enqueue(&fila, gerarPeca(idCounter++));
    }

    do {
        exibirFila(&fila);
        printf("\nOpcoes de acao:\n");
        printf("1 - Jogar peca (dequeue)\n");
        printf("2 - Inserir nova peca (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                if (dequeue(&fila, &removida)) {
                    printf("Peca jogada: [%c %d]\n", removida.nome, removida.id);
                }
                break;
            case 2:
                if (enqueue(&fila, gerarPeca(idCounter++))) {
                    printf("Nova peca inserida!\n");
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
