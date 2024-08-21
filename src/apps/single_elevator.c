#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CAPACITY 10

typedef struct persons {
    char name[31];
    char code[15];
    int initialFloor;
    int finalFloor;
    int direction; // n >= 0 (up = 1) and n < 0 (down = -1)
} person;

typedef struct elevators {
    person person;
    int capacity;
    struct elevators *next;
    struct elevators *prev;
} elevator;

int get_direction(person p);
int passenger_in(elevator **e, person p);
int passenger_out(elevator **e);
void list_passengers(elevator *e);

int main() {

    elevator *passengers_list = NULL;

    person p1;
    person p2;

    strcpy(p1.name, "Conrado Einstein");
    strcpy(p1.code, "000.111.222-33");
    p1.initialFloor = 0;
    p1.finalFloor = 5;
    p1.direction = get_direction(p1);

    strcpy(p2.name, "Hiel Saraiva");
    strcpy(p2.code, "333.222.111-00");
    p2.initialFloor = 3;
    p2.finalFloor = 4;
    p2.direction = get_direction(p2);

    passenger_in(&passengers_list, p1);
    passenger_in(&passengers_list, p2);
    list_passengers(passengers_list);
    return 0;
}

int get_direction(person p) {
    return p.finalFloor - p.initialFloor >= 0 ? 1 : -1;
}

int passenger_in(elevator **e, person p) {
    elevator *newPerson = malloc(sizeof(elevator));
    if (*e == NULL) {
        newPerson->person = p;
        newPerson->prev = NULL;
        newPerson->next = NULL;
        *e = newPerson;
    } else if ((*e)->capacity < CAPACITY){
        elevator *aux = *e;
        while ((strcmp(p.code, aux->person.code) > 0) && aux->next != NULL)
        {
            aux = aux->next;
        }

        if (aux == *e && (strcmp(p.code, aux->person.code) < 0))
        { // esta no comeco
            newPerson->person = p;
            newPerson->prev = NULL;
            newPerson->next = *e;
            *e = newPerson;
            aux->prev = newPerson;
        }
        else if (aux->next == NULL && (strcmp(p.code, aux->person.code) > 0))
        { // esta no fim
            newPerson->person = p;
            newPerson->prev = aux;
            newPerson->next = NULL;
            aux->next = newPerson;
        }
        else
        { // esta no meio
            aux = aux->prev;
            newPerson->person = p;
            newPerson->prev = aux;
            newPerson->next = aux->next;
            aux->next = newPerson;
            (newPerson->next)->prev = newPerson;
        }
    } else {
        return 0;
    }
    (*e)->capacity += 1;
    return -1;
}

void list_passengers(elevator *e) {
    elevator *aux = e;
    while (aux != NULL) {
        printf("Name: %s\n", aux->person.name);
        printf("Code: %s\n", aux->person.code);
        printf("Initial floor: %d\n", aux->person.initialFloor);
        printf("Final floor: %d\n", aux->person.finalFloor);
        printf("Direction: %s\n", aux->person.direction == 1 ? "Up" : "Down");
        printf("///////////////////\n\n");
        aux = aux->next;
    }
}
