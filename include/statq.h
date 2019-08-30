#ifndef _STATQ_H
#define _STATQ_H

#include <sys/types.h>
#include <sys/stat.h>


typedef struct stat stat_t;

typedef struct stat_q_node {
    struct stat_q_node* next;
    stat_t* file_stat;
} stat_q_node_t;


typedef struct stat_q {
    stat_q_node_t* head;
    stat_q_node_t* tail;
} stat_q_t;


stat_q_node_t* malloc_new_node(stat_t*);


void enqueue(stat_q_t*, stat_t*);


stat_t* peek(stat_q_t*);


stat_t* dequeue(stat_q_t*);


int is_empty(stat_q_t*);

#endif
