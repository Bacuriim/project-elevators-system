# Elevators System Project

## 🎯General Objective

- Data Structure work that consists of creating an elevator system using pointers in C.
- Implement some data structures learned in the course in this project.

## 📝Instructions

- Write a program that defines the up_route to be taken by n elevators to meet the demand of m people. To do this, an algorithm that defines the up_route to be taken by each of the elevators to meet the demand of the people must be developed, as well as a code that abstracts the operation of a group of elevators must be implemented.

### 🌐General considerations

- People can be inside one of the elevators, as well as outside the elevator on some floor with the intention of going to another destination floor.

- People inform the destination floor as soon as they enter the elevator.

- A person who is outside the elevator presses the up or down button to "call" the elevator.

- The n elevators have the same number of floors and provide access to the tower of interest, that is, any of the elevators serves (allows access to) the location (apartment, store, etc.) of a person's destination.

### 🖥️Considerations for simulation/modeling

- An elevator moves at a speed of 1 floor/second. For example, in 5 seconds an elevator moves from the 5th to the 10th floor; or in 3 seconds an elevator moves from the 3rd to the 6th floor.

- The time it takes for people to enter or exit is very small, that is, it can be assumed that people enter or exit instantaneously.

- The elevator checks every second if it needs to create/change the up_route, including to check if it needs to stop on any floor for people to exit or enter an elevator.

- A string with information from the case study to be executed by the algorithm will be read to indicate/simulate the current state of the elevators and when (in discrete periods of 1 second) people will request the service of an elevator.

### 📏Parameters to be considered

- Number "n" of elevators, where "n" can vary from 1 to 6.

- Number "a" of floors, where "a" can vary from 5 to 25.

### 📚Case Studies

- Case studies will be used to evaluate the performance of the up_route generation program/algorithm. For this, the metric to be used is the total number of floors traveled to meet the demand of the case studies. In other words, the fewer floors traveled, the more efficient the program/algorithm is.

### ✨Example

Case study 1:

E1 - elevator 1

E2 - elevator 2

—- | | —---------

En - elevator n

## 

P01 - person 1

P02 - person 2

—- | | —--------- 

Pm - person m 


    AM_25 E1_04_S_6,9,8 E2_11_D_5,8,9,3,2,T E3_20_D_5,8,T,9 T5_P01_S_4_6 T8_P01_D_6_4 T9_P02_S_5_10 T10_P03_S_3_9 T10_P04_D_6_T T10_P05_S_8_15 T15_P06_D_9_2 T15_P07_S_2_13 T18_P08_D_8_T T21_P01_D_16_3 T21_P10_S_T_13 T21_P11_S_T_12 T23_P12_S_T_15 T28_P13_S_2_13

- AM_25 means that the maximum floor is the 25th.

- E1_04_S_6,9,8 means that the elevator is on the 4th floor, going up and that the people in the elevator had pressed floors 6, 9 and 8.

- T5_P01_S_4_6 means that person 1 at time 5 seconds was on the 4th floor and when he enters the elevator he will press the button to go to the 6th floor.

## 

## 🗂️Project Overview

- The application consists of two modes: single elevator and multi elevators.

### 🛗Single Elevator

#### 🧩Internal Configuration

- The elevator will have access to "a" floors, between 5 and 25, so there will be "a" buttons for each floor, including the first floor.

#### 🏢External Configuration

- The elevator will have two buttons: one to go up and one to go down.

#### ⚙️Standard Operation

- Whenever there is no external or internal call, it will go to the first floor and will remain with the doors open.

#### 📊Operation Of Cases For a Single Elevator

- If the elevator starts an action of going up or down, it must complete its action while picking up passengers with that same action, going up or down.

- The next passenger to be disembarked is always the one who, in the same action, going up or down, is closest to the current floor of the elevator.

##

### 🛗🛗Multi Elevators

#### 🧩Internal Configuration

- The elevator will have access to "a" floors, between 5 and 25, so there will be "a" buttons for each floor, including the first floor.

#### 🏢External Configuration

- The elevator will have a floor selection panel, which will indicate which elevator, which can vary from 2 to 6, you should enter.

#### ⚙️Standard Operation

- Whenever there is no external or internal call, it will go to the first floor and will remain with the doors open.

#### 📘How To Operate Multiple Elevators

- As long as there are empty elevators stopped on the first floor, any new person who is on any floor and types on the panel will be redirected to one of these elevators.

- With all elevators occupied, any new person who types a number on the panel will be redirected to the elevator with the same action, up or down, and with the destination within its current up_route, and also with the final destination of its current up_route close to its current floor. If all elevators have different actions than the user's action, he will be redirected to the one closest to completing its up_route.

- The next passenger to be disembarked is always the one who, in the same action, getting off or going up, is closest to the current floor of the elevator.

## 🏆Team

- Conrado Einstein
- Hiel Saraiva

## 🆙Versions

- C99

## 🔧Tools

- [CLion](https://www.jetbrains.com/clion/)
