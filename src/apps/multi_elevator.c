#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <Windows.h>
#else

#include <unistd.h>
#include <strings.h>

#endif

// GLOBAL VARIABLES
int elevator_time = 0;

// STRUCTS
typedef struct doubly_linked_floor_list {
    int floor;
    struct doubly_linked_floor_list *next;
    struct doubly_linked_floor_list *prev;
} floor_list;

typedef struct passengers {
    char name[4]; // for example: P01, P06, P21. Including `\0`
    int initial_floor;
    int final_floor;
    int direction; // 1 = up, -1 = down
    int call_cooldown; // in seconds
} passengers;

typedef struct doubly_linked_passenger_list {
    passengers passenger;
    struct doubly_linked_passenger_list *next;
    struct doubly_linked_passenger_list *prev;
} passenger_list;

typedef struct elevators {
    char name[3]; // For example: E1, E4, E6. Including `\0`
    int actual_floor;
    int direction; // 1 = up, -1 = down
    struct doubly_linked_floor_list *route; // the route the elevator must follow
    struct doubly_linked_passenger_list *passengers_inside; // the passengers who are inside the elevator
    struct doubly_linked_passenger_list *passengers_to_enter;
    // the passengers that pressed the button and are waiting for enter the elevator
} elevators;

typedef struct doubly_linked_elevator_list {
    elevators elevator;
    struct doubly_linked_elevator_list *next;
    struct doubly_linked_elevator_list *prev;
} elevator_list;

//FUNCTIONS

// OK
void put_to_sleep(int seconds) {
    // this function stops the program flow for a while, both mac and windows
#ifdef _WIN32
    Sleep(seconds * 1000); // Sleep to Windows

#else
    sleep(seconds); // sleep to UNIX/Linux

#endif
}

// OK
int is_floor_in_list(floor_list *list, int floor) {
    // this function verify if the floor already is in the list of floors
    floor_list *aux = list;
    if (list != NULL) {
        while (aux != NULL) {
            if (aux->floor == floor) {
                return 1; // the floor already is in the list
            }
            aux = aux->next;
        }
    }
    return 0; // the floor is not in the list yet
}

// DOING
floor_list *search_floor(elevators *elevator) {
    // this function searches the actual_floor to remove from elevator route
    floor_list *aux = elevator->route;
    while(aux != NULL && aux->floor != elevator->actual_floor) {
        aux = aux->next;
    }
    return aux; // it's returns the pointer to the floor to remove
}

// OK
passenger_list *search_passenger_inside_list(elevators *elevator) {
    // this function searches a passenger that is inside the elevator and needs to get off at the route floor
    // ps.: the elevator route should NOT be NULL, otherwise a problem will happens
    passenger_list *aux = elevator->passengers_inside;
    while (aux != NULL && (aux->passenger).final_floor != (elevator->route)->floor) {
        aux = aux->next;
    }
    return aux;
    // if there's a passenger to gets off at this floor, it returns a pointer to this passenger, otherwise returns NULL
}

// OK
passenger_list *search_passenger_to_enter_list(elevators *elevator) {
    // this function searches a passenger that is outside the elevator and needs to get on
    // ps.: the elevator route should NOT be NULL, otherwise a problem will happens
    passenger_list *aux = elevator->passengers_to_enter;
    while (aux != NULL && (aux->passenger).initial_floor != elevator->actual_floor) { // (elevator->route)->floor
        aux = aux->next;
    }
    return aux;

    // if there's a passenger to gets on at the elevator, it returns a pointer to this passenger, otherwise returns NULL
}

// OK
passenger_list *search_passengers(passenger_list *passengers, int elevator_time) {
    // this function searches a passenger that is at the script
    passenger_list *aux = passengers;
    while (aux != NULL && elevator_time != (aux->passenger).call_cooldown) {
        aux = aux->next;
    }
    return aux;
    // if there's a passenger to be initialized, it returns a pointer to this passenger, otherwise returns NULL
}


