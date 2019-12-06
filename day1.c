#include "solver.h"
#include <stdlib.h>
#include <stdio.h>

Test_Case *get_test_cases(size_t *nbr_tests) {
    *nbr_tests = 7;
    Test_Case *test_cases = malloc((*nbr_tests) * sizeof(Test_Case));
    test_cases[0] = (Test_Case)
        {
            .input = "12\n",
            .expected_output = "2",
            .problem_two = false
        };
    test_cases[1] = (Test_Case)
        {
            .input = "14\n",
            .expected_output = "2",
            .problem_two = false
        };
    test_cases[2] = (Test_Case)
        {
            .input = "1969\n",
            .expected_output = "654",
            .problem_two = false
        };
    test_cases[3] = (Test_Case)
        {
            .input = "100756\n",
            .expected_output = "33583",
            .problem_two = false
        };
    test_cases[4] = (Test_Case)
        {
            .input = "14\n",
            .expected_output = "2",
            .problem_two = true
        };
    test_cases[5] = (Test_Case)
        {
            .input = "1969\n",
            .expected_output = "966",
            .problem_two = true
        };
    test_cases[6] = (Test_Case)
        {
            .input = "100756\n",
            .expected_output = "50346",
            .problem_two = true
        };
    return test_cases;
}

char *get_input_file(bool problem_two) {
    return "input1.txt";
}

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
