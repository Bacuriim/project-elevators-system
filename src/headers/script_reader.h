#ifndef SCRIPT_READER_C
#define SCRIPT_READER_C

// STRUCTS
typedef struct simple_linked_instruction_list {
    char *instruction;
    struct simple_linked_instruction_list *next;
} instruction_list;

// FUNCTIONS
void add_instruction(instruction_list **instructions, char *value);

void split_and_add_instructions(instruction_list **instructions, char *line);

instruction_list *reader(char archive[]);

#endif