#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <Windows.h>
#else

#include <unistd.h>

#endif

// GLOBAL VARIABLES
int elevator_time = 0;

// STRUCTS
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

typedef struct doubly_linked_floor_list {
    int floor;
    struct doubly_linked_floor_list *next;
    struct doubly_linked_floor_list *prev;
} floor_list;

typedef struct elevators {
    char name[3]; // For example: E1, E4, E6. Including `\0`
    int actual_floor;
    int direction; // 1 = up, -1 = down
    struct doubly_linked_floor_list **actual_route; // point to the actual route, that can be up_route or down_route
    struct doubly_linked_floor_list *up_route; // the route the elevator must follow if it's going up
    struct doubly_linked_floor_list *down_route; // the route the elevator must follow if it's going down
    struct doubly_linked_passenger_list *passengers_inside; // the passengers who are inside the elevator
    struct doubly_linked_passenger_list *passengers_to_enter;
    // the passengers that pressed the button and are waiting for enter the elevator
} elevators;

//FUNCTIONS
void put_to_sleep(int seconds) {
    // this function stops the program flow for a while, both mac and windows
#ifdef _WIN32
    Sleep(seconds * 1000); // Sleep to Windows

#else
    sleep(seconds); // sleep to UNIX/Linux

#endif
}

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

passenger_list *search_passenger_inside_list(elevators *elevator) {
    // this function searches a passenger that is inside the elevator and needs to get off at the actual_route floor
    // ps.: the elevator route should NOT be NULL, otherwise a problem will happens
    passenger_list *aux = elevator->passengers_inside;
    while (aux != NULL && (aux->passenger).final_floor != (*(elevator->actual_route))->floor) {
        aux = aux->next;
    }
    return aux;
    // if there's a passenger to gets off at this floor, it returns a pointer to this passenger, otherwise returns NULL
}

passenger_list *search_passenger_to_enter_list(elevators *elevator) {
    // this function searches a passenger that is outside the elevator and needs to get on
    // ps.: the elevator route should NOT be NULL, otherwise a problem will happens
    passenger_list *aux = elevator->passengers_to_enter;
    while (aux != NULL && (aux->passenger).initial_floor != (*(elevator->actual_route))->floor) {
        aux = aux->next;
    }
    return aux;
    // if there's a passenger to gets on at the elevator, it returns a pointer to this passenger, otherwise returns NULL
}

// DOING
passenger_list *search_passengers(passenger_list *passengers, int elevator_time) {
    // this function searches a passenger that is at the script
    passenger_list *aux = passengers;
    while (aux != NULL && elevator_time != (aux->passenger).call_cooldown) {
        aux = aux->next;
    }
    return aux;
    // if there's a passenger to be initialized, it returns a pointer to this passenger, otherwise returns NULL
}

