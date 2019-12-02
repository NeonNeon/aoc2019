#include "solver.h"
#include "tests.h"
#include <stdlib.h>
#include <stdio.h>

#define INPUT_ONE_FILE_PATH ("input1.txt")
#define INPUT_TWO_FILE_PATH ("input1.txt")
#define NBR_TEST_CASES (7)

Test_Case test_cases[NBR_TEST_CASES] = {{"12\n", "2", false},
                                        {"14\n", "2", false},
                                        {"1969\n", "654", false},
                                        {"100756\n", "33583", false},
                                        {"14\n", "2", true},
                                        {"1969\n", "966", true},
                                        {"100756\n", "50346", true}};

#include "common_main.c"
static long calculate_fuel(long mass);

char *solve_task(FILE *input, bool problem_two)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    long fuel_required = 0;
    while ((nread = getline(&line, &len, input)) != -1) {
        long mass = atol(line);
        if (!problem_two) {
            fuel_required +=  calculate_fuel(mass);
        } else {
            long fuel;
            while ((fuel = calculate_fuel(mass)) > 0) {
                fuel_required += fuel;
                mass = fuel;
            }
        }
    }
    free(line);
    char *output = calloc(100, sizeof(char));
    sprintf(output, "%ld", fuel_required);
    return output;
}

static long calculate_fuel(long mass) {
    return mass/3 -2;
}
