#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "../include/parse.h"
#include "../include/statq.h"
#include "../include/filedet.h"


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

    // Be a good neighbor.
    free_program_options(&options);

    return EXIT_SUCCESS;
}
