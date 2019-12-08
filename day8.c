#include "solver.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


Test_Case *get_test_cases(size_t *nbr_tests) {
    *nbr_tests = 0;
    Test_Case *test_cases = malloc((*nbr_tests) * sizeof(Test_Case));
    /* test_cases[0] = (Test_Case) */
    /*     { */
    /*         .input = "123456789012\n", */
    /*         .expected_output = "", */
    /*         .problem_two = false */
    /*     }; */
    return test_cases;
}

char *get_input_file(bool problem_two) {
    return "input8.txt";
}

char *solve_task(FILE *input, bool problem_two)
{
    size_t size = 0;
    char *text = NULL;
    getline(&text, &size, input);
    printf("got input %s\n", text);
    size_t i = 0;
    const size_t LAYER_DIM = 25*6;
    size_t min_nbr_zeros = 9999;
    size_t min_layer;
    size_t best_product = 0;
    
    size_t nbr_ones = 0;
    size_t nbr_twos = 0;
    size_t nbr_zeros = 99999;
    size_t current_layer = 0;
    int image[LAYER_DIM];
    for (int i = 0; i < LAYER_DIM; i ++) { image[i] = -1; }
    while(text[i]) {
        if (i % LAYER_DIM == 0) {
            if (nbr_zeros < min_nbr_zeros) {
                min_nbr_zeros = nbr_zeros;
                min_layer = current_layer;
                best_product = nbr_ones * nbr_twos;
                printf("Layer %ld had %ld 0's and %ld 1's prod=%ld\n", current_layer, nbr_zeros, nbr_ones, best_product);
            }
            nbr_ones = 0; nbr_twos = 0; current_layer++;
            nbr_zeros = 0;
        }
        if (image[i % LAYER_DIM] == -1 ||
            image[i % LAYER_DIM] == '2') {
            image[i % LAYER_DIM] = text[i];
        }
        switch (text[i]) {
        case '0': /* black */
            nbr_zeros++;
            break;
        case '1': /* white */
            nbr_ones++;
            break;
        case '2': /* transparent */
            nbr_twos++;
            break;
        }
        i++;
    }
    for (int j = 0; j < 6; j++) {
        for (int i = 0; i < 25; i++) {
            char c = image[j*25 + i];
            char p;
            if (c == '1') p = '#'; 
            else if (c == '2') p = '_';
            else p = ' ';
            printf("%c", p);
        }
        printf("\n");
    }
    
    char *output = calloc(100, sizeof(char));
    sprintf(output, "%ld", best_product);
    return output;
}