int insert_floor_list(elevators *elevator, int floor) {
    // this function inserts the floor value in ascending order or in descending order, depending on elevator direction

    // verifying if the value to be added already is in the list (using && to short-circuit)
    if (!((elevator->actual_route) == NULL || *(elevator->actual_route) == NULL) && is_floor_in_list(
            *(elevator->actual_route), floor) == 1) {
        return 0; // the floor value was NOT added to the list because it already was in there
    }

    // adding the floor to the list
    if ((elevator->actual_route) == NULL || *(elevator->actual_route) == NULL) {
        // if the list is empty
        floor_list *aux = malloc(sizeof(floor_list));
        aux->floor = floor;
        aux->prev = NULL;
        aux->next = NULL;
        if (elevator->direction == 1) {
            elevator->up_route = aux;
            elevator->actual_route = &(elevator->up_route);
        } else {
            elevator->down_route = aux;
            elevator->actual_route = &(elevator->down_route);
        }
    } else {
        // if the list already has some floor
        floor_list *aux1 = malloc(sizeof(floor_list));

        // ascending order
        if (elevator->direction == 1 && elevator->actual_floor < floor) {
            // moving pointer to the immediately next value that is bigger than floor value
            floor_list *aux2 = elevator->up_route;
            while (floor > aux2->floor && aux2->next != NULL) {
                aux2 = aux2->next;
            }

            if (aux2 == elevator->up_route && floor < aux2->floor) {
                // it's at the beginning
                aux1->floor = floor;
                aux1->prev = NULL;
                aux1->next = elevator->up_route;
                elevator->up_route = aux1;
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
        } else if (elevator->direction == -1 && elevator->actual_floor > floor) {
            // descending order

            // moving pointer to the last value that is bigger than floor value
            floor_list *aux2 = elevator->down_route;
            while (floor < aux2->floor && aux2->next != NULL) {
                aux2 = aux2->next;
            }

            if (aux2 == elevator->down_route && floor > aux2->floor) {
                // it's at the beginning
                aux1->floor = floor;
                aux1->prev = NULL;
                aux1->next = elevator->down_route;
                elevator->down_route = aux1;
                aux2->prev = aux1;
            } else if (aux2->next == NULL && floor < aux2->floor) {
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
        } else {
            free(aux1);
            return 0; // the floor was NOT added
        }
    }
    return 1; // the floor value was added to the list
}

int remove_floor_list(elevators *elevator) {
    // this function removes floor value from beginning
    if (*(elevator->actual_route) == NULL) {
        return 0; // elevator actual_route is empty
    }
    floor_list *aux = *(elevator->actual_route);
    *(elevator->actual_route) = (*(elevator->actual_route))->next;
    if (*(elevator->actual_route) != NULL) {
        (*(elevator->actual_route))->prev = NULL;
    }
    free(aux);
    return 1; // the floor in the route has been removed
}

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

// DOING
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

// DOING
void insert_passenger_in_to_enter_list(elevators *elevator, passenger_list **passengers, int elevator_time) {
    // this function inserts all passengers with same time of elevator_time to elevator system
    while (search_passengers(*passengers, elevator_time) != NULL) {
        insert_passenger_to_enter_list(elevator, (search_passengers(*passengers, elevator_time))->passenger);
        remove_passenger_in_script(passengers, (search_passengers(*passengers, elevator_time))->passenger);
    }
}

// DOING
void insert_passenger_list(passenger_list **list, passengers passenger) {
    // inserindo do final
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

// DOING
void go_down(elevators *elevator) {
    // this function makes elevator go down one floor
    put_to_sleep(1); // the descent time
    --(elevator->actual_floor);
}

// DOING
void go_up(elevators *elevator) {
    // this function makes elevator go up one floor
    put_to_sleep(1); // the rise time
    ++(elevator->actual_floor);
}

// DOING
int invert_elevator_direction(elevators *elevator) {
    // this function reverses the direction of the elevator
    //    if (elevator->direction == 1 && (*(elevator->actual_route)) == NULL) {
    //        elevator->actual_route = &(elevator->down_route);
    //        elevator->direction = -1;
    //        return 1; // the direction was reversed
    //    }
    //    if (elevator->direction == -1 && (*(elevator->actual_route)) == NULL) {
    //        elevator->actual_route = &(elevator->up_route);
    //        elevator->direction = 1;
    //        return 1; // the direction was reversed
    //    }
    //    return 0; // the direction was NOT reversed

    if ((*(elevator->actual_route)) == NULL) {
        elevator->actual_route = elevator->direction == 1 ? &(elevator->down_route) : &(elevator->up_route);
        elevator->direction = elevator->actual_route == &(elevator->down_route) ? 1 : -1;
        return 1; // the direction was reversed
    }
    return 0; // the direction was NOT reversed
}

// DOING
int get_new_elevator_direction(elevators *elevator) {
    // this function returns the new elevator direction
    return (*(elevator->actual_route))->floor - elevator->actual_floor > 0
               ? 1
               : -1; // will go up = 1; will go down = -1
}

// DOING
void move_elevator(elevators *elevator, passenger_list *passengers) {
    // this function makes an elevator work
    while ((*(elevator->actual_route)) != NULL || passengers != NULL) {
        if (get_new_elevator_direction(elevator) == elevator->direction) {
            if ((*(elevator->actual_route))->floor == elevator->actual_floor) {
                // saida do elevador
                if(search_passenger_inside_list(elevator) != NULL) {
                    if (elevator->actual_floor == 1) {
                        printf("elevator exits at T floor: ");
                    } else {
                        printf("elevator exits at %d floor: ", elevator->actual_floor);
                    }
                    while (search_passenger_inside_list(elevator) != NULL) {
                        if((*(elevator->actual_route))->floor == elevator->actual_floor) {
                            remove_floor_list(elevator);
                        }
                        printf("%s ", (search_passenger_inside_list(elevator))->passenger.name);
                        remove_passenger_inside_list(elevator);
                    }
                    printf("\n");
                }

                // entrada no elevador
                if(search_passenger_to_enter_list(elevator) != NULL) {
                    if (elevator->actual_floor == 1) {
                        printf("elevator entrances at T floor: ");
                    } else {
                        printf("elevator entrances at %d floor: ", elevator->actual_floor);
                    }
                    while (search_passenger_to_enter_list(elevator) != NULL) {
                        insert_passenger_inside_list(elevator, (search_passenger_to_enter_list(elevator))->passenger);
                        if((*(elevator->actual_route))->floor == elevator->actual_floor) {
                            remove_floor_list(elevator);
                        }
                        insert_floor_list(elevator, (search_passenger_to_enter_list(elevator))->passenger.final_floor);
                        printf(":%s ", (search_passenger_to_enter_list(elevator))->passenger.name);
                        remove_passenger_to_enter_list(elevator);
                    }
                    printf("\n");
                }
            }
            if (elevator->direction == 1) {
                go_up(elevator);
            } else {
                go_down(elevator);
            }
        }
        while (search_passengers(passengers, elevator_time) != NULL) {
            // adicionar no to_enter e remove de passengers list
            insert_passenger_in_to_enter_list(elevator, &passengers, elevator_time);
            insert_floor_list(elevator, ((search_passengers(passengers, elevator_time))->passenger.initial_floor));
        }
        invert_elevator_direction(elevator);
    }
}

// TO DO
// int move_elevators(elevator_list *elevators) {
//     // this function makes the program work
//
//
//     ++elevator_time;
//     printf("elapsed time: %d\n", elevator_time);
//     return 1; // all elevators have been moved
// }

int print_elevator_route(elevators *elevator) {
    // this function prints the elevator route
    if (*(elevator->actual_route) == NULL) {
        printf("elevator route: [  ]\n");
        return 0; // up_route or down_route is empty
    }
    floor_list *aux = *elevator->actual_route;
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

void print_elevator_state(elevators *elevator) {
    // this function prints all elevator informations
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
    insert_passenger_list(&passengers, passenger3);
    insert_passenger_list(&passengers, passenger2);
    insert_passenger_list(&passengers, passenger1);


    passenger_list *aux = passengers;
    while (aux != NULL) {
        printf("%s\n", aux->passenger.name);
        aux = aux->next;
    }
    elevator_time = 9;
    printf("////////////////////\n");


    insert_passenger_in_to_enter_list(&elevator1, &passengers, elevator_time);

    aux = passengers;
    while (aux != NULL) {
        printf("%s\n", aux->passenger.name);
        aux = aux->next;
    }


    // testing some functions


    // closing application


    return 0;
}