// OK
int insert_floor_list(elevators *elevator, int floor) {
    // this function inserts the floor value in ascending order

    // verifying if the value to be added already is in the list (using && to short-circuit)
    if ((elevator->route) != NULL && is_floor_in_list(elevator->route, floor) == 1) {
        return 0; // the floor value was NOT added to the list because it already was in there
    }

    // adding the floor to the list
    if (elevator->route == NULL) {
        // if the list is empty
        floor_list *aux = malloc(sizeof(floor_list));
        aux->floor = floor;
        aux->prev = NULL;
        aux->next = NULL;
        elevator->route = aux;
    } else {
        // if the list already has some floor
        floor_list *aux1 = malloc(sizeof(floor_list));

        // moving pointer to the immediately next value that is bigger than floor value
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
    return 1; // the floor value was added to the list
}
// DOING
int remove_floor_list(elevators *elevator) { // coloque para remover de qualquer lugar
    // this function removes floor value from beginning
    floor_list *aux = search_floor(elevator);
    if (elevator->route == NULL) {
        return 0; // elevator route is empty
    }

    if (elevator->route->floor == aux->floor) {
        // ta no comeco
        (elevator->route) = (elevator->route)->next;
        if (elevator->route != NULL) {
            (elevator->route)->prev = NULL;
        }
        free(aux);
    } else if(aux->next == NULL) {
        // esta no fim
        (aux->prev)->next = NULL;
        free(aux);
    } else {
        // esta no meio
        (aux->prev)->next = aux->next;
        (aux->next)->prev = aux->prev;
        free(aux);
    }

    return 1; // the floor in the route has been removed
}

// OK
void insert_passenger_inside_list(elevators *elevator, passengers passenger) {
    // this function inserts a passenger in the list of passengers inside of the elevator, always at the list beginning (entry order)
    // ps.: passengers with the same names may be added to the list, be careful
    if (elevator->passengers_inside == NULL) {
        // if the list is empty
        passenger_list *aux = malloc(sizeof(passenger_list));
        aux->passenger = passenger;
        aux->prev = NULL;
        aux->next = NULL;
        elevator->passengers_inside = aux;
    } else {
        // if the list already has some passenger
        passenger_list *aux = malloc(sizeof(passenger_list));
        aux->passenger = passenger;
        aux->prev = NULL;
        aux->next = elevator->passengers_inside;
        (elevator->passengers_inside)->prev = aux;
        elevator->passengers_inside = aux;
    }
}

// OK
int remove_passenger_inside_list(elevators *elevator) {
    // This function removes a passenger from the passenger list inside the elevator when some passenger have to
    // get off at this route floor
    // ps.: if passengers with the same name are on the list, the first one to appear will be removed
    passenger_list *aux = search_passenger_inside_list(elevator);
    if (elevator->passengers_inside == NULL || aux == NULL) {
        return 0; // the list is empty or there's no one getting off at this floor
    }

    if (aux == elevator->passengers_inside) {
        // it's at the beginning
        elevator->passengers_inside = (elevator->passengers_inside)->next;
        if (elevator->passengers_inside != NULL) {
            (elevator->passengers_inside)->prev = NULL;
        }
        free(aux);
    } else if (aux->next == NULL) {
        // it's at the ending
        (aux->prev)->next = NULL;
        free(aux);
    } else {
        // it's at the middle
        (aux->prev)->next = aux->next;
        (aux->next)->prev = aux->prev;
        free(aux);
    }
    return 1; // the passenger who gets off on this floor has been removed
}

// OK
void insert_passenger_to_enter_list(elevators *elevator, passengers passenger) {
    // this function inserts a passenger in the list of passengers outside of the elevator, always at the list beginning (entry order)
    // ps.: passengers with the same names may be added to the list, be careful
    if (elevator->passengers_to_enter == NULL) {
        // if the list is empty
        passenger_list *aux = malloc(sizeof(passenger_list));
        aux->passenger = passenger;
        aux->prev = NULL;
        aux->next = NULL;
        elevator->passengers_to_enter = aux;
    } else {
        // if the list already has some passenger
        passenger_list *aux = malloc(sizeof(passenger_list));
        aux->passenger = passenger;
        aux->prev = NULL;
        aux->next = elevator->passengers_to_enter;
        (elevator->passengers_to_enter)->prev = aux;
        elevator->passengers_to_enter = aux;
    }
}

// OK
int remove_passenger_to_enter_list(elevators *elevator) {
    // This function removes a passenger from the passenger list outside the elevator when some passenger have to
    // get on at this route floor
    // ps.: if passengers with the same name are on the list, the first one to appear will be removed
    passenger_list *aux = search_passenger_to_enter_list(elevator);
    if (elevator->passengers_to_enter == NULL || aux == NULL) {
        return 0; // the list is empty or there's no one getting on at this floor
    }

    if (aux == elevator->passengers_to_enter) {
        // it's at the beginning
        elevator->passengers_to_enter = (elevator->passengers_to_enter)->next;
        if (elevator->passengers_to_enter != NULL) {
            (elevator->passengers_to_enter)->prev = NULL;
        }
        free(aux);
    } else if (aux->next == NULL) {
        // it's at the ending
        (aux->prev)->next = NULL;
        free(aux);
    } else {
        // it's at the middle
        (aux->prev)->next = aux->next;
        (aux->next)->prev = aux->prev;
        free(aux);
    }
    return 1; // the passenger who gets on on this floor has been removed
}

// OK
int remove_passenger_in_script(passenger_list **passengersP, passengers passenger) {
    // this function removes a passenger from passengers script list when elevator_time == call_cooldown
    passenger_list *aux = search_passengers(*passengersP, elevator_time);

    if (*passengersP == NULL || aux == NULL) {
        return 0; // n removeu
    }

    if (aux == *passengersP) {
        // it's at the beginning
        *passengersP = (*passengersP)->next;

        if (*passengersP != NULL) {
            (*passengersP)->prev = NULL;
        }
        free(aux);
    } else if (aux->next == NULL) {
        // it's at the end
        (aux->prev)->next = NULL;
        free(aux);
    } else {
        // it's at the middle
        (aux->prev)->next = aux->next;
        (aux->next)->prev = aux->prev;
        free(aux);
    }
    return 1; // removeu
}

// OK
void insert_passenger_in_to_enter_list(elevators *elevator, passenger_list **passengers, int elevator_time) {
    // this function inserts all passengers with same time of elevator_time to elevator system
    while (search_passengers(*passengers, elevator_time) != NULL) {
        insert_passenger_to_enter_list(elevator, (search_passengers(*passengers, elevator_time))->passenger);
        insert_floor_list(elevator, (search_passengers(*passengers, elevator_time))->passenger.initial_floor);
        remove_passenger_in_script(passengers, (search_passengers(*passengers, elevator_time))->passenger);
    }
}

// OK
void insert_passenger_list(passenger_list **list, passengers passenger) {
    // inserindo no final
    if (*list == NULL) {
        // Se nao houver nenhum no:
        passenger_list *aux = malloc(sizeof(passenger_list));
        aux->passenger = passenger;
        aux->prev = NULL;
        aux->next = NULL;
        *list = aux;
    } else {
        // Se ja houver algum no:
        passenger_list *aux1 = malloc(sizeof(passenger_list));
        aux1->passenger = passenger;
        aux1->next = NULL;

        // aux2 percorre a lista ate o ultimo dado:
        passenger_list *aux2 = *list;
        while (aux2->next != NULL) {
            aux2 = aux2->next;
        }
        aux2->next = aux1;
        aux1->prev = aux2;
    }
}

// OK
void insert_elevator_list(elevator_list **list, elevators elevator) {
    // inserindo no final
    if (*list == NULL) {
        // Se nao houver nenhum no:
        elevator_list *aux = malloc(sizeof(elevator_list));
        aux->elevator = elevator;
        aux->prev = NULL;
        aux->next = NULL;
        *list = aux;
    } else {
        // Se ja houver algum no:
        elevator_list *aux1 = malloc(sizeof(elevator_list));
        aux1->elevator = elevator;
        aux1->next = NULL;

        // aux2 percorre a lista ate o ultimo dado:
        elevator_list *aux2 = *list;
        while (aux2->next != NULL) {
            aux2 = aux2->next;
        }
        aux2->next = aux1;
        aux1->prev = aux2;
    }
}

// OK
int print_elevator_route(elevators *elevator) {
    // this function prints the elevator route
    if (elevator->route == NULL) {
        printf("elevator route: [  ]\n");
        return 0; // route is empty
    }
    floor_list *aux = elevator->route;
    printf("elevator route: [");
    while (aux != NULL) {
        if (aux->floor != 1) {
            printf(" %d", aux->floor);
        } else {
            printf(" T");
        }
        aux = aux->next;
        if (aux != NULL) {
            printf(" ->");
        }
    }
    printf(" ]\n");
    return 1; // the list has been printed
}

// OK
int print_elevator_passengers_inside(elevators *elevator) {
    // this function prints the names of passengers inside the elevator
    if (elevator->passengers_inside == NULL) {
        printf("passengers inside: [  ]\n");
        return 0; // the elevator is empty
    }
    passenger_list *aux = elevator->passengers_inside;
    printf("passengers inside: [");
    while (aux != NULL) {
        printf(" %s", (aux->passenger).name);
        aux = aux->next;
        if (aux != NULL) {
            printf(" ,");
        }
    }
    printf(" ]\n");
    return 1; // the list has been printed
}

// OK
int print_elevator_passengers_to_enter(elevators *elevator) {
    // this function prints the names of passengers to enter the elevator
    if (elevator->passengers_to_enter == NULL) {
        printf("passengers to enter: [  ]\n");
        return 0; // the elevator is empty
    }
    passenger_list *aux = elevator->passengers_to_enter;
    printf("passengers to enter: [");
    while (aux != NULL) {
        printf(" %s", (aux->passenger).name);
        aux = aux->next;
        if (aux != NULL) {
            printf(" ,");
        }
    }
    printf(" ]\n");
    return 1; // the list has been printed
}

// OK
void print_elevator_state(elevators *elevator) {
    // this function prints all elevator informations
    printf("CURRENT TIME: %d\n", elevator_time);
    printf("name: %s\n", elevator->name);
    if (elevator->actual_floor != 1) {
        printf("actual floor: %d\n", elevator->actual_floor);
    } else {
        printf("actual floor: T\n");
    }
    if (elevator->direction == 1) {
        printf("direction: S\n"); // S = "Subindo" = means "going up" in portuguese
    } else {
        printf("direction: D\n"); // D = "Descendo" = means "going down" in portuguese
    }
    print_elevator_route(elevator);
    print_elevator_passengers_inside(elevator);
    print_elevator_passengers_to_enter(elevator);
}

// TO DO
void print_all_elevators() {

}

// OK
void go_down(elevators *elevator) {
    // this function makes elevator go down one floor
    put_to_sleep(1); // the descent time
    --(elevator->actual_floor);
}

// OK
void go_up(elevators *elevator) {
    // this function makes elevator go up one floor
    put_to_sleep(1); // the rise time
    ++(elevator->actual_floor);
}

// OK
int get_new_elevator_direction(elevators *elevator) {
    // this function returns the new elevator direction
    return (elevator->route)->floor - elevator->actual_floor >= 0 ? 1 : -1; // will go up = 1; will go down = -1
}

// OK
int invert_elevator_direction(elevators *elevator) {
    if(elevator->route != NULL) {
        elevator->direction = get_new_elevator_direction(elevator);
        return 1; // the direction was reversed
    }
    return 0; // the direction was NOT reversed
}

// DOING
void move_elevator(elevator_list **elevator, passenger_list **passengers) {
    // this function makes an elevator work
    while((*elevator)->elevator.route != NULL || *passengers != NULL) {
        print_elevator_state(&((*elevator)->elevator));
        // verify when the elevator bring to the floor route
        if ((*elevator)->elevator.route->floor == (*elevator)->elevator.actual_floor) {
            // people are exiting the elevator
            if(search_passenger_inside_list(&((*elevator)->elevator)) != NULL) {
                if((*elevator)->elevator.actual_floor == 1) {
                    printf("elevator exits at T floor: ");
                } else {
                    printf("elevator exits at %d floor: ", (*elevator)->elevator.actual_floor);
                }
                while(search_passenger_inside_list(&((*elevator)->elevator)) != NULL) {
                    printf("%s ", search_passenger_inside_list(&((*elevator)->elevator))->passenger.name);
                    remove_passenger_inside_list(&((*elevator)->elevator));
                }
                printf("\n");
            }

            // people are entering the elevator
            if (search_passenger_to_enter_list(&((*elevator)->elevator)) != NULL) {
                if ((*elevator)->elevator.actual_floor == 1) {
                    printf("elevator entrances at T floor: ");
                } else {
                    printf("elevator entrances at %d floor: ", (*elevator)->elevator.actual_floor);
                }
                while(search_passenger_to_enter_list(&((*elevator)->elevator)) != NULL) {
                    int floor = (search_passenger_to_enter_list(&((*elevator)->elevator)))->passenger.final_floor;
                    char name[4];
                    strcpy(name, (search_passenger_to_enter_list(&((*elevator)->elevator)))->passenger.name);
                    insert_passenger_inside_list(&((*elevator)->elevator), (search_passenger_to_enter_list(&((*elevator)->elevator)))->passenger);
                    remove_passenger_to_enter_list(&((*elevator)->elevator));
                    insert_floor_list(&((*elevator)->elevator), floor);
                    printf("%s ", name);
                }
                printf("\n");
            }
            remove_floor_list(&((*elevator)->elevator)); // erro esta no remove, n esta removendo o 6, dando free no canto errado
        }
        printf("\n");

        // pesquisando na lista de passageiros do script
        while (search_passengers(*passengers, elevator_time) != NULL) {
            // adicionar no to_enter e remove de passengers list
            insert_passenger_in_to_enter_list(&((*elevator)->elevator), passengers, elevator_time);
        }
        invert_elevator_direction(&((*elevator)->elevator));

        // movendo o elevador
        if ((&((*elevator)->elevator))->direction == 1) {
            go_up(&((*elevator)->elevator));
        } else {
            go_down(&((*elevator)->elevator));
        }

        ++ elevator_time;
        while((*elevator)->elevator.route == NULL && *passengers != NULL) {
            print_elevator_state(&((*elevator)->elevator));
            printf("\n");
            while (search_passengers(*passengers, elevator_time) != NULL) {
                // adicionar no to_enter e remove de passengers list
                insert_passenger_in_to_enter_list(&((*elevator)->elevator), passengers, elevator_time);
            }
            invert_elevator_direction(&((*elevator)->elevator));
            ++elevator_time;
        }
    }
    printf("elapsed time: %d\n", elevator_time - 1);
}


// DOING
void move_elevators(elevator_list *elevatorsList, passenger_list *passengers) {

}

// TO DO
int free_elevator_list() {
    return 0;
}

// TO DO
int free_floor_list() {
    return 0;
}

// TO DO
int free_passenger_list() {
    return 0;
}

// TO DO
int free_all() {
    return 0;
}

int main(void) {
    // manipulating script.txt and creating main variables
    int const maximum_floor = 25;


    // creating elevator(s)
    elevators elevator1 = {"E1", 4, 1, NULL, NULL, NULL, NULL, NULL};
    elevators elevator2 = {"E2", 11, -1, NULL, NULL, NULL, NULL, NULL};
    elevators elevator3 = {"E3", 20, -1, NULL, NULL, NULL, NULL, NULL};
    elevator_list *elevators = NULL;
    insert_elevator_list(&elevators, elevator1);
//    insert_elevator_list(&elevators, elevator2);
//    insert_elevator_list(&elevators, elevator3);
    insert_floor_list(&(elevators->elevator), 6);
    insert_floor_list(&(elevators->elevator), 9);
    insert_floor_list(&(elevators->elevator), 8);



    // creating passenger(s)
    passengers passenger1 = {"P01", 4, 6, 1, 5};
    passengers passenger2 = {"P01", 6, 4, -1, 8};
    passengers passenger3 = {"P02", 5, 10, 1, 9};
    passengers passenger4 = {"P03", 3, 9, 1, 10};
    passengers passenger5 = {"P04", 6, 1, -1, 10};
    passengers passenger6 = {"P05", 8, 15, 1, 10};
    passengers passenger7 = {"P06", 9, 2, -1, 15};
    passengers passenger8 = {"P07", 2, 13, 1, 15};
    passengers passenger9 = {"P08", 8, 1, -1, 18};
    passengers passenger10 = {"P01", 16, 3, -1, 21};
    passengers passenger11 = {"P10", 1, 13, 1, 21};
    passengers passenger12 = {"P11", 1, 12, 1, 21};
    passengers passenger13 = {"P12", 1, 15, 1, 23};
    passengers passenger14 = {"P13", 2, 13, 1, 28};
    passenger_list *passengers = NULL;
    insert_passenger_list(&passengers, passenger1);
    insert_passenger_list(&passengers, passenger2);
    insert_passenger_list(&passengers, passenger3);
    insert_passenger_list(&passengers, passenger4);
    insert_passenger_list(&passengers, passenger5);
    insert_passenger_list(&passengers, passenger6);
    insert_passenger_list(&passengers, passenger7);
    insert_passenger_list(&passengers, passenger8);
    insert_passenger_list(&passengers, passenger9);
    insert_passenger_list(&passengers, passenger10);
    insert_passenger_list(&passengers, passenger11);
    insert_passenger_list(&passengers, passenger12);
    insert_passenger_list(&passengers, passenger13);
    insert_passenger_list(&passengers, passenger14);

    // testing some functions
    if(elevators->next == NULL) {
        move_elevator(&elevators, &passengers);
    } else {
//        move_elevators();
    }

    // closing application


    return 0;
}
