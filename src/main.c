#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "../include/parse.h"


int main(int argc, char* argv[]) {
    program_options_t* options = malloc_default_program_options();
    if (options == NULL) {
        fprintf(stderr, "%s: Error: %s\n", argv[0], strerror(errno));
        return EXIT_FAILURE;
    }
    parse_options(argc, argv, options);

    fprintf(stdout, "I: %d\n", options->space_count_indentation);
    fprintf(stdout, "L: %d\n", options->is_follow_links);
    fprintf(stdout, "t: %d\n", options->is_display_filetype);
    fprintf(stdout, "p: %d\n", options->is_display_permissions);

    free(options);

    return EXIT_SUCCESS;
}
