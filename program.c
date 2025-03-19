/* Program to perform 1D cellular automaton (CA) computations and to use 1D CA
   to solve the density classification problem.

  Skeleton program written by Artem Polyvyanyy, http://polyvyanyy.com/,
  September 2024, with the intention that it be modified by students
  to add functionality, as required by the assignment specification.
  All included code is (c) Copyright University of Melbourne, 2024.

  Authorship Declaration:

  (1) I certify that except for the code provided in the initial skeleton file,
  the program contained in this submission is completely my own individual
  work, except where explicitly noted by further comments that provide details
  otherwise. I understand that work that has been developed by another student,
  or by me in collaboration with other students, or by non-students as a result
  of request, solicitation, or payment, may not be submitted for assessment in
  this subject. I understand that submitting for assessment work developed by
  or in collaboration with other students or non-students constitutes Academic
  Misconduct, and may be penalized by mark deductions, or by other penalties
  determined via the University of Melbourne Academic Honesty Policy, as
  described at https://academicintegrity.unimelb.edu.au.

  (2) I also certify that I have not provided a copy of this work in either
  softcopy or hardcopy or any other form to any other student, and nor will I
  do so until after the marks are released. I understand that providing my work
  to other students, regardless of my intention or any undertakings made to me
  by that other student, is also Academic Misconduct.

  (3) I further understand that providing a copy of the assignment specification
  to any form of code authoring or assignment tutoring service, or drawing the
  attention of others to such services and code that may have been made
  available via such a service, may be regarded as Student General Misconduct
  (interfering with the teaching activities of the University and/or inciting
  others to commit Academic Misconduct). I understand that an allegation of
  Student General Misconduct may arise regardless of whether or not I personally
  make use of such solutions or sought benefit from such actions.

  Signed by: [Rasheeduddin Ahmed Mohammed 1615917]
  Dated:     [2/10/2024]
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

/* #DEFINE'S -----------------------------------------------------------------*/
#define SDELIM "==STAGE %d============================\n"   // stage delimiter
#define MDELIM "-------------------------------------\n"    // delimiter of -'s
#define THEEND "==THE END============================\n"    // end message

#define CRTRNC '\r'     // carriage return character
#define NBRHDS 8        // number of possible neighborhoods

#define BUFFER_SIZE 64
#define NUMOFLINES 6


#define DEBUG 0

/* TYPE DEFINITIONS ----------------------------------------------------------*/
typedef char cells_t;            // base type to store states of cells
typedef struct state state_t;    // a cellular automaton state

struct state {                   // a state in a CA is defined by
    cells_t* clls;               // ... an array of cells and
    state_t* next;               // ... a link to the next state
};

typedef struct {                 // a run of a CA consists of
    state_t* head;               // ... the initial state and
    state_t* foot;               // ... the current state,
} run_t;                         // implemented as a linked list of states


/* FUNCTION PROTOTYPES ----------------------------------------------------------*/
int mygetchar(void);            
void do_stage0(char **contents);
char* do_stage1(char **contents);
void do_stage2(char **contents, char *last_state); 

run_t* create_empty_run();
run_t* insert_state(run_t *run, cells_t *clls, int size);
void free_run(run_t *run);

char** get_contents(); 
void print_contents(char **contents);
void free_contents(char **contents);
void free_stage(char A[]);

char get_next_cell(char i, char j, char k, char pattern[]); 
char *get_next_stage(char A[], int size, char pattern[]);
void convert_pattern(int pattern[], char result[]);
void get_pattern(int pattern[], int rule);
void parse_integers(char *input, int *num1, int *num2);
char *processrun(state_t* current, int start, int cell, char* array, int* curr, int size);

/* WHERE IT ALL HAPPENS ------------------------------------------------------*/
int main(int argc, char *argv[]) {

    char** file_contents = get_contents();

    do_stage0(file_contents);
    char *last_state = do_stage1(file_contents);
    do_stage2(file_contents, last_state);

    #if DEBUG 
    print_contents(file_contents); 
    free_contents(file_contents);
    #endif

    return EXIT_SUCCESS;        // algorithms are fun!!!
}

/* USEFUL FUNCTIONS ----------------------------------------------------------*/

/* NOTE: create_empty run() insert_state(run_t *run, cells_t *clls, int size) free_run(run_t *run)
  are modified functions from Alistair Moffat "Programming, Problem Solving, and Abstraction with C" */

