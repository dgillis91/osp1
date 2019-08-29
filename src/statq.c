#include "../include/statq.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>


int malloc_new_node(stat_t* filestat) {
    stat_q_t* new_node = (stat_q_t*) malloc(sizeof(stat_q_t));
    if (new_node == NULL) {
        return -1;
    }
    new_node->next = NULL;
    new_node->file_stat = file_stat;
    return 0;
}


void enqueue(stat_q_t* queue, stat_t* filestat) {
    /* Enqueue a stat struct. Note that we use a node
     * structure. Programmers should never directly
     * attempt to access queue nodes. Doing so will
     * likely lead to memory leaks.
     */
    stat_q_node_t* new_node malloc_new_node(filestat);
    // Empty - enqueue at head.
    if (queue->head == NULL) {
        queue->head = queue->tail = new_node;
        return;
    }
    new_node->next = queue->head;
    queue->head = new_node;
}
