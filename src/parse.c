#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../include/parse.h"


void print_help_and_terminate(char* argv[]) {
    printf("Usage: %s [-h] [-I n] [-L -d -g -i -p -s -t -u | -l] [dirname]\n"
           "For: \n"
           "    -h: Display help and exit.\n"
           "    -I: Number of spaces to use for indent.\n"
           "    -L: Follow sym links. Default false.\n"
           "    -d: Show the time of last modification.\n", argv[0]);
    printf("    -g: Print the GID for the file.\n"
           "    -i: Print the number of links in the inode table.\n"
           "    -p: Print permission bits.\n"
           "    -s: Print the size in bytes.\n"
           "    -t: Print information on file type.\n");
    printf("    -u: Print the UID of the file.\n"
           "    -l: Same as specifying -tpiugs.\n");
    exit(EXIT_SUCCESS);
}


program_options_t* malloc_default_program_options() {
    /* Allocate a struct with the default command line arguments.
     * Note that allocating INSIDE of a function is not necessarily
     * good form. There is always the possibility that the caller
     * doesn't realize malloc is called in the function, and a memory
     * leak follows. That said, I find it easier to read and follow.
     * So, the function has been named `malloc_...` for added verbosity.
     * @Returns: A dynamically allocated struct with the program options.
     *           Must be `free`d by the caller. 
    */
    program_options_t* program_opts = (program_options_t*) malloc(sizeof(program_options_t));
    // Recall that malloc return null for failure and sets errno to ENOMEM.
    if (program_opts == NULL) {
        return NULL;
    }
    program_opts->is_print_help = 0;
    program_opts->space_count_indentation = 4;
    program_opts->is_follow_links = 0;
    program_opts->is_display_filetype = 0;
    program_opts->is_display_permissions = 0;
    program_opts->is_display_link_count = 0;
    program_opts->is_display_uid = 0;
    program_opts->is_display_gid = 0;
    program_opts->is_display_size_in_bytes = 0;
    program_opts->is_display_last_modification = 0;
    program_opts->is_display_all = 0;
    program_opts->run_on = NULL;
    return program_opts;
}

void free_program_options(program_options_t** program_opts) {
    /* Because the options are going to be dynamically allocated,
    ** and they have a dynamically allocated 'string', we create
    ** a method to clean up the allocated memory. 
    ** @Param:
    **  - program_opts: pointer to a program_options structure.
    */
    free((*program_opts)->run_on);
    free(*program_opts);
    *program_opts = NULL;
}


void parse_options(int argc, char* argv[], program_options_t* program_opts) {
    /* Parse the command line options and modify the passed in `program_opts`
     * with them. 
     * @Params:
     * * Note argc and argv come from the main method.
     * - argc: Count of arguments from the program.
     * - argv: Vector of arguments.
     * - program_opts: Pointer to a program_options_t. Is modified.
     * @Reference: 
     * - https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
     * - http://man7.org/linux/man-pages/man3/getopt.3.html
    */
    // Set this so that `getopt` doesn't write error messages.
    opterr = 0;
    int current_option;
    // As per the man page, calling `getopt` parses the arguments from argv,
    // as specified in the third param, `optstring`. When there are no option
    // characters left, `getopt` returns -1.
    while ((current_option = getopt(argc, argv, "hI:Ltpiugsdl")) != -1) {
       switch (current_option) {
        case 'h':
            print_help_and_terminate(argv);
            break;
        case 'I':
            program_opts->space_count_indentation = atoi(optarg);
            break;
        case 'L':
            program_opts->is_follow_links = 1;
            break;
        case 't':
            program_opts->is_display_filetype = 1;
            break;
        case 'p':
            program_opts->is_display_permissions = 1;
            break;
        case 'i':
            program_opts->is_display_link_count = 1;
            break;
        case 'u':
            program_opts->is_display_uid = 1;
            break;
        case 'g':
            program_opts->is_display_gid = 1;
            break;
        case 's':
            program_opts->is_display_size_in_bytes = 1;
            break;
        case 'd':
            program_opts->is_display_last_modification = 1;
            break;
        case 'l':
            program_opts->is_display_all = 1;
            program_opts->is_display_filetype = 1;
            program_opts->is_display_permissions = 1;
            program_opts->is_display_link_count = 1;
            program_opts->is_display_uid = 1;
            program_opts->is_display_gid = 1;
            program_opts->is_display_size_in_bytes = 1;
            break;
        // From the man page:
        // By default, getopt() prints an error message on standard error,
        // places the erroneous option character in optopt, and returns '?'
        // as the function result.
        // Note that `getopt` sets a global `optopt` with the option character.
        case '?':
            // If the error is for not having an arg with `I` . . . 
            if (optopt == 'I') {
                fprintf(stderr, "%s: Error: Argument `I` requires an integer parameter\n", argv[0]);
            // Unknown opt
            } else {
                fprintf(stderr, "%s: Error: Unkown option character -%c\n", argv[0], optopt);
            }
            // In either case, terminate.
            print_help_and_terminate(argv);
            break;
       }
    }

    // Parse the path out. 
    // We also need to allocate the memory here. Remember to copy the
    // data into your char*. Otherwise you are assigning the memory location
    // of argv[optind] and you will spend five hours being a fool. 
    int path_length = 0;
    if (argv[optind] != NULL) {
        path_length = strlen(argv[optind]);
        program_opts->run_on = (char*) malloc(sizeof(char) * path_length);
        strcpy(program_opts->run_on, argv[optind]);
    } else {
        path_length = strlen(".");
        program_opts->run_on = (char*) malloc(sizeof(char) * path_length);
        strcpy(program_opts->run_on, ".");
    }
}

