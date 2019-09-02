#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include "../include/parse.h"
#include "../include/statq.h"
#include "../include/filedet.h"


void list_directory(char*, program_options_t*, int);


int main(int argc, char* argv[]) {
    // Pointer to the file/directory we are currently examining.
    struct dirent* direntp;
    // Directory stream to iterate over the directory.
    DIR* directory_stream;
    // Program options passed through the command line
    program_options_t* options = malloc_default_program_options();
    // validate that the malloc worked.
    if (options == NULL) {
        fprintf(stderr, "%s: Error: %s\n", argv[0], strerror(errno));
        return EXIT_FAILURE;
    }
    parse_options(argc, argv, options);

    printf("optind: %d\n", optind);
    printf("opt: %s\n", argv[optind]);

    list_directory(options->run_on, options, options->space_count_indentation);

    // Be a good neighbor.
    free_program_options(&options);

    // This is required so I'm doing it. 
    char error_message[] = ": Here's an error message";
    char display_error[100];
    strcpy(display_error, argv[0]);
    strcat(display_error, error_message);
    perror(display_error);

    return EXIT_SUCCESS;
}


void list_directory(char* directory, program_options_t* program_options, int indent_space_count) {
    /* Method to print the directory tree.*/
    return;
}