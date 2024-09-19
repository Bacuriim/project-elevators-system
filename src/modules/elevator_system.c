#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "../headers/elevator_system.h"
#include "../headers/script_reader.h"

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
#endif

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

floor_list *search_floor(elevators *elevator) {
    // this function searches the actual_floor to remove from elevator route
    floor_list *aux = elevator->route;
    while (aux != NULL && aux->floor != elevator->actual_floor) {
        aux = aux->next;
    }
    return aux; // it's returns the pointer to the floor to remove
}

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

passenger_list *search_passenger_to_enter_list(elevators *elevator) {
    // this function searches a passenger that is outside the elevator and needs to get in
    // ps.: the elevator route should NOT be NULL, otherwise a problem will happens
    passenger_list *aux = elevator->passengers_to_enter;
    while (aux != NULL && (aux->passenger).initial_floor != elevator->actual_floor) {
        // (elevator->route)->floor
        aux = aux->next;
    }
    return aux;
    // if there's a passenger to gets on at the elevator, it returns a pointer to this passenger, otherwise returns NULL
}

passenger_list *search_passengers(passenger_list *passengers, int elevator_time) {
    // this function searches a passenger that is at the script
    passenger_list *aux = passengers;
    while (aux != NULL && elevator_time != (aux->passenger).call_cooldown) {
        aux = aux->next;
    }
    return aux;
    // if there's a passenger to be initialized, it returns a pointer to this passenger, otherwise returns NULL
}

int floor_counter(elevators elevator) {
    // this function counts how many floors there are on the route
    elevators aux = elevator;
    int floors = 0;
    if (aux.route == NULL) {
        return 0;
    } else {
        while (aux.route != NULL) {
            ++floors;
            aux.route = aux.route->next;
        }
        return floors; // it returns the count of floors
    }
}

elevator_list *search_elevator_v1(elevator_list *elevators) {
    // this function searches the elevator with the least routes
    elevator_list *aux1 = elevators; // percorre a lista de elevadores
    elevator_list *aux2 = NULL;
    int shortest_route = floor_counter(aux1->elevator);
    while (aux1 != NULL) {
        if (floor_counter(aux1->elevator) <= shortest_route) {
            shortest_route = floor_counter(aux1->elevator);
            aux2 = aux1;
        }
        aux1 = aux1->next;
    }
    return aux2; // it returns a pointer to the elevator
}

int passengers_to_enter_counter(elevators elevator) {
    // this function counts how many passengers are waiting to enter the elevator
    passenger_list *aux = elevator.passengers_to_enter;
    int passengers_waiting = 0;
    if (aux == NULL) {
        return 0;
    } else {
        while (aux != NULL) {
            ++passengers_waiting;
            aux = aux->next;
        }
        return passengers_waiting; // it returns the number of passengers
    }
}

elevator_list *search_elevator_v2(elevator_list *elevators) {
    // this function searches the elevator with the least passengers waiting to enter the elevator
    elevator_list *aux1 = elevators; // percorre a lista de elevadores
    elevator_list *aux2 = NULL;
    int shortest_passengers_waiting = passengers_to_enter_counter(aux1->elevator);
    while (aux1 != NULL) {
        if (passengers_to_enter_counter(aux1->elevator) <= shortest_passengers_waiting) {
            shortest_passengers_waiting = passengers_to_enter_counter(aux1->elevator);
            aux2 = aux1;
        }
        aux1 = aux1->next;
    }
    return aux2; // it returns a pointer to the elevator
}

int all_elevator_routes_are_null(elevator_list *elevators) {
    // this function find out if all elevators routes are NULL
    // obs.: the elevators parameter is NEVER NULL
    elevator_list *aux = elevators;
    while (aux != NULL) {
        if (aux->elevator.route != NULL) {
            return 0; // some route is not null
        }
        aux = aux->next;
    }
    if (aux == NULL) {
        return 1; // all routes are null
    }
}

