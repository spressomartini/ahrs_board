/*
 * queue from PyroAVR, plus a few additional functionality :)
 */
#include "utils/queue.h"

queue_t *queue_init(queue_t *self, char *buf, size_t size) {
    self->buffer = buf;
    self->cap = size;
    self->size = 0;
    self->rd = self->buffer;
    self->wr = self->buffer;
    self->op_ok = true;
    return self;
}

char queue_pop(queue_t *self) {
    char val = 0;
    if(self->size > 0){
        self->op_ok = true;
        self->size--;
        val = *self->rd;
        if(self->rd - self->buffer == self->cap) {
            self->rd = self->buffer;
        }
        else {
            self->rd++;
        }
    }
    else {
        self->op_ok = false;
    }
    return val;
}

void queue_push(queue_t *self, char val) {
    if(self->size < self->cap) {
        *self->wr = val;
        self->size++;
        if(self->wr == self->buffer + self->cap) {
            self->wr = self->buffer;
        }
        else {
            self->wr++;
        }
        self->op_ok = true;
    }
    else {
        self->op_ok = false;
    }
}

char queue_peek(queue_t *self) {
    char val = 0;
    if(self->size > 0) {
        self->op_ok = true;
        val = *self->rd;
    }
    else {
        self->op_ok = false;
    }
    return val;
}

size_t queue_skip(queue_t *self, size_t num) {
    // why did you call this function?
    if (num == 0) {
        return 0;
    }
    
    num = self->size < num ? self->size : num;

    self->rd += num;
    
    if(self->rd - self->buffer >= self->cap) {
        self->rd -= self->cap;
    }

    self->size -= num;

    return num;
}

char *queue_block_read_addr(queue_t *self) {
    return self->rd;
}

size_t queue_block_read_len(queue_t *self) {
    size_t rd_len;
    char *wr = self->wr, *rd = self->rd, *buf = self->buffer;

    if (wr > rd) {
        rd_len = wr - rd;
    }
    else if (wr < rd) {
        rd_len = self->cap - (rd - buf);
    }
    else {
        rd_len = 0;
    }

    return rd_len;
}
