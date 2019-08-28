#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


void print_help_and_terminate() {
    fprintf(stdout, "This is a help message\n");
    exit(EXIT_SUCCESS);
}


typedef struct program_options {
    int is_print_help;
    int space_count_indentation;
    int is_follow_links;
    int is_display_filetype;
    int is_display_permissions;
    int is_display_link_count;
    int is_display_uid;
    int is_display_gid;
    int is_display_size_in_bytes; // Not clear on this requirement
    int is_display_last_modification;
    int is_display_all;
} program_options_t;


program_options_t* malloc_default_program_options() {
    program_options_t* program_opts = (program_options_t*) malloc(sizeof(program_options_t));
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
    return program_opts;
}


void parse_options(int argc, char* argv[], program_options_t* program_opts) {
    // Returns -1 for stop, -2 for error. Will write via perror, etc.
    int current_option;
    while ((current_option = getopt(argc, argv, "hI:Ltpiugsdl")) != -1) {
       switch (current_option) {
        case 'h':
            print_help_and_terminate();
            break;
        case 'I':
            program_opts->space_count_indentation = atoi(optarg);
            break;
       }
    }
}


int main(int argc, char* argv[]) {
    program_options_t* options = malloc_default_program_options();
    parse_options(argc, argv, options);

    fprintf(stdout, "I: %d\n", options->space_count_indentation);

    return EXIT_SUCCESS;
}
