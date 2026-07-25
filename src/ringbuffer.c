#include "ringbuffer.h"

#include <stdlib.h>
#include <string.h>

ringbuffer_t *rb_create(size_t capacity) {
    if (capacity == 0) {
        return NULL;
    }
    ringbuffer_t *rb = malloc(sizeof(*rb));
    if (rb == NULL) {
        return NULL;
    }
    rb->data = malloc(capacity);
    if (rb->data == NULL) {
        free(rb);
        return NULL;
    }
    rb->capacity = capacity;
    rb->head = 0;
    rb->tail = 0;
    rb->size = 0;
    return rb;
}

void rb_destroy(ringbuffer_t *rb) {
    if (rb == NULL) {
        return;
    }
    free(rb->data);
    free(rb);
}

size_t rb_size(const ringbuffer_t *rb) {
    return rb->size;
}

size_t rb_capacity(const ringbuffer_t *rb) {
    return rb->capacity;
}

bool rb_is_empty(const ringbuffer_t *rb) {
    return rb->size == 0;
}

bool rb_is_full(const ringbuffer_t *rb) {
    return rb->size == rb->capacity;
}

size_t rb_write(ringbuffer_t *rb, const unsigned char *src, size_t len) {
    size_t written = 0;
    while (written < len && rb->size < rb->capacity) {
        rb->data[rb->tail] = src[written];
        rb->tail = (rb->tail + 1) % rb->capacity;
        rb->size++;
        written++;
    }
    return written;
}

size_t rb_read(ringbuffer_t *rb, unsigned char *dst, size_t len) {
    size_t read = 0;
    while (read < len && rb->size > 0) {
        dst[read] = rb->data[rb->head];
        rb->head = (rb->head + 1) % rb->capacity;
        rb->size--;
        read++;
    }
    return read;
}

void rb_clear(ringbuffer_t *rb) {
    rb->head = 0;
    rb->tail = 0;
    rb->size = 0;
}
