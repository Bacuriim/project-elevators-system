#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACITY 10

/*
Developers: Conrado Einstein Malessa de Oliveira / Hiel Saraiva Freitas de Queiroga

This code simulates an Elevator using 2 struct, elevators and calls as doubly linked list,
and passengers simulates real person calling the Elevator to upper ou down floors.
 */

typedef struct data {
    char name[31];
    char code[15];
    int initial_floor;
    int final_floor;
    int direction; // 1 = up, -1 = down
    int call_cooldown;
} data;

typedef struct passengers {
    data *data;
    struct passengers *next;
    struct passengers *prev;
} passenger;

typedef struct calls {
    int floor;
    int direction; // 1 = up, -1 = down
    struct calls *next;
    struct calls *prev;
} call;

typedef struct elevators {
    passenger *passengers;
    int capacity;
    call *calls;
    int actualFloor;
    int direction; // 1 = up, -1 = down
} elevator;

void up(elevator *e);
void down(elevator *e);
int get_direction(data *d);
int passenger_in(elevator *e, data *p_data);
int passenger_out(elevator *e);
void list_passengers(elevator *e);
void initialize_passenger(data *p_data, char name[], char code[], int initial_floor, int final_floor);
void move(elevator *e);
void add_call(call **c, int floor, int direction);
void process_calls(elevator *e);
void remove_call(call **c, int floor);
void list_floors(elevator *e);

int main() {
    elevator elevator = {NULL, 0, NULL, 0, 1};

    data p1;
    data p2;
    data p3;

    initialize_passenger(&p1, "Conrado Einstein", "000.111.222-33", 0, 5);
    initialize_passenger(&p2, "Hiel Saraiva", "333.222.111-00", 3, 4);
    initialize_passenger(&p3, "Ajalmar Rego", "123.123.123-00", 6, 8);

    passenger_in(&elevator, &p1);
    passenger_in(&elevator, &p2);
    passenger_in(&elevator, &p3);

    list_passengers(&elevator);

    move(&elevator);

    return 0;
}

void move(elevator *e) {
    while (e->calls != NULL) {
        process_calls(e);

        if (e->direction == 1) {
            up(e);
        } else {
            down(e);
        }

        list_floors(e);

        remove_call(&e->calls, e->actualFloor);
        passenger_out(e);
    }
}

void process_calls(elevator *e) {
    call *c = e->calls;
    if (c != NULL) {
        e->direction = (c->floor > e->actualFloor) ? 1 : -1;
    }
}

void down(elevator *e) {
    _sleep(2);
    e->actualFloor--;
    printf("Descendo... Andar atual: %d\n", e->actualFloor);
}

void up(elevator *e) {
    _sleep(2);
    e->actualFloor++;
    printf("Subindo... Andar atual: %d\n", e->actualFloor);
}

void initialize_passenger(data *p_data, char name[], char code[], int initial_floor, int final_floor) {
    strcpy(p_data->name, name);
    strcpy(p_data->code, code);
    p_data->initial_floor = initial_floor;
    p_data->final_floor = final_floor;
    p_data->direction = get_direction(p_data);
    p_data->call_cooldown = 2;
}

int get_direction(data *d) {
    return d->final_floor - d->initial_floor >= 0 ? 1 : -1;
}

void add_call(call **c, int floor, int direction) {
    call *new_call = malloc(sizeof(call));
    new_call->floor = floor;
    new_call->direction = direction;
    new_call->next = NULL;
    new_call->prev = NULL;

    if (*c == NULL) {
        *c = new_call;
    } else {
        call *aux = *c;
        while (aux->next != NULL) {
            aux = aux->next;
        }
        aux->next = new_call;
        new_call->prev = aux;
    }
}

void remove_call(call **c, int floor) {
    call *aux = *c;
    while (aux != NULL) {
        if (aux->floor == floor) {
            if (aux->prev != NULL) {
                aux->prev->next = aux->next;
            } else {
                *c = aux->next;
            }
            if (aux->next != NULL) {
                aux->next->prev = aux->prev;
            }
            free(aux);
            return;
        }
        aux = aux->next;
    }
}

int passenger_in(elevator *e, data *p_data) {
    passenger *new_passenger = malloc(sizeof(passenger));
    new_passenger->data = p_data;
    new_passenger->next = NULL;
    new_passenger->prev = NULL;

    if (e->passengers == NULL) {
        e->passengers = new_passenger;
        add_call(&e->calls, p_data->initial_floor, p_data->direction);
        add_call(&e->calls, p_data->final_floor, p_data->direction);
    } else if (e->capacity < CAPACITY) {
        passenger *aux = e->passengers;
        while (aux->next != NULL) {
            aux = aux->next;
        }
        aux->next = new_passenger;
        new_passenger->prev = aux;

        add_call(&e->calls, p_data->initial_floor, p_data->direction);
        add_call(&e->calls, p_data->final_floor, p_data->direction);
    } else {
        return 0;
    }

    e->capacity++;
    return 1;
}

int passenger_out(elevator *e) {
    if (e->passengers == NULL) {
        return 0;
    }

    passenger *to_remove = e->passengers;

    //Remove passengers when they reach their floor.
    while (to_remove != NULL) {
        if (to_remove->data->final_floor == e->actualFloor) {
            if (to_remove->prev != NULL) {
                to_remove->prev->next = to_remove->next;
            } else {
                e->passengers = to_remove->next;
            }

            if (to_remove->next != NULL) {
                to_remove->next->prev = to_remove->prev;
            }

            printf("Passageiro %s saiu no andar %d\n", to_remove->data->name, e->actualFloor);
            free(to_remove);
            e->capacity--;
            return 1;
        }
        to_remove = to_remove->next;
    }

    return 0;
}

void list_floors(elevator *e) {
    printf("------------------------------------------\n");
    for (int i = 10; i >= 0; i--) {
        if (i == e->actualFloor) {
            printf("|   Andar %02d: [X]   |\n", i);
        } else {
            printf("|   Andar %02d: [ ]   |\n", i);
        }
    }
    printf("------------------------------------------\n");
}

void list_passengers(elevator *e) {
    passenger *aux = e->passengers;
    while (aux != NULL) {
        printf("Name: %s\n", aux->data->name);
        printf("Code: %s\n", aux->data->code);
        printf("Initial floor: %d\n", aux->data->initial_floor);
        printf("Final floor: %d\n", aux->data->final_floor);
        printf("Direction: %s\n", aux->data->direction == 1 ? "Up" : "Down");
        printf("///////////////////\n\n");
        aux = aux->next;
    }
}