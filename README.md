# Elevators System Project

## 🎯General Objective

- Data Structure work that consists of creating an elevator system using pointers in C.
- Implement some data structures learned in the course in this project.

## 🗂️Project Overview

- The application consists of two modes: single elevator and multi elevators.

### 🛗Single Elevator

#### 🧩Internal Configuration

- The elevator will have access to N floors, so there will be N buttons for each floor, including the first floor and possible basements, plus 3 buttons, one to close the doors, one to open the doors and the emergency button.

#### 🏢External Configuration

- The elevator will have two buttons: one to go up and one to go down.

#### ⚙️Standard Operation

- Whenever there is no external or internal call, it will go to the first floor and will remain with the doors open.

- The internal emergency button, when pressed, will send the elevator to the first floor and will ask everyone to leave it, and after a while it will return to standard operation.

- The internal button to close the doors, when pressed, will close the doors if the doors are open. If the doors are already closed, they will remain closed.

- The internal door open button, when pressed, if the doors are closed, will open the doors, but only if the elevator is stopped on a certain floor. If the doors are open, they will remain open.

#### 📊Operation Of Cases For a Single Elevator

- If the elevator starts an action of going up or down, it must complete its action while picking up passengers with that same action, going up or down.

- The next passenger to be disembarked is always the one who, in the same action, going up or down, is closest to the current floor of the elevator.

##

### 🛗🛗Multi Elevators

#### 🧩Internal Configuration

- The elevator will have only 3 buttons: open doors, close doors and emergency.

#### 🏢External Configuration

- The elevator will have a floor selection panel, which will indicate which elevator you should enter.

#### ⚙️Standard Operation

- Whenever there is no external or internal call, it will go to the first floor and will remain with the doors open.

- The internal emergency button, when pressed, will send only the elevator whose button was pressed to the first floor and will ask everyone to exit it, and after a while it will return to standard operation.

- The internal close doors button, when pressed, will close the doors if the doors are open. If the doors are already closed, they will remain closed.

- The internal door open button, when pressed, if the doors are closed, will open the doors, but only if the elevator is stopped on a certain floor. If the doors are open, they will remain open.

#### 📘How To Operate Multiple Elevators

- As long as there are empty elevators stopped on the first floor, any new person who is on any floor and types on the panel will be redirected to one of these elevators.

- With all elevators occupied, any new person who types a number on the panel will be redirected to the elevator with the same action, up or down, and with the destination within its current route, and also with the final destination of its current route close to its current floor. If all elevators have different actions than the user's action, he will be redirected to the one closest to completing its route.

- The next passenger to be disembarked is always the one who, in the same action, getting off or going up, is closest to the current floor of the elevator.

## 🏆Team

- Conrado Einstein
- Hiel Saraiva

## 🆙Versions

- C99

## 🔧Tools

- [CLion](https://www.jetbrains.com/clion/)
