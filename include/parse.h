#ifndef _PARSE_H
#define _PARSE_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


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


program_options_t* malloc_default_program_options();


void parse_options(int, char**, program_options_t*);


void print_help_and_terminate();


#endif
