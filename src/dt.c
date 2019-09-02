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
#define FILE_STAT_ERROR -2


int list_directory(char*, program_options_t*, int);


int is_parent_directory(char*);


int is_current_directory(char*);


int is_followable_link(struct stat*, int);


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
            fprintf(stderr, "%s: Failed to open %s ", argv[0], options->run_on);
            perror("");
        }
        else if (list_result == FILE_STAT_ERROR) {
            fprintf(stderr, "%s: Failed to stat file ", argv[0]);
            perror("");
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
    struct stat file_stat;

    if ((directory_stream = opendir(directory)) == NULL) {
       return -1;
    }

    // Iterate over all of the files in the current directory.
    while ((directory_entry = readdir(directory_stream)) != NULL) {
        // Build the path to the current file. That is, the directory we've
        // iterated to, and /d_name (the name of the current file).
        char path[4096];
        snprintf(path, sizeof(path), "%s/%s", directory, directory_entry->d_name);
        // Attempt to stat the file. 
        if (stat(path, &file_stat) == -1) {
            return FILE_STAT_ERROR;
        }
        // If this is a directory, iterate into it. Only follow links if told to by caller. 
        if (S_ISDIR(file_stat.st_mode) || is_followable_link(&file_stat, program_options->is_follow_links)) {
            // We don't need to iterate over the current directory, or the parent directory. This would
            // cause an infinite loop. Note that this can also happen with sym links.
            if (is_current_directory(directory_entry->d_name) || is_parent_directory(directory_entry->d_name)) {
                continue;
            }
            print_tree_entry(directory_entry, &file_stat, indent_space_count, program_options);
            list_directory(path, program_options, indent_space_count + program_options->space_count_indentation);
        // Else, just print the details. 
        } else {
            print_tree_entry(directory_entry, &file_stat, indent_space_count, program_options);
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


int is_followable_link(struct stat* file_stat, int follow_links) {
    if (!follow_links) {
        return 0;
    }
    return S_ISLNK(file_stat->st_mode);
}