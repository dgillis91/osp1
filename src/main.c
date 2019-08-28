#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../include/parse.h"


int main(int argc, char* argv[]) {
    program_options_t* options = malloc_default_program_options();
    parse_options(argc, argv, options);

    fprintf(stdout, "I: %d\n", options->space_count_indentation);

    return EXIT_SUCCESS;
}
