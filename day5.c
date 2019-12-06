#include "solver.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Test_Case *get_test_cases(size_t *nbr_tests) {
    *nbr_tests = 5;
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
    test_cases[4] = (Test_Case)
        {
            .input = "1002,4,3,4,33\n",
            .expected_output = "1002,4,3,4,99",
            .problem_two = false
        };

    return test_cases;
}

char *get_input_file(bool problem_two) {
    return "input5.txt";
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


typedef enum OP_Code {
    ADD=1,
    MULT=2,
    STDIN_TO_PARAM=3,
    PARAM_TO_STDOUT=4,
    JMP_TRUE=5,
    JMP_FALSE=6,
    LESS_THAN=7,
    EQUALS=8,
    HALT=99
} op_code_t;

typedef enum Parameter_Mode {Position=0, Immediate=1} para_mode_t;

typedef struct Paramters {
    size_t size;
    para_mode_t modes[3];
} Parameter;

static op_code_t get_op_code(long instruction) {
    /* last 2 digits of instruction */
    return instruction % 100;
}

static para_mode_t get_parameter(long instruction, size_t argnum) {
    para_mode_t mode;
    switch (argnum) {
    case 1:
        mode = (instruction/100) % 10;
        break;
    case 2:
        mode = (instruction/1000) % 10;
        break;
    case 3:
        mode = (instruction/10000) % 10;
        break;
    }
    /* printf("Instruction: %ld\t mode:%d \t arg:%zu\n", instruction, mode, argnum); */
    return mode;
}

typedef struct ProgramState {
    long *program;
    size_t pc;
    size_t size;
} program_state_t;

static long get_current_instruction(program_state_t *state) {
    return state->program[state->pc];
}

static void run_program(long *program, int nbr_of_numbers) {
    /* op code 1: addition */
    /*     program[result_idx] = l_operand + r_operand; */
    /* op code 2: multiplication */
    /*     program[result_idx] = l_operand * r_operand; */
    /* op code 3: read int from stdin and store to parameter */
    /* op code 4: read value from parameter and put to stdout */
    /* case 99: HALT */

    program_state_t state = {program, 0, nbr_of_numbers};

    while (1) {
        /* read instruction */
        long instruction = get_current_instruction(&state);
        op_code_t op_code = get_op_code(instruction);
        /* printf("read opcode %d\n", op_code); */
        if (op_code == HALT) break;

        /* read the parameter types */
        switch (op_code) {
        case ADD:
        case MULT:
        case LESS_THAN:
        case EQUALS:
            ;
            /* Look at the para_mode_t of 1 argument, if != 0 then IMMEDIATE,       else positional */
            long left = (get_parameter(instruction, 1)) ? state.program[state.pc+1] : state.program[state.program[state.pc+1]];
            long right = (get_parameter(instruction, 2)) ? state.program[state.pc+2] : state.program[state.program[state.pc+2]];
            long res;
            switch (op_code) {
            case ADD:
                res = left + right;
                /* printf("%ld+%ld=%ld\n", left, right, res); */
                break;
            case MULT:
                res = left * right;
                /* printf("%ld*%ld=%ld\n", left, right, res); */
                break;
            case LESS_THAN:
                res = (left < right) ? 1 : 0;
                break;
            case EQUALS:
                res = (left == right) ? 1 : 0;
                break;
            }
            if (get_parameter(instruction, 3)) {
                /* IMMEDIATE, should not happen */
                printf("error writing to immiedate");
            } else {
                /* positional */
                state.program[state.program[state.pc+3]] = res;
                /* printf("res strored in pos %ld\n", state.program[state.pc+3]); */
            }
            state.pc += 4;
            break;
        case STDIN_TO_PARAM:
            ;
            long input;
            printf("provide an int\n");
            if (fscanf(stdin, "%ld", &input) != 1) {
                printf("error parse");
            }
            if (get_parameter(instruction, 1)) {
                /* IMMEDIATE, should not happen */
                printf("error writing to immiedate");
            } else {
                /* POS */
                state.program[state.program[state.pc+1]] = input;
            }
            state.pc+=2;
            break;

        case PARAM_TO_STDOUT:
            ; long to_output;
            if (get_parameter(instruction, 1)) {
                to_output = state.program[state.pc+1];
            } else {
                /* POS */
                to_output = state.program[state.program[state.pc+1]];
            }
            printf(">>>>>  %ld  \n", to_output);
            state.pc+=2;
            break;
        case JMP_FALSE:
        case JMP_TRUE:
            ; long if_arg; long addr;
            if (get_parameter(instruction, 1)) {
                if_arg = state.program[state.pc+1];
            } else {
                if_arg = state.program[state.program[state.pc+1]];
            }
            if (get_parameter(instruction, 2)) {
                addr = state.program[state.pc+2];
            } else {
                addr = state.program[state.program[state.pc+2]];
            }
            if (op_code == JMP_TRUE && if_arg != 0) {
                state.pc = addr;
                /* printf("Since %ld is != 0 I will jump to %ld", if_arg, addr); */
            } else if (op_code == JMP_FALSE && if_arg == 0) {
                state.pc = addr;
                /* printf("Since %ld is == 0 I will jump to %ld", if_arg, addr); */
            } else {
                state.pc += 3;
            }
            break;
        default:
            printf("Got weird op code %d\n", op_code);
            exit(EXIT_FAILURE);
        }
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

    program = get_program(text, nbr_chars, &nbr_of_numbers);
    printf("Read program\n%s\n", text);
    run_program(program, nbr_of_numbers);
    printf("run finished\n");
    output = get_output_str(program, nbr_of_numbers, nbr_chars+1);
    goto done;
        

 done:
    free(text);
    free(program);
    return output;
}

