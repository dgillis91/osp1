/* Functions to manipulate file data.
 */

#include "../include/filedet.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

#define MODE_COUNT 9
mode_t MODES[MODE_COUNT] = {
    S_IRUSR, S_IWUSR, S_IXUSR,
    S_IRGRP, S_IWGRP, S_IXGRP,
    S_IROTH, S_IWOTH, S_IXOTH
};

#define RWX_CHAR_COUNT 3
char RWX_CHAR[RWX_CHAR_COUNT] = {'r', 'w', 'x'};


void print_tree_entry(struct dirent* directory_entry, int indent, program_options_t* p_options) {
    printf("%*c%s\n", indent, ' ', directory_entry->d_name);
}


void file_access_string(struct stat* file_stat, char* buffer) {
    /* Write the unix style file access to `buffer`.
     * Assumes length of `buffer` is 11 - which includes
     * the null terminator.
     */
    const char UNSET_CHAR = '-';
    if (S_ISDIR(file_stat->st_mode)) {
        buffer[0] = 'd';
    } else {
        buffer[0] = UNSET_CHAR;
    }
    int i;
    for (i = 1; i < MODE_COUNT + 1; ++i) {
        if (file_stat->st_mode & MODES[i - 1]) {
            buffer[i] = RWX_CHAR[(i - 1) % RWX_CHAR_COUNT];
        } else {
            buffer[i] = UNSET_CHAR;
        }
    }
    buffer[10] = '\0';
}
