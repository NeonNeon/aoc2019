#include "solver.h"
#include <stdlib.h>
#include <stdio.h>

Test_Case *get_test_cases(size_t *nbr_tests) {
    *nbr_tests = 7;
    Test_Case *test_cases = malloc((*nbr_tests) * sizeof(Test_Case));
    test_cases[0] = (Test_Case)
        {
            .input = "111111-111111",
            .expected_output = "1",
            .problem_two = false
        };
    test_cases[1] = (Test_Case)
        {
            .input = "223450-223450",
            .expected_output = "0",
            .problem_two = false
        };
    test_cases[2] = (Test_Case)
        {
            .input = "123789-123789",
            .expected_output = "0",
            .problem_two = false
        };
    test_cases[3] = (Test_Case)
        {
            .input = "112233-112233",
            .expected_output = "1",
            .problem_two = true
        };
    test_cases[4] = (Test_Case)
        {
            .input = "123444-123444",
            .expected_output = "0",
            .problem_two = true
        };
    test_cases[5] = (Test_Case)
        {
            .input = "556777-556777",
            .expected_output = "1",
            .problem_two = true
        };
    test_cases[7] = (Test_Case)
        {
            .input = "111122-111122",
            .expected_output = "1",
            .problem_two = true
        };
    return test_cases;
}


char *get_input_file(bool problem_two) {
    return "input4.txt";
}


static bool valid_number(long nbr, bool problem_two) {
    static char buffer[50];
    sprintf(buffer, "%ld\0", nbr);


    bool return_value = false;
    bool current_same_as_last = false;
    bool prev_prev_different_from_cur = false;
    bool next_different_from_cur = false;
    char prev_char = buffer[0];
    char cur_char;
    for(int i = 1; buffer[i] != '\0'; i++) {
        cur_char = buffer[i];
        if (cur_char < prev_char) return false;

        current_same_as_last = cur_char == prev_char;
        prev_prev_different_from_cur = i < 2 || cur_char != buffer[i-2];
        next_different_from_cur = cur_char != buffer[i+1];
        return_value |= current_same_as_last && (!problem_two ||
                                                 (prev_prev_different_from_cur && next_different_from_cur));
        prev_char = cur_char;
    }
    return return_value;
}

char *solve_task(FILE *input, bool problem_two)
{
    long start, end;
    int matches = fscanf(input, "%ld-%ld", &start, &end);
    if (matches != 2) {
        printf("fscanf failed");
        return NULL;
    }
    int number_of_valid_numbers = 0;
    for (long i = start; i <= end; i++) {
        if (valid_number(i, problem_two)) {
            number_of_valid_numbers++;
            /* printf("%ld was OK\n", i); */
        }
    }

    char *output = calloc(50, sizeof(char));
    sprintf(output, "%d", number_of_valid_numbers);
    return output;
}

