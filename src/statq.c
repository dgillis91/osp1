#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/statq.h"


stat_queue_node_t* malloc_new_node(stat_t* filestat, dirent_t* direntp) {
    stat_queue_node_t* new_node = (stat_queue_node_t*) malloc(sizeof(stat_queue_node_t));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->next = NULL;
    new_node->file_stat = filestat;
    new_node->direntp = direntp;
    return new_node;
}


void free_stat_queue_node(stat_queue_node_t* node) {
    /* Assumes node allocated via `malloc`. Note that direntp is not allocated.*/
    free(node->file_stat);
    free(node);
}


void enqueue(stat_queue_t* queue, stat_t* filestat, dirent_t* direntp) {
    /* Enqueue a stat struct. Note that we use a node
     * structure. Programmers should never directly
     * attempt to access queue nodes. Doing so will
     * likely lead to memory leaks.
     */
    stat_queue_node_t* new_node = malloc_new_node(filestat, direntp);
    // Empty - enqueue at head.
    if (queue->tail == NULL) {
        queue->head = queue->tail = new_node;
        return;
    }
    queue->tail->next = new_node;
    queue->tail = new_node;
}


stat_queue_node_t* peek(stat_queue_t* queue) {
    return queue->head;
}


stat_queue_node_t* dequeue(stat_queue_t* queue) {
    // If empty
    if (queue->tail == NULL) {
        return NULL;
    }
    // Get the head node so we can return it
    stat_queue_node_t* node = queue->head;
    // Move the head pointer to the next node.
    queue->head = node->next;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    return node;
}


int is_empty(stat_queue_t* queue) {
    return (queue->head == NULL);
}
