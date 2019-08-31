#ifndef _STATQ_H
#define _STATQ_H

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>


typedef struct stat stat_t;
typedef struct dirent dirent_t;

typedef struct stat_queue_node {
    struct stat_queue_node* next;
    stat_t* file_stat;
    dirent_t* direntp;
} stat_queue_node_t;


typedef struct stat_queue {
    stat_queue_node_t* head;
    stat_queue_node_t* tail;
} stat_queue_t;


stat_queue_node_t* malloc_new_node(stat_t*, dirent_t*);


void free_stat_queue_node(stat_queue_node_t*);


void enqueue(stat_queue_t*, stat_t*, dirent_t*);


stat_queue_node_t* peek(stat_queue_t*);


stat_queue_node_t* dequeue(stat_queue_t*);


int is_empty(stat_queue_t*);

#endif
