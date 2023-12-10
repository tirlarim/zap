# Conway's Game of Life Documentation

## Table of Contents

1. **Introduction**
   1. Purpose
   2. Overview
2. **Rules of the Game**
   1. Grid
   2. Cells
   3. Birth and Death
3. **Main Functions**
   1. `play`
   2. `tickArena`
   3. `drawNewFrame`
4. **Usage**
   1. Compile the Game
   2. Configuring Initial State
   3. Pausing and Resuming
   4. Change simulation speed
   5. Grid Customization
   6. Exiting the Game
5. **Maps**

## 1. Introduction

### 1.1 Purpose

Conway's Game of Life is a cellular automaton that simulates the evolution of cells on a two-dimensional grid. This documentation provides information on the rules of the game, implementation details, and how to use and configure the simulation.

### 1.2 Overview

The game consists of a grid of cells, each of which can be in one of two states: alive or dead. The state of a cell evolves over generations based on a set of rules.

## 2. Rules of the Game

### 2.1 Grid

The game is played on a two-dimensional grid of cells. Each cell has eight neighbors: horizontally, vertically, and diagonally adjacent.

### 2.2 Cells

- **Alive**: A live cell continues to live in the next generation.
- **Dead**: A dead cell may become alive in the next generation.

### 2.3 Birth and Death

- **Birth**: A dead cell with exactly three live neighbors becomes alive in the next generation.
- **Death**: A live cell with fewer than two live neighbors dies due to underpopulation, and a live cell with more than three live neighbors dies due to overpopulation.

## 3. Main Functions

- `void play(char* mapPath);`: Sets up the initial grid based on the map proposed.
- `bool tickArena(ARENA* arena, SETTINGS* settings)`: Advances the simulation to the next generation based on the game settings.
- `void drawNewFrame(ARENA* arena, SETTINGS* settings, unsigned int tickCount, bool isAlive)`: Draw current grid state, based on game settings

## 4. Usage

### 4.1 Compile the Game

To compile the game, type 

`gcc -Ofast -march=native -Werror=vla -Wall ps6/main.c ps6/life/engine.c ps6/life/graphic.c ps6/utils/utils.c -o lifeGame -lcurses`

#### The flag `-march=native` is extremely important. The code contains functions that need at least avx2 instructions support for the best compilation.

### 4.2 Configuring Initial State

To start a game, type in the terminal: 

`./lifeGame ./ps6/maps/acron.txt`

Also, you can create and use your own maps.

### 4.3 Pausing and Resuming

To pause game press `q` one time
To resume game press `q` again

### 4.4 Change simulation speed

Use `=` or `-` to increase or decrease simulation speed.

### 4.5 Grid Customization

Then game on pause (`q` pressed one time) press `space` to **_Add Cell_** or **_Remove Cell_** from current cursor pos.

To change the cursor position, use `Arrows` on your keyboard.

To toggle `Draw Mode`, press `Enter` 1 time.

To change `Camera Position`, use standard wasd keys.

### 4.6 Exiting the Game

To exit from game press `ESC` one time

## 5. Maps

1. acron.txt
2. aircraft-carrier.txt
3. barge.txt
4. beacon.txt
5. bee-hive.txt
6. blinkers.txt
7. block.txt
8. boat.txt
9. figure-eight.txt
10. glider-eater.txt
11. glider.txt
12. gosper-glider-gun-inf.txt
13. gosper-glider-gun.txt
14. heavyweight-spaceship-HWSS.txt
15. light-weight-spaceship-LWSS.txt
16. loaf.txt
17. long-boat.txt
18. long-ship.txt
19. middle-weight-spaceship-MWSS.txt
20. penta-decathlon.txt
21. pond.txt
22. pulsar.txt
23. queen-bee.txt
24. ship.txt
25. simkin-glider-gun.txt
26. still-life-20.txt
27. toad.txt
28. tub.txt
