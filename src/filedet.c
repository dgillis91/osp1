/* Functions to manipulate file data.
 */

#include "../include/filedet.h"
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>


// Constants for # of bytes in a Gig, Meg, and KB. 
#define G_SIZE 1000000000
#define M_SIZE 1000000
#define K_SIZE 1000

// Length of the time string. 
#define C_TIME_LENGTH 26

// Allows file permissions to be created with fewer LOC.
#define MODE_COUNT 9
mode_t MODES[MODE_COUNT] = {
    S_IRUSR, S_IWUSR, S_IXUSR,
    S_IRGRP, S_IWGRP, S_IXGRP,
    S_IROTH, S_IWOTH, S_IXOTH
};

#define RWX_CHAR_COUNT 3
char RWX_CHAR[RWX_CHAR_COUNT] = {'r', 'w', 'x'};


void print_tree_entry(struct dirent* directory_entry, struct stat* file_stat, int indent, program_options_t* p_options) {
    /* Method to print the entry in the directory.
    ** @Params:
    **  - directory_entry: A pointer to the file in the directory to print.
    **  - file_stat: Details on the file.
    **  - indent: Number of spaces (' ') to indent this entry.
    **  - p_options: Struct created when parsing command line args.
    **/
    char permissions[11];
    char link_count[4];
    char username[50];
    char groupname[50];
    char file_size[15];
    char time_string[C_TIME_LENGTH];

    // Permissions
    if (p_options->is_display_permissions) {
        file_access_string(file_stat, permissions);
    } else {
        strcpy(permissions, "");
    }

    // Link count
    if (p_options->is_display_link_count) {
        sprintf(link_count, "%3d", file_stat->st_nlink);
    } else {
        strcpy(link_count, "");
    }

    // User ID
    struct passwd* pwd;
    if (p_options->is_display_uid) {
        pwd = getpwuid(file_stat->st_uid);
        strcpy(username, pwd->pw_name);
    } else {
        strcpy(username, "");
    }
    
    // Group ID
    struct group* grp;
    if (p_options->is_display_gid) {
        grp = getgrgid(file_stat->st_gid);
        strcpy(groupname, grp->gr_name);
    } else {
        strcpy(groupname, "");
    }

    // File size
    if (p_options->is_display_size_in_bytes) {
        pretty_file_size(file_size, (long) file_stat->st_size);
    } else {
        strcpy(file_size, "");
    }

    // Last modified
    if (p_options->is_display_last_modification) {
        strncpy(time_string, ctime(&file_stat->st_mtime), C_TIME_LENGTH - 1);
        // Remove the newline
        time_string[C_TIME_LENGTH - 2] = 0;
    } else {
        strcpy(time_string, "");
    }

    // Build the output
    char detail_string[150];
    snprintf(detail_string, 150, "%s %s %s %s %s %s",
             permissions, link_count, username, groupname,
             file_size, time_string);

    char* pad_string = NULL;
    int filename_length = strlen(directory_entry->d_name);
    int sub = indent;
    if (indent == 0) {
        sub = 1;
    }
    int file_pad_length = 100 - filename_length - indent;
    pad_string = malloc(sizeof(char) * file_pad_length);
    snprintf(pad_string, file_pad_length, "%*c", file_pad_length, ' ');

    //char filename_string[101];
    //snprintf(filename_string, 100, "%s%*c", directory_entry->d_name, file_pad_length, ' ');

    printf("%*c%s %s %s\n", indent, ' ', directory_entry->d_name, pad_string, detail_string);
    free(pad_string);
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


void pretty_file_size(char* buffer, long file_size) {
    char post_fix[2];
    long size_scaler;
    double size;
    // Gig
    if (file_size > G_SIZE) {
        strcpy(post_fix, "G");
        size_scaler = G_SIZE;
    // Megs
    } else if (file_size > M_SIZE) {
        strcpy(post_fix, "M");
        size_scaler = M_SIZE;
    // KB
    } else if (file_size > K_SIZE) {
        strcpy(post_fix, "K");
        size_scaler = K_SIZE;
    } else {
        strcpy(post_fix, " ");
        size_scaler = 1;
    }

    size = file_size / (double) size_scaler;

    snprintf(buffer, 15, "%7.2f %s", size, post_fix);
}