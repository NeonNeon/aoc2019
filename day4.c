#include "solver.h"
#include "tests.h"
#include <stdlib.h>
#include <stdio.h>

#define INPUT_ONE_FILE_PATH ("input4.txt")
#define INPUT_TWO_FILE_PATH ("input4.txt")
#define NBR_TEST_CASES (7)

Test_Case test_cases[NBR_TEST_CASES] = {
    {"111111-111111", "1", false},
    {"223450-223450", "0", false},
    {"123789-123789", "0", false},
    {"112233-112233", "1", true},
    {"123444-123444", "0", true},
    {"556777-556777", "1", true},
    {"111122-111122", "1", true}
};

#include "common_main.c"

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

