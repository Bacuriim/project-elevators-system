#include <stdio.h>
#define CAPACIDADE 10

typedef struct pessoas {
    char nome[31];
    int andarInicial;
    int andarFinal;
}pessoa;

typedef struct elevadores {
    pessoa *pessoa;
    int capacidade = CAPACIDADE;
    struct elevadores *prox;
    struct elevadores *ant;
}elevador;

int embarcar(elevador **elevador, pessoa pessoa);
int desembarcar(elevador **elevador);
void listar_passageiros(elevador **elevador);

int main(void) {
    printf("Hello, World!\n");
    return 0;
}
