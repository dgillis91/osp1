/* Functions to manipulate file data.
 */

#include "../include/filedet.h"
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <pwd.h>
#include <grp.h>


#define G_SIZE 1000000000
#define M_SIZE 1000000
#define K_SIZE 1000

#define MODE_COUNT 9
mode_t MODES[MODE_COUNT] = {
    S_IRUSR, S_IWUSR, S_IXUSR,
    S_IRGRP, S_IWGRP, S_IXGRP,
    S_IROTH, S_IWOTH, S_IXOTH
};

#define RWX_CHAR_COUNT 3
char RWX_CHAR[RWX_CHAR_COUNT] = {'r', 'w', 'x'};


void print_tree_entry(struct dirent* directory_entry, struct stat* file_stat, int indent, program_options_t* p_options) {
    char permissions[11];
    char link_count[4];
    char username[50];
    char groupname[50];
    char file_size[15];

    // Permissions
    if (p_options->is_display_permissions) {
        file_access_string(file_stat, permissions);
    } else {
        strcpy(permissions, "");
    }

    // Link count
    if (p_options->is_display_link_count) {
        sprintf(link_count, "%d", file_stat->st_nlink);
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

    printf("%*c%s %s %s %s %s %s\n", indent, ' ', directory_entry->d_name, 
           permissions, link_count, username, groupname, file_size);
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
        strcpy(post_fix, "");
        size_scaler = 1;
    }

    size = file_size / (double) size_scaler;

    snprintf(buffer, 15, "%7.2f %s", size, post_fix);
}