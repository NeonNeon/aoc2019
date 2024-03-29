#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "solver.h"

static void print_help_text_and_exit(char **argv);
static void run_tests(void);

int main(int argc, char **argv)
{
    FILE *stream;
    if (argc != 2) {
        print_help_text_and_exit(argv);
    }

    char *arg = argv[1];

    if (strcmp(arg, "test") == 0) {
        run_tests();
        exit(EXIT_SUCCESS);
    } 
    
    int nbr = atoi(argv[1]);
    bool run_second_problem;
    if (nbr == 1) {
        run_second_problem = false;
    } else if (nbr == 2) {
        run_second_problem = true;
    } else {
        print_help_text_and_exit(argv);
    }
    stream = fopen(get_input_file(run_second_problem), "r");
    
    if (stream == NULL) {
        perror("common_main failed to open file");
        exit(EXIT_FAILURE);
    }

    char *output = solve_task(stream, run_second_problem);
    if (output != NULL) {
        printf("Output from solver was: %s\n", output);
    }
    free(output);
    fclose(stream);
    exit(EXIT_SUCCESS);
}

void print_help_text_and_exit(char **argv)
{
    fprintf(stderr, "Usage: %s [1|2|test]\n", argv[0]);
    exit(EXIT_FAILURE);
}

void run_tests(void)
{
    size_t nbr_of_test_cases = 0;
    Test_Case *test_cases = get_test_cases(&nbr_of_test_cases);
    if (!test_cases || nbr_of_test_cases == 0) {
        printf("No test cases avaliable!\n");
    }
    for(int i = 0; i < nbr_of_test_cases; i++) {
        Test_Case t = test_cases[i];
        FILE *input_stream = tmpfile();
        fwrite(t.input, sizeof(char), strlen(t.input), input_stream);
        rewind(input_stream);
        char *output = solve_task(input_stream, t.problem_two);
        if (output==NULL) {
            output = "NULLPTR";
        }
        if (strcmp(output, t.expected_output) == 0) {
            printf("Test %d of problem %d successful!\n", i+1, (t.problem_two) ? 2 : 1);
        } else {
            printf("=========== Test failed! ===========\n");
            printf("Test case %d\nInput:\n%s\nExpected output:\n%s\nActual output:\n%s\n", i+1, t.input, t.expected_output, output);
        }
        if (output != NULL) {
            free(output);
        }
        fclose(input_stream);
    }
    free(test_cases);
}