/* Allocates memory for a CA evolution and returns pointer of run*/
run_t* create_empty_run() {
    // Allocating memory for run
    run_t* run = (run_t*)malloc(sizeof(run_t)); 
    assert(run != NULL);

    run->head = run->foot = NULL;
    return run; 
}

/* Inserts new state into run, updating the foot and head of the run accordingly. Returns
   pointer to run*/
run_t* insert_state(run_t *run, cells_t *clls, int size) {
    
    // Allocates memory for states
    state_t* new_state = (state_t*)malloc(sizeof(state_t));
    assert(run != NULL && new_state != NULL);

    new_state->clls = (cells_t*)malloc(size * sizeof(cells_t));
    assert(new_state->clls != NULL);
    
    // Copy the contents of clls to the new state
    strncpy(new_state->clls, clls, size);
    new_state->next = NULL;
    
    // updates run to include new state 
    if (run->foot == NULL) {
        // First element of the list
        run->head = run->foot = new_state;
    } else {
        // add new state to the foot of run
        run->foot->next = new_state; 
        run->foot = new_state; 
    }
    return run;
}

/* Frees all memory allocated for the run*/
void free_run(run_t *run) {
    state_t *curr, *prev;
    assert(run != NULL); 
    curr = run->head;
    while (curr) {
        prev = curr; 
        curr = curr->next;
        free(prev->clls);  // Free the array of cells
        free(prev);        // Free the state structure
    }
    free(run);  // Finally, free the run itself
}

/* Reads input lines from txt file and dynamically allocates it into an array of strings. Returns 
   pointer to array */
char** get_contents() {
    char buffer[BUFFER_SIZE];
    int i = 0;
    int length = 0;

    // Dynamamically allocates memory for txt file
    char **contents = (char**)malloc(NUMOFLINES * sizeof(char*)); 
    assert(contents); 

    // Reading from stdin 
    while (i < NUMOFLINES && fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
        length = strlen(buffer);
        // Allocate memory for each line 
        contents[i] = (char*)malloc(sizeof(char) * (length + 1)); 
        assert(contents[i]);

        // copying txt file lines to array
        strcpy(contents[i], buffer);
        i++;
    }
  
    // If fewer lines were read than NUMOFLINES, set the remaining pointers to NULL
    for (; i < NUMOFLINES; i++) {
        contents[i] = NULL;
    }

    return contents; 
}

/* Print the contents from text file */
void print_contents(char **contents) {
    if (contents != NULL) {
        for (int i = 0; i < NUMOFLINES; i++) {
            printf("Line %d: %s", i + 1, contents[i]);
        }
        printf("\n");
    }
}

/* free the contents of the text file */
void free_contents(char **contents) {
    if (contents != NULL) {
        for (int i = 0; i < NUMOFLINES; i++) {
            free(contents[i]); // Free each line's memory
        }
    }
    free(contents); // Free the array of pointers
}

/* Takes CA current state, length of string, and pattern to return the next stage 
   for CA */
char *get_next_stage(char A[], int size, char pattern[]) {
    // Dynamically allocate memory for the next stage array
    char *B = (char *)malloc(size * sizeof(char));
    assert(B);

    // Generate the next state for the middle cells
    for (int i = 1; i < size - 1; i++) {
        B[i] = get_next_cell(A[i-1], A[i], A[i+1], pattern);
    }

    // Handle the first and last cells (circular behavior)
    B[0] = get_next_cell(A[size - 1], A[0], A[1], pattern);
    B[size - 1] = get_next_cell(A[size - 2], A[size - 1], A[0], pattern);
    
    return B;
}

/*Takes the state of 3 neigbouring cells, i, j, k and CA update rule to generate 
  the next state of a cell based on neighbors
*/
char get_next_cell(char i, char j, char k, char pattern[]) {
    char next;
    if (i == '.' && j == '.' && k == '.') {
        next = pattern[0];
    } else if (i == '.' && j == '.' && k == '*') {
        next = pattern[1];
    } else if (i == '.' && j == '*' && k == '.') {
        next = pattern[2];
    } else if (i == '.' && j == '*' && k == '*') {
        next = pattern[3];
    } else if (i == '*' && j == '.' && k == '.') {
        next = pattern[4];
    } else if (i == '*' && j == '.' && k == '*') {
        next = pattern[5];
    } else if (i == '*' && j == '*' && k == '.') {
        next = pattern[6];
    } else if (i == '*' && j == '*' && k == '*') {
        next = pattern[7];
    }
    return next; 
}

