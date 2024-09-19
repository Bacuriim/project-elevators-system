#ifndef PROJECT_ELEVATOR_ELEVATOR_SYSTEM_H
#define PROJECT_ELEVATOR_ELEVATOR_SYSTEM_H

#include "script_reader.h"

// STRUCTS
typedef struct doubly_linked_floor_list {
    int floor;
    struct doubly_linked_floor_list *next;
    struct doubly_linked_floor_list *prev;
} floor_list;

typedef struct doubly_linked_time_list {
    int time;
    struct doubly_linked_time_list *next;
    struct doubly_linked_time_list *prev;
} time_list;

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
    int timer; // travel time
    int floor_counter;
    struct doubly_linked_floor_list *route; // the route the elevator must follow
    struct doubly_linked_passenger_list *passengers_inside; // the passengers who are inside the elevator
    struct doubly_linked_passenger_list *passengers_to_enter;
    // the passengers that pressed the button and are waiting for enter the elevator
    struct doubly_linked_floor_list *history;
    struct doubly_linked_time_list *time;
} elevators;

typedef struct doubly_linked_elevator_list {
    elevators elevator;
    struct doubly_linked_elevator_list *next;
    struct doubly_linked_elevator_list *prev;
} elevator_list;

// GLOBAL VARIABLES
extern int program_time;
extern int maximum_floor;

// FUNCTIONS
void put_to_sleep(int seconds);
int is_floor_in_list(floor_list *list, int floor);
floor_list *search_floor(elevators *elevator);
passenger_list *search_passenger_inside_list(elevators *elevator);
passenger_list *search_passenger_to_enter_list(elevators *elevator);
passenger_list *search_passengers(passenger_list *passengers, int elevator_time);
int floor_counter(elevators elevator);
elevator_list *search_elevator_v1(elevator_list *elevators);
int passengers_to_enter_counter(elevators elevator);
elevator_list *search_elevator_v2(elevator_list *elevators);
int all_elevator_routes_are_null(elevator_list *elevators);
int insert_floor_list_v1(elevators *elevator, int floor);
int get_proximity(int floor1, int floor2);
int insert_floor_list_v2(elevators *elevator, int floor);
int remove_floor_list(elevators *elevator);
void insert_passenger_inside_list(elevators *elevator, passengers passenger);
int remove_passenger_inside_list(elevators *elevator);
void insert_passenger_to_enter_list(elevators *elevator, passengers passenger);
int remove_passenger_to_enter_list(elevators *elevator);
int remove_passenger_in_script(passenger_list **passengersP, passengers passenger);
void insert_passenger_in_to_enter_list(elevators *elevator, passenger_list **passengers, int elevator_time);
void insert_passenger_list(passenger_list **list, passengers passenger);
void insert_elevator_list(elevator_list **list, elevators elevator);
int remove_elevator_list(elevator_list **list);
void insert_history_list(elevators *elevator, int floor);
int remove_from_history_list(elevators *elevator);
void insert_timer_list(elevators *elevator, int time);
int remove_from_timer_list(elevators *elevator);
void print_elevator_history(elevators *elevator);
int print_elevator_route(elevators *elevator);
int print_elevator_passengers_inside(elevators *elevator);
int print_elevator_passengers_to_enter(elevators *elevator);
void print_elevator_state(elevators *elevator);
void go_down(elevators *elevator);
void go_up(elevators *elevator);
int get_new_elevator_direction(elevators *elevator);
int invert_elevator_direction(elevators *elevator);
void move_elevator(elevator_list **elevator, passenger_list **passengers);
void move_elevators(elevator_list **elevators, passenger_list **passengers);
int free_floor_list(floor_list **list);
int free_time_list(time_list **list);
int free_passenger_list(passenger_list **list);
int free_elevator_list(elevator_list **list);
int free_all(passenger_list **listP, elevator_list **listE);
int elevator_counter(elevator_list *elevatorsList);
int is_floor_interval_ok(int maximum_floor, passenger_list *passengersList, elevator_list *elevatorsList);
int interpreter(instruction_list *instructionList, passenger_list **passengersList, elevator_list **elevatorsList);

#endif
