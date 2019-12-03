#include "solver.h"
#include "tests.h"
#include <stdlib.h>
#include <stdio.h>

#define INPUT_ONE_FILE_PATH ("input3.txt")
#define INPUT_TWO_FILE_PATH ("input3.txt")
#define NBR_TEST_CASES (3)

Test_Case test_cases[NBR_TEST_CASES] =
    {{"R8,U5,L5,D3\n" /* line 1 */
         "U7,R6,D4,L4\n" /* line 2 */,
         "6" /* distance */,
         false},
     {"R75,D30,R83,U83,L12,D49,R71,U7,L72\n" /* Line 1 */
      "U62,R66,U55,R34,D71,R55,D58,R83\n" /* Line 2 */,
      "159" /* distance */,
      false},
     {"R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51\n" /* Line 1 */
      "U98,R91,D20,R16,D67,R40,U7,R15,U6,R7\n", /* Line 2 */
      "135" /* distance */,
      false}};

#include "common_main.c"

typedef struct Tripple {
    int x;
    int y;
    int nbr_steps;
} Coordinate;

typedef struct Walkers {
    Coordinate current;
    Coordinate *visited_coordinates;
    size_t nbr_visited_coordinates;
    size_t MAX_SIZE;
} Walker;

#define MAX_BUFFER_SIZE (1000000)

static bool read_input_lines(char **first, size_t *flen,
                             char **second, size_t *slen,
                             FILE *stream)
{
    /* assumes only 2 input lines */
    ssize_t nread;
    nread = getline(first, flen, stream);
    if (nread == -1) {
        return false;
    }
    nread = getline(second, slen, stream);
    if (nread == -1) {
        return false;
    }
    return true;
}

static bool add_coordinates(Walker *walker,
                            const char direction, size_t steps) {
    while (steps--) {
        walker->current.nbr_steps++;
        switch (direction) {
        case 'R':
            walker->current.x += 1;
            break;
        case 'L':
            walker->current.x -= 1;
            break;
        case 'U':
            walker->current.y += 1;
            break;
        case 'D':
            walker->current.y -= 1;
            break;
        default:
            return false;
            printf("ERROR: add_coordinate got weird direction");
        }
        if (walker->nbr_visited_coordinates + 1 > walker->MAX_SIZE) {
            printf("ERROR: add_coordinate out of memory!");
            return false;
        }
        bool already_visited = false;
        int i = 0;
        while(i < walker->nbr_visited_coordinates) {
            if (walker->current.x == walker->visited_coordinates[i].x &&
                walker->current.y == walker->visited_coordinates[i].y) {
                already_visited = true;
            }
            i++;
        }
        if (!already_visited) {
            walker->visited_coordinates[walker->nbr_visited_coordinates++] =
                walker->current;
        }
        printf("Added coord %d, %d\n", walker->current.x, walker->current.y);
    }
    return true;
}
int total_steps(Coordinate left, Coordinate right) {
    return left.nbr_steps + right.nbr_steps;
}

int manhattan(Coordinate left, Coordinate right) {
    int distance =  abs(left.x - right.x) + abs(left.y - right.y);
    printf("Distance between (%d, %d) and (%d, %d) is \n%d\n", left.x, left.y, right.x, right.y, distance);
    return distance;
}

static Coordinate find_closest(Walker *left, Walker *right, boolean problem_two) {
    Coordinate closest = {0, 0, 0};
    int min_distance = 999999999;
    Coordinate l_iter;
    Coordinate r_iter;
    int new_distance;
    for (int i = 0; i< left->nbr_visited_coordinates; i++ ) {
        l_iter = left->visited_coordinates[i];
        if(!problem_two) {
            new_distance = manhattan(l_iter, (Coordinate) {0,0,0});
            if (new_distance >= min_distance) {
                continue;
            }
        }
        for (int j = 0; j< right->nbr_visited_coordinates; j++ ) {
            r_iter = right->visited_coordinates[j];
            if (l_iter.x == r_iter.x && l_iter.y == r_iter.y) {
                printf("They share (%d, %d)\n", l_iter.x, l_iter.y);
                if (problem_two) {
                    new_distance = total_steps(l_iter, r_iter);
                    if (new_distance < min_distance) {
                        min_distance = new_distance;
                        closest = r_iter;
                    }
                } else {
                    /* Problem 1 */
                    min_distance = new_distance;
                    closest = r_iter;
                }
                break; /* inner loop */
            }
        }
    }
    printf("MINIMUM STEPS: %d", min_distance);
    return closest;
}

char *solve_task(FILE *input, bool problem_two)
{
    char *first_line = NULL;
    size_t first_length = 0;
    char *second_line = NULL;
    size_t second_length = 0;
    bool rc;
    rc = read_input_lines(&first_line, &first_length,
                          &second_line, &second_length,
                          input);
    if (!rc) {
        free(first_line);
        free(second_line);
        printf("Couldnt read from input file!");
        return NULL;
    }

    printf("Line1 = %s", first_line);
    printf("Line2 = %s", second_line);

    Coordinate *first_line_coordinates = calloc(MAX_BUFFER_SIZE,
                                                sizeof(Coordinate));

    if (!first_line_coordinates) {
        free(first_line);
        free(second_line);
        perror("error calloc");
        return NULL;
    }

    
    Walker first_walker = {
        {0,0},                  /* start at origo */
        first_line_coordinates, /* visited coordinates */
        0,                      /* current size */
        MAX_BUFFER_SIZE            /* max size of list */
    };
    first_line_coordinates = NULL; /* drop this referennce */

    /* Parse first line and populate coordinates */

    size_t steps;
    char direction;
    char *iterator = first_line;
    while(iterator != NULL) {
        sscanf(iterator, "%c%zu", &direction, &steps);
        add_coordinates(&first_walker, direction, steps);
        iterator = strchr(iterator, ',');
        if (!iterator) break;
        /* iterator now points to ',' */
        /* therefore we increase it by 1 to move past ',' */
        iterator++;
        printf("Read direction %c and steps %zu\n", direction, steps);
    }

    Coordinate *second_line_coordinates = calloc(MAX_BUFFER_SIZE,
                                                 sizeof(Coordinate));
    Walker second_walker = {
        {0,0},                  /* start at origo */
        second_line_coordinates, /* visited coordinates */
        0,                      /* current size */
        MAX_BUFFER_SIZE            /* max size of list */
    };
    second_line_coordinates = NULL; /* drop this referennce */
    
    iterator = second_line;
    while(iterator != NULL) {
        int matches = sscanf(iterator, "%c%zu", &direction, &steps);
        if (matches != 2) {
            printf("ERROR WTF matches %d\n", matches);
        }
        add_coordinates(&second_walker, direction, steps);
        iterator = strchr(iterator, ','); /* last one doesnt have the comma! */
        /* iterator now points to ',' */
        if (iterator == NULL) break;
        /* therefore we increase it by 1 to move past ',' */
        iterator++;
        printf("Read direction %c and steps %zu\n", direction, steps);
    }

    Coordinate closest = find_closest(&first_walker, &second_walker, problem_two);
    int distance = manhattan(closest, (Coordinate) {0,0});
    printf("FINAL DISTANCE = %d\n", distance);
    char *output = calloc(50, sizeof(char));
    sprintf(output, "%d", distance);

    
    printf("free1");
    free(first_walker.visited_coordinates);
    free(second_walker.visited_coordinates);
    printf("free2");
    free(first_line);
    printf("free3");
    free(second_line);
    return output;
}
