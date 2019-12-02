#ifndef TESTS_H
#define TESTS_H

#include <stdbool.h>

typedef struct Test_Cases {
    char *input;
    char *expected_output;
    bool problem_two;
} Test_Case;

#endif
