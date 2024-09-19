/*
Developers: Conrado Einstein Malessa de Oliveira / Hiel Saraiva Freitas de Queiroga
This code simulates an Elevator
 */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "../headers/script_reader.h"
#include "../headers/elevator_system.h"

#ifdef _WIN32
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

// GLOBAL VARIABLES
int program_time = 0;
int maximum_floor;

//MAIN
int main(void) {
    char script_name[31];
    printf("Enter the name of the file that will be read as a script, limit 30 characters(put the file extension): ");
    scanf("%30s", script_name);
    printf("\n");

    char archive[80] = ".." PATH_SEPARATOR "src" PATH_SEPARATOR "scripts" PATH_SEPARATOR;
    strcat(archive, script_name);

    // manipulating script.txt and creating main lists
    passenger_list *passengers = NULL;
    elevator_list *elevators = NULL;
    instruction_list *instruction = (instruction_list *) reader(archive);

    if (instruction == NULL) {
        printf("The entered file was not found!\n");
        return 1; // script name error
    }

    while (instruction != NULL) {
        int state = interpreter(instruction, &passengers, &elevators);
        if (state == 1)
            return 1; // script error
        instruction = instruction->next;
    }

    // main application
    if (elevators->next == NULL) {
        move_elevator(&elevators, &passengers);
    } else {
        move_elevators(&elevators, &passengers);
    }

    // closing application
    free_all(&passengers, &elevators);

    return 0;
}
