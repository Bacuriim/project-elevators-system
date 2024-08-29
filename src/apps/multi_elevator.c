#include <stdio.h>
#include <stdlib.h>

typedef struct passengers {
    char name[3]; // For example: P01, P06, P21
    int initial_floor;
    int final_floor;
    int direction; // 1 = up, -1 = down
    int call_cooldown;
} passengers;

//
typedef struct elevators {
    char name[2]; // For example: E1, E4, E6
    int actual_floor;
    int direction; // 1 = up, -1 = down
    struct doubly_linked_floor_list *route; // the route the elevator must follow
    struct doubly_linked_passenger_list *passengers_inside; // the passengers who are inside the elevator
    // struct doubly_linked_passenger_list *passengers_to_enter // the passengers that pressed the button and are waiting for enter the elevator
} elevators;

typedef struct doubly_linked_passenger_list {
    passengers passengers;
    struct doubly_linked_passenger_list *next;
    struct doubly_linked_passenger_list *prev;
} passenger_list;

typedef struct doubly_linked_floor_list {
    int floor;
    struct doubly_linked_floor_list *next;
    struct doubly_linked_floor_list *prev;
} floor_list;

// TO DO (fix: insert when we have 2 equal floors {maybe will be necessary make a new function to search})
// TO DO (feat: do something to make the elevator decide when it will be in ascending or descending order )
void insert_floor_list(elevators *elevator, int floor) {
    // this function inserts in ascending order
    if (elevator->route == NULL) {
        // if the list is empty
        floor_list *aux = malloc(sizeof(floor_list));
        aux->floor = floor;
        aux->prev = NULL;
        aux->next = NULL;
        elevator->route = aux;
    } else {
        // if the list already has something
        floor_list *aux1 = malloc(sizeof(floor_list));

        floor_list *aux2 = elevator->route;
        while (floor > aux2->floor && aux2->next != NULL) {
            aux2 = aux2->next;
        }

        if (aux2 == elevator->route && floor < aux2->floor) {
            // it's at the beginning
            aux1->floor = floor;
            aux1->prev = NULL;
            aux1->next = elevator->route;
            elevator->route = aux1;
            aux2->prev = aux1;
        } else if (aux2->next == NULL && floor > aux2->floor) {
            // it's at the ending
            aux1->floor = floor;
            aux1->prev = aux2;
            aux1->next = NULL;
            aux2->next = aux1;
        } else {
            // it's at the middle
            aux2 = aux2->prev;
            aux1->floor = floor;
            aux1->prev = aux2;
            aux1->next = aux2->next;
            aux2->next = aux1;
            (aux1->next)->prev = aux1;
        }
    }
}

int remove_floor_list(elevators *elevator) {
    // this function removes from beginning
    if (elevator->route == NULL) {
        return 0; // elevator route is empty
    }
    floor_list *aux = elevator->route;
    elevator->route = (elevator->route)->next;
    if (elevator->route != NULL) {
        (elevator->route)->prev = NULL;
    }
    free(aux);
    return 1;
}

// TO DO
void insert_passenger_list(elevators *elevator, passengers) {
}

// TO DO
int remove_passenger_list(elevators *elevator) {
    return 0;
}

int print_elevator_route(elevators elevator) {
    if (elevator.route == NULL) {
        return 0; // route is empty
    }
    printf("[");
    while (elevator.route != NULL) {
        if ((elevator.route)->floor != 1) {
            printf(" %d", (elevator.route)->floor);
        } else {
            printf(" T");
        }
        elevator.route = (elevator.route)->next;
        if (elevator.route != NULL) {
            printf(" ->");
        }
    }
    printf(" ]\n");
    return 1;
}

// TO DO
int print_elevator_passengers(elevators elevator) {
    return 0;
}

// TO DO (put printf to elevator_passengers)
void print_elevator(elevators elevator) {
    printf("name: %s\n", elevator.name);
    if (elevator.actual_floor != 1) {
        printf("actual floor: %d\n", elevator.actual_floor);
    } else {
        printf("actual floor: T\n");
    }
    if (elevator.direction == 1) {
        printf("direction: S\n"); // S = "Subindo" = means "going up" in portuguese
    } else {
        printf("direction: D\n"); // D = "Descendo" = means "going down" in portuguese
    }
    print_elevator_route(elevator);
    print_elevator_passengers(elevator);
}

int main(void) {
    // manipulating script.txt


    // creating elevator(s)
    elevators elevator1 = {"E1", 4, 1, NULL, NULL};


    // creating passenger(s)


    // testing some functions
    insert_floor_list(&elevator1, 1);
    insert_floor_list(&elevator1, 9);
    insert_floor_list(&elevator1, 8);
    insert_floor_list(&elevator1, 11);
    insert_floor_list(&elevator1, 3);
    insert_floor_list(&elevator1, 4);
    // insert_floor_list(&elevator1, 8);
    print_elevator_route(elevator1);

    remove_floor_list(&elevator1);
    print_elevator_route(elevator1);
    remove_floor_list(&elevator1);
    remove_floor_list(&elevator1);
    remove_floor_list(&elevator1);
    remove_floor_list(&elevator1);
    print_elevator_route(elevator1);
    remove_floor_list(&elevator1);
    print_elevator_route(elevator1);


    // closing application


    return 0;
}
