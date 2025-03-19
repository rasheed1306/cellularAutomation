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

  Signed by: [Enter your full name and student number here before submission]
  Dated:     [Enter the date that you "signed" the declaration]
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

/* #DEFINE'S -----------------------------------------------------------------*/
#define SDELIM "==STAGE %d============================\n"   // stage delimiter
#define MDELIM "-------------------------------------\n"    // delimiter of -'s
#define THEEND "==THE END============================\n"    // end message

#define CRTRNC '\r'     // carriage return character
#define NBRHDS 8        // number of possible neighborhoods

/* TYPE DEFINITIONS ----------------------------------------------------------*/
typedef char cells_t;            // base type to store states of cells
typedef struct state state_t;           // a cellular automaton state
typedef unsigned char rule_t[NBRHDS];   // an elementary CA update rule function

struct state {                   // a state in a CA is defined by
    cells_t*        clls;        // ... an array of cells and
    state_t*        next;        // ... a link to the next state
};

typedef struct {                 // a run of a CA consists of
    state_t*        init;        // ... the initial state and
    state_t*        curr;        // ... the current state,
} run_t;                         // implemented as a linked list of states


typedef struct {                 // an elementary CA is defined by
    unsigned char   code;        // ... a code of the update rule,
    unsigned int    size;        // ... a number of cells,
    unsigned int    elts;        // ... number of elements to store cells,
    unsigned int    time;        // ... the current time step,
    rule_t          rule;        // ... an update rule function, and
    run_t*          run;         // ... a run of state steps
} CA_t;

/* USEFUL FUNCTIONS ----------------------------------------------------------*/
int             mygetchar(void);                // getchar() that skips
                                                //    carriage returns

/* WHERE IT ALL HAPPENS ------------------------------------------------------*/
int main(int argc, char *argv[]) {
    // Message from Artem: The proposed in this skeleton file #define's,
    // typedef's, and struct's are the subsets of those from my sample solution
    // to this assignment. You can decide to use them in your program, or if
    // you find them confusing, you can remove them and implement your solution
    // from scratch. I will share my sample solution with you at the end of
    // the subject.
    return EXIT_SUCCESS;        // algorithms are fun!!!
}

/* USEFUL FUNCTIONS ----------------------------------------------------------*/

// An improved version of getchar(); skips carriage return characters.
// NB: Adapted version of the mygetchar() function by Alistair Moffat
int mygetchar() {
    int c;
    while ((c=getchar())==CRTRNC);          // skip carriage return characters
    return c;
}

/* THE END -------------------------------------------------------------------*/