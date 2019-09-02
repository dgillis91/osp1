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


#define DIRECTORY_OPEN_ERROR -1


int list_directory(char*, program_options_t*, int);


int is_parent_directory(char*);


int is_current_directory(char*);


int main(int argc, char* argv[]) {
    // Program options passed through the command line
    program_options_t* options = malloc_default_program_options();
    // validate that the malloc worked.
    if (options == NULL) {
        fprintf(stderr, "%s: Error: %s\n", argv[0], strerror(errno));
        return EXIT_FAILURE;
    }
    parse_options(argc, argv, options);

    // Doing a nested if so I can add more excceptionary criteria with little repeat code.
    int list_result;
    if ((list_result = list_directory(options->run_on, options, 0)) != 0) {
        free_program_options(&options);
        if (list_result == DIRECTORY_OPEN_ERROR) {
            fprintf(stderr, "%s: Failed to open %s\n", argv[0], options->run_on);
        }
        return EXIT_FAILURE;
    }

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


int list_directory(char* directory, program_options_t* program_options, int indent_space_count) {
    /* Method to print the directory tree.*/
    DIR* directory_stream;
    struct dirent* directory_entry;

    if ((directory_stream = opendir(directory)) == NULL) {
       return -1;
    }

    while ((directory_entry = readdir(directory_stream)) != NULL) {
        if (directory_entry->d_type == DT_DIR) {
            char path[4096];
            // We don't need to iterate over the current directory, or the parent directory. This would
            // cause an infinite loop. Note that this can also happen with sym links.
            if (strcmp(directory_entry->d_name, "..") == 0 || strcmp(directory_entry->d_name, ".") == 0) {
                continue;
            }
            snprintf(path, sizeof(path), "%s/%s", directory, directory_entry->d_name);
            printf("%*c%s\n", indent_space_count, ' ', directory_entry->d_name);
            list_directory(path, program_options, indent_space_count + program_options->space_count_indentation);
        } else {
            printf("%*c%s\n", indent_space_count, ' ', directory_entry->d_name);
        }
    }
    closedir(directory_stream);

    return 0;
}

int is_current_directory(char* directory) {
    return (strcmp(directory, ".") == 0);
}

int is_parent_directory(char* directory) {
    return (strcmp(directory, "..") == 0);
}