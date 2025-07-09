# Cellular Automata Simulator

A C program that simulates elementary cellular automata (ECA) and solves the density classification problem using rules 184 and 232, as described in Assignment 2 for COMP10002 (Foundations of Algorithms, Semester 2, 2024).

## Features

- **Stage 0**: Reads and visualizes ECA configurations (size, rule, initial state).
- **Stage 1**: Executes the automaton for a given number of steps and tracks cell states.
- **Stage 2**: Solves the density classification problem using Fukš's method (rules 184/232).
- **Dynamic memory**: Handles arbitrary-sized automata without hardcoded limits.

## Input/Output

- **Input**: Instructions via stdin (e.g., automaton size, rule code, initial state, steps).
- **Output**: Prints automaton evolution, statistics, and classification results to stdout.  
  (See `test0.txt` and `test0-out.txt` in the assignment PDF for examples.)

## How to Build & Run

1. Compile:  
   ```bash
   gcc -Wall -o program program.c -std=c99 -lm
./program < test0.txt



