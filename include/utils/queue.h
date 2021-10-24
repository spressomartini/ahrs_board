#pragma once
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    char *buffer;
    size_t cap;
    size_t size;
    char *rd, *wr;
    bool op_ok;
} queue_t;

/**
 * Initialize a queue.
 * @param self the queue
 * @param buffer the location of the underlying buffer
 * @param size the size of the underlying buffer, in bytes
 * @return pointer to the initialized queue.
 */
queue_t *queue_init(queue_t *self, char *buf, size_t size);


/**
 * Get the first element of the queue
 * @param self the queue
 * @return first element in the queue
 */
char queue_pop(queue_t *self);

/**
 * Add a new element of a given size to the queue.
 * @param self the queue
 * @param val pointer to the value to push
 */
void queue_push(queue_t *self, char val);

/**
 * View the last element without removal.
 * @param self the queue
 * @return value on top of queue.
 */
char queue_peek(queue_t *self);

/* block read functions */
size_t queue_skip(queue_t *self, size_t num);
char *queue_block_read_addr(queue_t *self);
size_t queue_block_read_len(queue_t *self);
