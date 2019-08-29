/* Functions for manipulating file details.
 */

#ifndef _FILEDET_H
#define _FILEDET_H

#include <sys/stat.h>
#include <sys/types.h>

void file_access_string(struct stat* file_stat, char* buffer);

#endif
