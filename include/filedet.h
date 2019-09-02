/* Functions for manipulating file details.
 */

#ifndef _FILEDET_H
#define _FILEDET_H

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <parse.h>

void print_tree_entry(struct dirent*, int, program_options_t*);


void file_access_string(struct stat* file_stat, char* buffer);

#endif