/* Converts CA update rule from 0s and 1s to a character array of . and *. With 0 corresponding to . and * to 1.*/ 
void convert_pattern(int pattern[], char result[]) {
    for (int i = 0; i < 8; i++) {
        if (pattern[i] == 0) {
            result[i] = '.';
        } else {
            result[i] = '*';
        }
    }
}

/* Generates CA update rule from the rule number and updates pattern to it*/ 
void get_pattern(int pattern[], int rule) {

    // divide rule by 2 and store the remainder to get pattern_int
    if (rule > 254) { 
        printf("Rule out of bounds\n");
        exit(EXIT_FAILURE); 
    }
    for (int i = 0; rule > 0; i++) {
        pattern[i] = rule % 2; 
        rule = rule / 2; 
    }
}

/* Separates a string of 2 integers to two integers with ',' acting as delim */
void parse_integers(char *input, int *num1, int *num2) {
    int i = 0, current_number = 0;

    while (input[i] != ',') {
        current_number = current_number * 10 + (input[i] - '0');
        i++;
    }
    *num1 = current_number; // Assign first number with proper sign

    // Move past the comma
    i++;

    // Reset for second number
    current_number = 0;

    while (input[i] != '\n' && input[i] != '\0') { // handle null-terminated string
        current_number = current_number * 10 + (input[i] - '0');
        i++;
    }
    *num2 = current_number; // Assign second number with proper sign
}


/* Processes a run; 
   takes in current, a CA run head; start and cell, positions specifying data to be stored; array, an array to store specific clls; curr, 
   evolving time of state; size, size of state
   It travels along the CA linked list printing out the evolution, updating array to contain specific clls, and returning the last state 
   of evolution  
    */
char *processrun(state_t* current, int start, int cell, char* array, int* curr, int size) {
    // Traverse and print the linked list for rule 232
    char* last_state = (char *)malloc((size) * sizeof(char));
    assert(last_state);


    while (current != NULL) {
        // prints linked list data
        printf("%4d: ", *curr);
        for (int i = 0; i < size; i++) {
            printf("%c", current->clls[i]);
        }
        printf("\n");
        //stores cell & start state info in array
        if (*curr >= start) {
            array[*curr - start] = current->clls[cell];
        }
       
        // store last state
        if (current->next == NULL) {
            strncpy(last_state, current->clls, size); 
        }
        current = current->next;
        (*curr)++;
    }

    return last_state; 

}

/* does stage0 of the program taking into the contents of the txt file*/
void do_stage0(char **contents) {
    printf(SDELIM, 0);

    int size = atoi(contents[0]);
    int rule = atoi(contents[1]);

    // getting and converting pattern from rule
    int int_pattern[8] = {0};
    get_pattern(int_pattern, rule); 

    printf("SIZE: %d\n", size);
    printf("RULE: %d\n", rule); 

    printf("%s", MDELIM);
    printf(" 000 001 010 011 100 101 110 111\n");

    // printing pattern for rule
    printf("%3d", int_pattern[0]);
    for (int i = 1; i < 8; i++) {
        printf("%4d", int_pattern[i]);
    }
    printf(" \n");

    // printing initial state
    printf("%s", MDELIM);
    printf("%4d: %s", 0, contents[2]);

}

/* does stage1 of the program taking in contents of txt file and 
 returning the last state of the CA*/
char* do_stage1(char **contents) {
    printf(SDELIM, 1);

    int size = atoi(contents[0]);
    int rule = atoi(contents[1]);
    int elts = atoi(contents[3]);

    // getting and converting pattern from rule
    int int_pattern[8] = {0}; 
    char pattern[8];
    get_pattern(int_pattern, rule); 
    convert_pattern(int_pattern, pattern);

    // Initial conditions
    run_t* run = create_empty_run(); 
    run = insert_state(run, contents[2], size);

    char *next_stage = get_next_stage(contents[2], size, pattern); 

    for (int i = 0; i < elts; i++) {
        run = insert_state(run, next_stage, size);
        next_stage = get_next_stage(next_stage, size, pattern);
    }

    // getting start & cell no from txt
    int start = 0; 
    int cell = 0; 
    parse_integers(contents[4], &cell, &start); 

    // Traverse and print the linked list
    int curr = 0; 

    char * array = (char *)malloc((elts - start + 1) * sizeof(char));
    char *last_state = processrun(run->head, start, cell, array, &curr, size);

    // separator 
    printf("%s", MDELIM);

    // printing on and off states from start at cell no.
    int on = 0, off = 0;
    for (int i = 0; i < curr - start; i++) {
         if (array[i] == '*') {
            on++;
        } else {
            off++;
        }
    }
    printf("#ON=%d #OFF=%d CELL#%d START@%d\n", on, off, cell, start);

    #if DEBUG
    // Print the contents of the array
    printf("Contents of the array: ");
    for (int i = 0; i < 9; i++) {
        printf("%c ", array[i]);
    }
    printf("\n");
    #endif

    // Free the allocated memory
    free(array);
    free_run(run);

    return last_state;
}

