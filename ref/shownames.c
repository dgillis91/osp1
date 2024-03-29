/* shownames.c
 * Taken from UNIX SYSTEMS PROGRAMMING
    - COMMUNICATION, CONCURRENCY, AND THREADS
 * Small program to print a directory.
 *
 * From the man page:
 *      struct dirent {
             ino_t      d_fileno;
             __uint64_t d_seekoff;
             __uint16_t d_reclen;
             __uint16_t d_namlen;
             __uint8_t  d_type;
             char    d_name[1024];
     };

     In addition:
     #define DT_UNKNOWN       0
     #define DT_FIFO          1
     #define DT_CHR           2
     #define DT_DIR           4
     #define DT_BLK           6
     #define DT_REG           8
     #define DT_LNK          10
     #define DT_SOCK         12
     #define DT_WHT          14
*/
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../include/filedet.h"

int main(int argc, char* argv[]) {
    // This is a pointer to the struct described above. Note that
    // the actual definition of the struct varies by system.
    struct dirent* direntp;
    // This is a directory stream, or an "ordered" sequence of all
    // directories in a directory.
    DIR* dirp;

    if (argc != 2) {
       fprintf(stderr, "Usage: %s directory_name\n", argv[0]);
       return EXIT_FAILURE;
    }

    // Call to open the directory and get the stream.
    if ((dirp = opendir(argv[1])) == NULL) {
       perror("Failed to open directory");
       return EXIT_FAILURE;
    }

    // Now we iterate over the directory. From the man page:
    // The readdir() function returns a pointer to the next
    // directory entry. It returns NULL upon reaching the
    // end of the directory or on error.
    // Let's add some stuff to the output to dooo learning.
    while ((direntp = readdir(dirp)) != NULL) {
        printf("%s:%3d\n", direntp->d_name,
               direntp->d_type);
    }
    
    struct stat file_stat;
    if (stat(argv[1], &file_stat) == -1) {
        perror("Unable to stat");
        return EXIT_FAILURE;
    }
    char perms[11];
    file_access_string(&file_stat, perms);
    printf("----\n");
    printf("%s\n", perms);
    
    
    // Attempt to close the directory multiple times, if
    // interrupted.
    while ((closedir(dirp) == -1) && (errno == EINTR)) ;

    return EXIT_SUCCESS;
}