int insert_floor_list_v1(elevators *elevator, int floor) {
    // this function inserts the floor value in ascending order

    // verifying if the value to be added already is in the list (using && to short-circuit)
    if ((elevator->route) != NULL && is_floor_in_list(elevator->route, floor) == 1) {
        return 0; // the floor value has NOT added to the list because it already was in there
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

int get_proximity(int floor1, int floor2) {
    // this function calculates the proximity between a given floor and the elevator
    return abs(floor1 - floor2);
}

int insert_floor_list_v2(elevators *elevator, int floor) {
    // this function inserts the floor value in proximity order
    // check if the floor is already in the list
    if (elevator->route != NULL && is_floor_in_list(elevator->route, floor) == 1) {
        return 0; // the floor was not added because it is already in the list
    }

    // create a new floor
    floor_list *aux1 = malloc(sizeof(floor_list));
    if (aux1 == NULL) {
        return -1; // return error code if memory allocation fails
    }
    aux1->floor = floor;

    // if the list is empty
    if (elevator->route == NULL) {
        aux1->prev = NULL;
        aux1->next = NULL;
        elevator->route = aux1;
    } else {
        // add the new floor temporarily at the end of the list
        floor_list *aux2 = elevator->route;
        while (aux2->next != NULL) {
            aux2 = aux2->next;
        }
        aux2->next = aux1;
        aux1->prev = aux2;
        aux1->next = NULL;
    }

    // reorder the list based on proximity to the current elevator floor
    floor_list *sorted_list = NULL; // new sorted list
    floor_list *aux2 = elevator->route;
    while (aux2 != NULL) {
        floor_list *next = aux2->next; // save the next floor

        // insert the floor into the new sorted list
        if (sorted_list == NULL || get_proximity(elevator->actual_floor, aux2->floor) < get_proximity(
                elevator->actual_floor, sorted_list->floor)) {
            // insert at the beginning
            aux2->next = sorted_list;
            aux2->prev = NULL;
            if (sorted_list != NULL) {
                sorted_list->prev = aux2;
            }
            sorted_list = aux2;
        } else {
            // insert in the middle or at the end
            floor_list *sorted_current = sorted_list;
            while (sorted_current->next != NULL && get_proximity(elevator->actual_floor, aux2->floor) >= get_proximity(
                    elevator->actual_floor, sorted_current->next->floor)) {
                sorted_current = sorted_current->next;
            }
            aux2->next = sorted_current->next;
            aux2->prev = sorted_current;
            if (sorted_current->next != NULL) {
                sorted_current->next->prev = aux2;
            }
            sorted_current->next = aux2;
        }
        aux2 = next; // move to the next floor
    }

    // update the route list
    elevator->route = sorted_list; // set the sorted list as the new route

    return 1; // the floor was added and the list was reordered
}

int remove_floor_list(elevators *elevator) {
    // this function removes a floor value from any place
    floor_list *aux = search_floor(elevator);
    if (elevator->route == NULL) {
        return 0; // elevator route is empty
    }

    if (elevator->route->floor == aux->floor) {
        // it's at the beginning
        (elevator->route) = (elevator->route)->next;
        if (elevator->route != NULL) {
            (elevator->route)->prev = NULL;
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
    // this function removes a passenger from the passenger list outside the elevator when some passenger have to
    // get in at this route floor
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

int remove_passenger_in_script(passenger_list **passengersP, passengers passenger) {
    // this function removes a passenger from passengers script list when elevator_time == call_cooldown
    passenger_list *aux = search_passengers(*passengersP, program_time);

    if (*passengersP == NULL || aux == NULL) {
        return 0; // the passenger in the script has not been removed
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
    return 1; // the passenger in the script has been removed
}

void insert_passenger_in_to_enter_list(elevators *elevator, passenger_list **passengers, int elevator_time) {
    // this function inserts all passengers with same time of elevator_time to elevator system
    while (search_passengers(*passengers, elevator_time) != NULL) {
        insert_passenger_to_enter_list(elevator, (search_passengers(*passengers, elevator_time))->passenger);
        insert_floor_list_v2(elevator, (search_passengers(*passengers, elevator_time))->passenger.initial_floor);
        remove_passenger_in_script(passengers, (search_passengers(*passengers, elevator_time))->passenger);
    }
}

void insert_passenger_list(passenger_list **list, passengers passenger) {
    // this function inserts a passenger from the script.txt to the passengers script list
    // inserting at the end
    if (*list == NULL) {
        // if the list is empty
        passenger_list *aux = malloc(sizeof(passenger_list));
        aux->passenger = passenger;
        aux->prev = NULL;
        aux->next = NULL;
        *list = aux;
    } else {
        // if the list already has some passenger
        passenger_list *aux1 = malloc(sizeof(passenger_list));
        aux1->passenger = passenger;
        aux1->next = NULL;

        // moving the pointer aux2 to the last data
        passenger_list *aux2 = *list;
        while (aux2->next != NULL) {
            aux2 = aux2->next;
        }
        aux2->next = aux1;
        aux1->prev = aux2;
    }
}

void insert_elevator_list(elevator_list **list, elevators elevator) {
    // this function inserts a elevator from the script.txt to the elevators script list
    // inserting at the end
    if (*list == NULL) {
        // if the list is empty
        elevator_list *aux = malloc(sizeof(elevator_list));
        aux->elevator = elevator;
        aux->prev = NULL;
        aux->next = NULL;
        *list = aux;
    } else {
        // if the list already has some elevator
        elevator_list *aux1 = malloc(sizeof(elevator_list));
        aux1->elevator = elevator;
        aux1->prev = NULL;

        (*list)->prev = aux1;
        aux1->next = *list;
        *list = aux1;

        //        elevator_list *aux1 = malloc(sizeof(elevator_list));
        //        aux1->elevator = elevator;
        //        aux1->next = NULL;
        //
        //        // moving the pointer aux2 to the last data
        //        elevator_list *aux2 = *list;
        //        while (aux2->next != NULL) {
        //            aux2 = aux2->next;
        //        }
        //        aux2->next = aux1;
        //        aux1->prev = aux2;
    }
}

int remove_elevator_list(elevator_list **list) {
    // this function removes a elevator from the elevators script list (from beginning)
    if (*list != NULL) {
        elevator_list *aux = *list;
        *list = (*list)->next;

        if (*list != NULL) {
            (*list)->prev = NULL;
        }
        free(aux);

        return 1;
    }
    return 0;
}

void insert_history_list(elevators *elevator, int floor) {
    // this function inserts at the end
    if (elevator->history == NULL) {
        // if the list is empty
        floor_list *aux = malloc(sizeof(floor_list));
        aux->next = NULL;
        aux->prev = NULL;
        aux->floor = floor;
        elevator->history = aux;
    } else {
        // if there is already something on the list
        floor_list *aux1 = malloc(sizeof(floor_list));
        aux1->next = NULL;
        aux1->floor = floor;

        // aux2 goes through the list to the last floor
        floor_list *aux2 = elevator->history;
        while (aux2->next != NULL) {
            aux2 = aux2->next;
        }
        (aux2->next) = aux1;
        aux1->prev = aux2;
    }
}

int remove_from_history_list(elevators *elevator) {
    // this function removes from the beginning
    if (elevator->history != NULL)
    {
        floor_list *aux = elevator->history;
        elevator->history = (elevator->history)->next;

        if (elevator->history != NULL)
        {
            (elevator->history)->prev = NULL;
        }
        free(aux);

        return 1; // removed
    }
    return 0; // the list is empty
}

void insert_timer_list(elevators *elevator, int time) {
    // this function inserts at the end
    if (elevator->time == NULL) {
        // if the list is empty
        time_list *aux = malloc(sizeof(time_list));
        aux->next = NULL;
        aux->prev = NULL;
        aux->time = time;
        elevator->time = aux;
    } else {
        // if there is already something on the list
        time_list *aux1 = malloc(sizeof(time_list));
        aux1->next = NULL;
        aux1->time = time;

        // aux2 goes through the list to the last time
        time_list *aux2 = elevator->time;
        while (aux2->next != NULL) {
            aux2 = aux2->next;
        }
        (aux2->next) = aux1;
        aux1->prev = aux2;
    }
}

int remove_from_timer_list(elevators *elevator) {
    // this function removes from the beginning
    if (elevator->time != NULL)
    {
        time_list *aux = elevator->time;
        elevator->time = (elevator->time)->next;

        if (elevator->time != NULL)
        {
            (elevator->time)->prev = NULL;
        }
        free(aux);

        return 1; // removed
    }
    return 0; // the list is empty
}

void print_elevator_history(elevators *elevator) {
    // this function prints the elevator summary
    floor_list *auxF = elevator->history;
    time_list *auxT = elevator->time;
    int maximum_time = 0;
    printf("%s: ", elevator->name);
    while(auxF != NULL && auxT != NULL) {
        printf("%d(%d)", auxF->floor, auxT->time);
        if(auxF->next != NULL && auxT->next != NULL) {
            printf("->");
        }
        if(maximum_time < auxT->time) {
            maximum_time = auxT->time;
        }
        auxF = auxF->next;
        auxT = auxT->next;
    }
    printf("\n");
    printf("Elevator travel time %s: %ds\n", elevator->name, maximum_time);
    printf("Number of floors traveled by elevator %s: %d\n", elevator->name, elevator->floor_counter);
    printf("\n");
}

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
    // this function prints all elevator information
    printf("CURRENT TIME: %d\n", program_time);
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

void go_down(elevators *elevator) {
    // this function makes elevator go down one floor
    --(elevator->actual_floor);
}

void go_up(elevators *elevator) {
    // this function makes elevator go up one floor
    ++(elevator->actual_floor);
}

int get_new_elevator_direction(elevators *elevator) {
    // this function returns the new elevator direction
    return (elevator->route)->floor - elevator->actual_floor >= 0 ? 1 : -1; // will go up = 1; will go down = -1
}

int invert_elevator_direction(elevators *elevator) {
    // this functions tries to invert the elevator direction
    if (elevator->route != NULL) {
        elevator->direction = get_new_elevator_direction(elevator);
        return 1; // the direction has been reversed
    }
    return 0; // the direction has not been reversed
}

void move_elevator(elevator_list **elevator, passenger_list **passengers) {
    // this function makes just one elevator work
    while ((*elevator)->elevator.route != NULL || *passengers != NULL) {
        print_elevator_state(&((*elevator)->elevator));
        // verify when the elevator bring to the floor route
        if ((*elevator)->elevator.route->floor == (*elevator)->elevator.actual_floor) {
            // people are exiting the elevator
            if (search_passenger_inside_list(&((*elevator)->elevator)) != NULL) {
                if ((*elevator)->elevator.actual_floor == 1) {
                    printf("(-)elevator exits at T floor: ");
                } else {
                    printf("(-)elevator exits at %d floor: ", (*elevator)->elevator.actual_floor);
                }
                while (search_passenger_inside_list(&((*elevator)->elevator)) != NULL) {
                    printf("%s ", search_passenger_inside_list(&((*elevator)->elevator))->passenger.name);
                    remove_passenger_inside_list(&((*elevator)->elevator));
                }
                printf("\n");
            }

            // people are entering the elevator
            if (search_passenger_to_enter_list(&((*elevator)->elevator)) != NULL) {
                if ((*elevator)->elevator.actual_floor == 1) {
                    printf("(+)elevator entrances at T floor: ");
                } else {
                    printf("(+)elevator entrances at %d floor: ", (*elevator)->elevator.actual_floor);
                }
                while (search_passenger_to_enter_list(&((*elevator)->elevator)) != NULL) {
                    int floor = (search_passenger_to_enter_list(&((*elevator)->elevator)))->passenger.final_floor;
                    char name[4];
                    strcpy(name, (search_passenger_to_enter_list(&((*elevator)->elevator)))->passenger.name);
                    insert_passenger_inside_list(&((*elevator)->elevator),
                                                 (search_passenger_to_enter_list(&((*elevator)->elevator)))->passenger);
                    remove_passenger_to_enter_list(&((*elevator)->elevator));
                    insert_floor_list_v2(&((*elevator)->elevator), floor);
                    printf("%s ", name);
                }
                printf("\n");
            }
            insert_history_list(&((*elevator)->elevator), (*elevator)->elevator.actual_floor);
            insert_timer_list(&((*elevator)->elevator), (*elevator)->elevator.timer);
            // it removes the actual floor from the floor_list
            remove_floor_list(&((*elevator)->elevator));
        }
        printf("\n");

        // searching the script's passenger list if there are any passengers to enter into the system
        while (search_passengers(*passengers, program_time) != NULL) {
            // add to to_enter and remove from passengers list
            insert_passenger_in_to_enter_list(&((*elevator)->elevator), passengers, program_time);
        }
        // it tries to reverse the direction of the elevator if necessary
        invert_elevator_direction(&((*elevator)->elevator));

        // moving the elevator
        if ((*elevator)->elevator.passengers_inside != NULL || (*elevator)->elevator.passengers_to_enter != NULL || (*elevator)->elevator.route != NULL ) {
            if ((&((*elevator)->elevator))->direction == 1) {
                go_up(&((*elevator)->elevator));
                put_to_sleep(1); // sleep fora second
                ++((*elevator)->elevator.floor_counter);
            } else {
                go_down(&((*elevator)->elevator));
                put_to_sleep(1); // sleep for a second
                ++((*elevator)->elevator.floor_counter);
            }
        }

        ++((*elevator)->elevator.timer);
        ++program_time; // adds in one the elevator time counter

        // if the elevator has to remain empty and stopped for a while until someone presses the button
        while ((*elevator)->elevator.route == NULL && *passengers != NULL) {
            printf("\n\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n\n");
            print_elevator_state(&((*elevator)->elevator));
            put_to_sleep(1);
            // searching the script's passenger list if there are any passengers to enter into the system
            while (search_passengers(*passengers, program_time) != NULL) {
                // add to to_enter and remove from passengers list
                insert_passenger_in_to_enter_list(&((*elevator)->elevator), passengers, program_time);
            }
            // it tries to reverse the direction of the elevator if necessary
            invert_elevator_direction(&((*elevator)->elevator));
            ++((*elevator)->elevator.timer);
            ++program_time; // adds in one the elevator time counter
        }
        printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n\n");
    }

    print_elevator_history(&((*elevator)->elevator));
    printf("END: Total time: %ds & Total movements: %d\n", program_time - 1, ((*elevator)->elevator).floor_counter);
}

void move_elevators(elevator_list **elevators, passenger_list **passengers) {
    // this function makes multiples elevators work
    elevator_list *aux = *elevators;
    while (all_elevator_routes_are_null(*elevators) != 1 || *passengers != NULL) {
        while (aux != NULL) {
            // if the elevator has to remain empty and stopped for a while until someone presses the button
            if (aux->elevator.route == NULL) {
                print_elevator_state(&(aux->elevator));
                printf("\n");
                if (aux->elevator.route == NULL && *passengers != NULL) {
                    // searching the script's passenger list if there are any passengers to enter into the system
                    while (search_passengers(*passengers, program_time) != NULL &&
                           aux == search_elevator_v2(*elevators)) {
                        // add to to_enter and remove from passengers list
                        insert_passenger_in_to_enter_list(&(aux->elevator), passengers, program_time);
                    }
                    // it tries to reverse the direction of the elevator if necessary
                    invert_elevator_direction(&(aux->elevator));
                }
                ++(aux->elevator.timer);
                aux = aux->next;
            } else {
                print_elevator_state(&(aux->elevator));
                // verify when the elevator bring to the floor route
                if ((aux)->elevator.route->floor == aux->elevator.actual_floor) {
                    // people are exiting the elevator
                    if (search_passenger_inside_list(&(aux->elevator)) != NULL) {
                        if (aux->elevator.actual_floor == 1) {
                            printf("(-)elevator exits at T floor: ");
                        } else {
                            printf("(-)elevator exits at %d floor: ", aux->elevator.actual_floor);
                        }
                        while (search_passenger_inside_list(&(aux->elevator)) != NULL) {
                            printf("%s ", search_passenger_inside_list(&(aux->elevator))->passenger.name);
                            remove_passenger_inside_list(&(aux->elevator));
                        }
                        printf("\n");
                    }

                    // people are entering the elevator
                    if (search_passenger_to_enter_list(&(aux->elevator)) != NULL) {
                        if (aux->elevator.actual_floor == 1) {
                            printf("(+)elevator entrances at T floor: ");
                        } else {
                            printf("(+)elevator entrances at %d floor: ", aux->elevator.actual_floor);
                        }
                        while (search_passenger_to_enter_list(&(aux->elevator)) != NULL) {
                            int floor = (search_passenger_to_enter_list(&(aux->elevator)))->passenger.final_floor;
                            char name[4];
                            strcpy(name, (search_passenger_to_enter_list(&(aux->elevator)))->passenger.name);
                            insert_passenger_inside_list(&(aux->elevator),
                                                         (search_passenger_to_enter_list(&(aux->elevator)))->passenger);
                            remove_passenger_to_enter_list(&(aux->elevator));
                            insert_floor_list_v2(&(aux->elevator), floor);
                            printf("%s ", name);
                        }
                        printf("\n");
                    }
                    insert_history_list(&(aux->elevator), (aux->elevator).actual_floor);
                    insert_timer_list(&(aux->elevator), (aux->elevator).timer);
                    remove_floor_list(&(aux->elevator));
                }
                printf("\n");

                // searching the script's passenger list if there are any passengers to enter into the system (using the search_v2 strategy)
                while (search_passengers(*passengers, program_time) != NULL && aux == search_elevator_v2(*elevators)) {
                    // add to to_enter and remove from passengers list
                    insert_passenger_in_to_enter_list(&(aux->elevator), passengers, program_time);
                }
                // it tries to reverse the direction of the elevator if necessary
                invert_elevator_direction(&(aux->elevator));

                // moving the elevator
                if (aux->elevator.passengers_inside != NULL || aux->elevator.passengers_to_enter != NULL || aux->elevator.route != NULL ) {
                    if ((&(aux->elevator))->direction == 1) {
                        go_up(&(aux->elevator));
                        ++(aux->elevator.floor_counter);
                    } else {
                        go_down(&(aux->elevator));
                        ++(aux->elevator.floor_counter);
                    }
                }

                // if the elevator has to remain empty and stopped for a while until someone presses the button
                if (aux->elevator.route == NULL && *passengers != NULL) {
                    // print_elevator_state(&(aux->elevator));
                    // printf("\n");
                    // searching the script's passenger list if there are any passengers to enter into the system
                    while (search_passengers(*passengers, program_time) != NULL &&
                           aux == search_elevator_v2(*elevators)) {
                        // add to to_enter and remove from passengers list
                        insert_passenger_in_to_enter_list(&(aux->elevator), passengers, program_time);
                    }
                    // it tries to reverse the direction of the elevator if necessary
                    invert_elevator_direction(&(aux->elevator));
                }
                ++(aux->elevator.timer);
                aux = aux->next;
            }
        }
        put_to_sleep(1); // time to sleep
        ++program_time;
        aux = *elevators;
        printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n\n");
    }
    aux = *elevators;
    int movements = 0;
    while(aux != NULL) {
        print_elevator_history(&(aux->elevator));
        movements = movements + aux->elevator.floor_counter;
        aux = aux->next;
    }
    printf("END: Total time: %ds & Total movements: %d\n", program_time - 1, movements);
}

int free_floor_list(floor_list **list) {
    if(*list != NULL) {
        while(*list != NULL) {
            floor_list *aux = *list;
            *list = (*list)->next;
            if(*list != NULL) {
                (*list)->prev = NULL;
            }
            free(aux);
        }
        return 1; // free all
    }
    return 0; // list already is empty
}

int free_time_list(time_list **list) {
    if(*list != NULL) {
        while(*list != NULL) {
            time_list *aux = *list;
            *list = (*list)->next;
            if(*list != NULL) {
                (*list)->prev = NULL;
            }
            free(aux);
        }
        return 1; // free all
    }
    return 0; // list already is empty
}

int free_passenger_list(passenger_list **list) {
    if(*list != NULL) {
        while(*list != NULL) {
            passenger_list *aux = *list;
            *list = (*list)->next;
            if(*list != NULL) {
                (*list)->prev = NULL;
            }
            free(aux);
        }
        return 1; // free all
    }
    return 0; // list already is empty
}

int free_elevator_list(elevator_list **list) {
    if(*list != NULL) {
        while(*list != NULL) {
            free_floor_list(&((*list)->elevator.route));
            free_floor_list(&((*list)->elevator.history));
            free_passenger_list(&((*list)->elevator.passengers_to_enter));
            free_passenger_list(&((*list)->elevator.passengers_inside));
            free_time_list(&((*list)->elevator.time));
            elevator_list *aux = *list;
            *list = (*list)->next;
            if(*list != NULL) {
                (*list)->prev = NULL;
            }
            free(aux);
        }
        return 1; // free all
    }
    return 0; // list already is empty
}

int free_all(passenger_list **listP, elevator_list **listE) {
    free_passenger_list(listP);
    free_elevator_list(listE);
    return 0;
}

int elevator_counter(elevator_list *elevatorsList) {
    elevator_list *aux = elevatorsList;
    int counter = 0;
    while (aux != NULL) {
        aux = aux->next;
        ++counter;
    }
    return counter;
}

int is_floor_interval_ok(int maximum_floor, passenger_list *passengersList, elevator_list *elevatorsList) {
    passenger_list *auxP = passengersList;
    elevator_list *auxE = elevatorsList;

    // passenger floors
    while (auxP != NULL) {
        if (auxP->passenger.initial_floor < 1 || auxP->passenger.initial_floor > maximum_floor) {
            return 0;
        }
        if (auxP->passenger.final_floor < 1 || auxP->passenger.final_floor > maximum_floor) {
            return 0;
        }
        auxP = auxP->next;
    }

    // elevators floors
    while (auxE != NULL) {
        if (auxE->elevator.actual_floor < 1 || auxE->elevator.actual_floor > maximum_floor) {
            return 0;
        }
        floor_list *auxF = elevatorsList->elevator.route;
        while (auxF != NULL) {
            if (auxF->floor < 1 || auxF->floor > maximum_floor) {
                return 0;
            }
            auxF = auxF->next;
        }
        auxE = auxE->next;
    }

    return 1; // it's all ok
}

int interpreter(instruction_list *instructionList, passenger_list **passengersList, elevator_list **elevatorsList) {
    char *inst = instructionList->instruction;
    char *saveptr; // to use with strtok_r
    char *token;

    if (strncasecmp(inst, "T", 1) == 0) {
        int cooldown = atoi(strtok_r(inst + 1, "_", &saveptr));
        int first_jump = cooldown < 10 ? 3 : 4;
        inst = inst + first_jump;

        if (strncasecmp(inst, "P", 1) == 0) {
            char *aux = strtok_r(inst, "_", &saveptr);
            passengers passenger;
            passenger.call_cooldown = cooldown;
            strncpy(passenger.name, aux, sizeof(passenger.name));
            inst = inst + 4;

            if (strncasecmp(inst, "S", 1) == 0) {
                passenger.direction = 1;
            } else if (strncasecmp(inst, "D", 1) == 0) {
                passenger.direction = -1;
            } else {
                printf("Passenger direction invalid!");
                return 1; // problem at the script
            }

            inst = inst + 2;
            passenger.initial_floor = (strncasecmp(inst, "T", 1) == 0) ? 1 : atoi(strtok_r(inst, "_", &saveptr));
            int second_jump = passenger.initial_floor < 10 ? 2 : 3;
            inst = inst + second_jump;
            passenger.final_floor = (strncasecmp(inst, "T", 1) == 0) ? 1 : atoi(strtok_r(inst, "_", &saveptr));

            insert_passenger_list(passengersList, passenger);
            //            printf("\nPASSENGER NAME: %s", passenger.name);
            //            printf("\nPASSENGER DIRECTION: %d", passenger.direction);
            //            printf("\nPASSENGER INIT FLOOR: %d", passenger.initial_floor);
            //            printf("\nPASSENGER FINAL FLOOR: %d", passenger.final_floor);
            return 0; // no problem
        } else {
            printf("Invalid passenger!");
            return 1; // problem at the script
        }
        //        printf("\nCOOLDOWN: %d\n", cooldown);
    } else if (strncasecmp(inst, "E", 1) == 0) {
        elevators elevator;
        elevator.history = NULL;
        elevator.time = NULL;
        elevator.route = NULL;
        elevator.passengers_to_enter = NULL;
        elevator.passengers_inside = NULL;
        elevator.timer = 0;
        elevator.floor_counter = 0;

        token = strtok_r(inst, "_", &saveptr);
        strncpy(elevator.name, token, sizeof(elevator.name));

        token = strtok_r(NULL, "_", &saveptr);
        elevator.actual_floor = atoi(token);
        insert_history_list(&elevator, elevator.actual_floor);
        insert_timer_list(&elevator, 0);

        token = strtok_r(NULL, "_", &saveptr);
        if (strncasecmp(token, "S", 1) == 0) {
            elevator.direction = 1;
        } else if (strncasecmp(token, "D", 1) == 0) {
            elevator.direction = -1;
        } else {
            printf("Elevator direction invalid!");
            return 1; // problem at the script
        }

        // process floor list
        token = strtok_r(NULL, ",", &saveptr);
        while (token != NULL) {
            int floor = (strncasecmp(token, "T", 1) == 0) ? 1 : atoi(token);
            insert_floor_list_v2(&elevator, floor);
            token = strtok_r(NULL, ",", &saveptr); // next floor
        }
        //        printf("\nELEVATOR NAME: %s", elevator.name);
        //        printf("\nELEVATOR AC FLOOR: %d", elevator.actual_floor);
        //        printf("\nELEVATOR DIRECTION: %d\n", elevator.direction);
        //        print_elevator_route(&elevator);
        insert_elevator_list(elevatorsList, elevator);

        if (elevator_counter(*elevatorsList) > 6) {
            printf("No more than 6 elevators are allowed");
            return 1; // problem at the script
        }
        return 0; // no problem
    } else if (strncasecmp(inst, "AM_", 3) == 0) {
        inst = inst + 3;
        if (atoi(inst) >= 5 && atoi(inst) <= 25) {
            maximum_floor = atoi(inst);

            if (is_floor_interval_ok(maximum_floor, *passengersList, *elevatorsList) != 1) {
                printf(
                        "The passenger floor range or the elevator floor range does not comply with the building's maximum floor");
                return 1; // problem at the script
            }

            return 0; // no problem
        } else {
            printf("Invalid Maximum Floor!");
            return 1; // problem at the script
        }
        //        printf("\nMAXIMUM FLOOR: %d\n\n", maximum_floor);
    } else {
        printf("Invalid instruction");
        return 1; // problem at the script
    }
}