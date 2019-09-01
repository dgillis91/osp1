#ifndef _PARSE_H
#define _PARSE_H

// As it turns out, PATH_MAX is complex across systems. So,
// we are just going to make an assumption. 
#define PROGRAM_OPTIONS_MAX_PATH_LENGTH 4096

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
    char* run_on; 
} program_options_t;


program_options_t* malloc_default_program_options();


void free_program_options(program_options_t**);


void parse_options(int, char**, program_options_t*);


void print_help_and_terminate(char**);


#endif
