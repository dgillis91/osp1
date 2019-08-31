#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../include/filedet.h"
#include "../include/statq.h"

int main(int argc, char* argv[]) {
    // This is a pointer to the struct described above. Note that
    // the actual definition of the struct varies by system.
    struct dirent* direntp;
    // This is a directory stream, or an "ordered" sequence of all
    // directories in a directory.
    DIR* dirp;

    stat_queue_t* queue = (stat_queue_t*) malloc(sizeof(stat_queue_t));
    queue->head = queue->tail = NULL;
    
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
    // For each entry, enqueue to a queue.
    stat_t* file_stat;
    while ((direntp = readdir(dirp)) != NULL) {
        printf("%s:%3d\n", direntp->d_name,
               direntp->d_type);
        file_stat = (stat_t*) malloc(sizeof(stat_t));
        stat(direntp->d_name, file_stat);
        enqueue(queue, file_stat, direntp);
    }
    
    stat_queue_node_t* queue_node;
    while (!is_empty(queue)) {
        queue_node = dequeue(queue);
        printf("test\n");
        printf("%d | \n", (int) queue_node->file_stat->st_size);
    }
    
    // Attempt to close the directory multiple times, if
    // interrupted.
    while ((closedir(dirp) == -1) && (errno == EINTR)) ;

    return EXIT_SUCCESS;
}
