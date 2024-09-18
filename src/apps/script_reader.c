#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct simple_linked_instruction_list {
    char *instruction;
    struct simple_linked_instruction_list *next;
} instruction_list;

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
        printf("Error opening file!\n");
        return NULL;
    }

    // read each line of the file until the end
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // remove newline character if present
        buffer[strcspn(buffer, "\n")] = '\0';
        split_and_add_instructions(&instructions, buffer);
    }

    // close the file
    fclose(file);

    // display stored instructions
    instruction_list *current = instructions;
    while (current != NULL) {
        //        printf("Instrucao: %s\n", current->instruction);
        current = current->next;
    }

    // free the allocated memory
    return instructions;
}
