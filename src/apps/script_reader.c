//
// Created by Conrado Einstein on 13/09/2024.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct simple_linked_instruction_list {
    char *instruction;
    struct simple_linked_instruction_list *next;
}instruction_list;

void add_instruction(instruction_list **instructions, char *value) {
    instruction_list *new_node = malloc(sizeof(instruction_list));
    new_node->instruction = strdup(value);
    new_node->next = *instructions;
    *instructions = new_node;
}

void split_and_add_instructions(instruction_list **instructions, char *line) {
    char *token = strtok(line, " ");
    while (token != NULL) {
        add_instruction(instructions, token);
        token = strtok(NULL, " ");
    }
}

instruction_list *reader(char archive[]) {
    FILE *file;
    char buffer[500];
    instruction_list *instructions = NULL;

    file = fopen(archive, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return NULL;
    }

    // Ler cada linha do arquivo até o fim
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Remover o caractere de nova linha, se presente
        buffer[strcspn(buffer, "\n")] = '\0';
        split_and_add_instructions(&instructions, buffer);
    }

    // Fechar o arquivo
    fclose(file);

    // Exibir as instruções armazenadas
    instruction_list *current = instructions;
    while (current != NULL) {
        printf("Instrucao: %s\n", current->instruction);
        current = current->next;
    }

    // Liberar a memória alocada
    return instructions;
}