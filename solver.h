#ifndef SOLVER_H
#define SOLVER_H


#include <stdbool.h>
#include <stdio.h>

typedef struct Test_Cases {
    char *input;
    char *expected_output;
    bool problem_two;
} Test_Case;

char *solve_task(FILE *input, bool problem_two);

char *get_input_file(bool problem_two);

Test_Case *get_test_cases(size_t *nbr_of_test_cases);

#endif