/* does stage2 of the program taking in the contents of txt file and the last state of stage1*/
void do_stage2(char **contents, char * last_state) {
    printf(SDELIM, 2);

    int size = atoi(contents[0]);
    int elts = atoi(contents[3]);

    int elts184 = (size - 2) / 2;
    int elts232 = (size - 1) / 2; 

    // printing rule and step 
    printf("RULE: %d; STEPS: %d.\n", 184, elts184);
    printf("%s", MDELIM);

     // getting and converting pattern for rule 184
    int int_pattern[8] = {0}; 
    char pattern[8];
    get_pattern(int_pattern, 184); 
    convert_pattern(int_pattern, pattern);

    // Initial conditions
    run_t* run184 = create_empty_run(); 
    run184 = insert_state(run184, last_state, size);

    char *next_stage = get_next_stage(last_state, size, pattern); 

    // appending next state of CA into run
    for (int i = 0; i < elts184; i++) {
        run184 = insert_state(run184, next_stage, size);
        next_stage = get_next_stage(next_stage, size, pattern);
    }
    int start = 0, cell = 0; 
    parse_integers(contents[5], &cell, &start);

    // Traverse and print the linked list for rule 184
    int curr = elts; 
    char * array = (char *)malloc((elts184 + elts232 + elts - start + 1) * sizeof(char));

    char *last184_state = processrun(run184->head, start, cell, array, &curr, size);

    // printing rule and step 
    printf("%s", MDELIM);
    printf("RULE: %d; STEPS: %d.\n", 232, elts232);
    printf("%s", MDELIM);

    // getting and converting pattern for rule 232
    get_pattern(int_pattern, 232); 
    convert_pattern(int_pattern, pattern);

    // Process Rule 232 with a new run
    run_t* run232 = create_empty_run();
    run232 = insert_state(run232, last184_state, size);

    next_stage = get_next_stage(last184_state, size, pattern);

    for (int i = 0; i < elts232; i++) {
        run232 = insert_state(run232, next_stage, size);
        next_stage = get_next_stage(next_stage, size, pattern);
    }

    curr = curr - 1; 
    // Traverse and print the linked list for rule 232
    char *last232_state = processrun(run232->head, start, cell, array, &curr, size);

    // separator 
    printf("%s", MDELIM);

    // printing the no of on and off states from time start at cell number.
    int on = 0, off = 0;
    for (int i = 0; i < curr - start; i++) {
         if (array[i] == '*') {
            on++;
        } else {
            off++;
        }
    }
    printf("#ON=%d #OFF=%d CELL#%d START@%d\n", on, off, cell, start);

    // separator 
    printf("%s", MDELIM);

    // prints the state at t = elts
    printf("%4d: ", elts);
    for (int i = 0; i < size; i ++) {
        printf("%c", last_state[i]);
    }
    printf("\n");

    // character is '>' for more alive cells '<' for more dead cells
    // = for same amount of alive and dead cells
    // checks if last state of CA has more alive or dead cells
    int more_states = 0; 
    char character; 
    for (int i = 0; i < size; i ++) {
        if (last232_state[i] == '*') {
            more_states++;
        } else {
            more_states--; 
        }
    }
    if (more_states == size) {
        character = '>';
    } else if (more_states == -size) {
        character = '<';
    } else {
        character = '=';
    }

    printf("AT T=%d: #ON/#CELLS %c 1/2\n", elts, character);
    
    printf("%s", THEEND);

    #if DEBUG
    // Print the contents of the array
    printf("Contents of the array: ");
    for (int i = 0; i < 17; i++) {
        printf("%c ", array[i]);
    }
    printf("\n");
    #endif

    // Free the allocated memory
    free(array);
    free_run(run184);
    free_run(run232);
    free(last184_state);
    free(last232_state);

}

/* THE END -------------------------------------------------------------------*/