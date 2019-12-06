#include "solver.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Test_Case *get_test_cases(size_t *nbr_tests) {
    *nbr_tests = 4;
    Test_Case *test_cases = malloc((*nbr_tests) * sizeof(Test_Case));
    test_cases[0] = (Test_Case)
        {
            .input = "1,0,0,0,99\n",
            .expected_output = "2,0,0,0,99",
            .problem_two = false
        };
    test_cases[1] = (Test_Case)
        {
            .input = "2,3,0,3,99\n",
            .expected_output = "2,3,0,6,99",
            .problem_two = false
        };
    test_cases[2] = (Test_Case)
        {
            .input = "2,4,4,5,99,0\n",
            .expected_output = "2,4,4,5,99,9801",
            .problem_two = false
        };
    test_cases[3] = (Test_Case)
        {
            .input = "1,1,1,4,99,5,6,0,99\n",
            .expected_output = "30,1,1,4,2,5,6,0,99",
            .problem_two = false
        };
    return test_cases;
}

char *get_input_file(bool problem_two) {
    return "input2a.txt";
}


static bool execute_code(int op_code, long l_operand, long r_operand,
                    int result_idx, long* program, int nbr_of_numbers) {
    if(result_idx > nbr_of_numbers) {
        printf("result_idx out of bounds, error");
        exit(1);
    }
    switch (op_code) {
    case 1:
        program[result_idx] = l_operand + r_operand;
        break;
    case 2:
        program[result_idx] = l_operand * r_operand;
        break;
    case 99:
        return true;
        break;
    default:
        printf("wrong op_code, error");
        exit(1);
    }
    return false;
}

static char* get_program_input(FILE *stream)
{
    char *line = NULL;
    size_t len = 0;
    if ((getline(&line, &len, stream)) == -1) {
        free(line); return NULL;
    }
    return line;
}

static long *get_program(char *text, int nbr_chars, int *nbr_of_numbers) {
    long *program=malloc(nbr_chars*sizeof(long));
    // fyll på lista med tal
    int index=0;
    for(int i=0; i < nbr_chars;) {
        if (text[i] == '\n') {
            //printf("Found \\n\n");
            break;
        }
        long nbr = atol(text+i);
        program[index++] = nbr;
        //printf("Found %ld\n", nbr); 
        while(i < nbr_chars && text[i++]!=',');
    }
    *nbr_of_numbers = index;
    return program;
}

static void run_program(long *program, int nbr_of_numbers) {
    for (int i=0; i + 3 < nbr_of_numbers; i+=4) {
        int op_code = program[i];
        int left_operand_idx = program[i+1];
        int right_operand_idx = program[i+2];
        if (left_operand_idx > nbr_of_numbers ||
            right_operand_idx > nbr_of_numbers) {
            printf("error out of bounds");
            exit(1);
        }
        int result_idx = program[i+3];
        bool finished = execute_code(op_code, program[left_operand_idx], program[right_operand_idx],
                                     result_idx, program, nbr_of_numbers);
        if (finished) break;
    }
}

static char *get_output_str(long *program, int nbr_of_numbers, int size_of_output_str)
{
    int length_used_of_output_str = 0;
    char *output = calloc(size_of_output_str, sizeof(char));
    for (int i=0; i < nbr_of_numbers ; i++) {
#define BUF_SIZE (100)
        char buf[BUF_SIZE];
        for(int j=0; j < BUF_SIZE; j++) {
            buf[j] = '\0';
        }
        char *format_str = (i == nbr_of_numbers - 1) ? "%ld" : "%ld,"; //no comma for last
        sprintf(buf, format_str, program[i]);
        int nbr_chars_to_add_to_output = strlen(buf);
        
        if (length_used_of_output_str + nbr_chars_to_add_to_output > size_of_output_str) {
            //printf("has to reallocate");
            realloc(output, 2*size_of_output_str);
            size_of_output_str = 2 * size_of_output_str;
        }
        strcat(output, buf);
        length_used_of_output_str += nbr_chars_to_add_to_output;
        output[length_used_of_output_str] = '\0';
    }
    return output;
}

char *solve_task(FILE *input, bool problem_two)
{
    char *output;
    char *text = get_program_input(input);
    int nbr_chars = strlen(text);

    int nbr_of_numbers;
    long *program;
    // problem 1
    if (!problem_two) {
        program = get_program(text, nbr_chars, &nbr_of_numbers);
        run_program(program, nbr_of_numbers);
        output = get_output_str(program, nbr_of_numbers, nbr_chars+1);
        goto done;
    } 

    // problem 2
    long noun, verb;
    for (noun = 0; noun < 100; noun++) {
        for (verb = 0; verb < 100; verb++) {
            program = get_program(text, nbr_chars, &nbr_of_numbers);
            program[1] = noun;
            program[2] = verb;
            run_program(program, nbr_of_numbers);
            if(program[0] == 19690720) {
                long res = 100*noun + verb;
                output = calloc(40, sizeof(char));
                sprintf(output, "%ld", res);
                goto done;
            }
            free(program);
        }
    }

 done:
    free(text);
    free(program);
    return output;
}

