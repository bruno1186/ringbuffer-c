#include "ringbuffer.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;
static int checks = 0;

#define CHECK(cond)                                                     \
    do {                                                                \
        checks++;                                                       \
        if (!(cond)) {                                                  \
            failures++;                                                 \
            fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        }                                                               \
    } while (0)

static void test_create_and_empty(void) {
    ringbuffer_t *rb = rb_create(8);
    CHECK(rb != NULL);
    CHECK(rb_capacity(rb) == 8);
    CHECK(rb_size(rb) == 0);
    CHECK(rb_is_empty(rb));
    CHECK(!rb_is_full(rb));
    rb_destroy(rb);
}

static void test_create_zero_returns_null(void) {
    CHECK(rb_create(0) == NULL);
}

static void test_write_then_read(void) {
    ringbuffer_t *rb = rb_create(16);
    const unsigned char in[] = "hello";
    size_t w = rb_write(rb, in, 5);
    CHECK(w == 5);
    CHECK(rb_size(rb) == 5);

    unsigned char out[8] = {0};
    size_t r = rb_read(rb, out, 5);
    CHECK(r == 5);
    CHECK(memcmp(in, out, 5) == 0);
    CHECK(rb_is_empty(rb));
    rb_destroy(rb);
}

static void test_fill_and_overflow(void) {
    ringbuffer_t *rb = rb_create(4);
    const unsigned char in[] = {1, 2, 3, 4, 5, 6};
    size_t w = rb_write(rb, in, 6);
    CHECK(w == 4); /* so cabem 4 */
    CHECK(rb_is_full(rb));
    rb_destroy(rb);
}

static void test_wraparound(void) {
    ringbuffer_t *rb = rb_create(4);
    const unsigned char a[] = {10, 20, 30};
    CHECK(rb_write(rb, a, 3) == 3);

    unsigned char tmp[2];
    CHECK(rb_read(rb, tmp, 2) == 2); /* consome 10,20 */

    const unsigned char b[] = {40, 50, 60};
    CHECK(rb_write(rb, b, 3) == 3); /* usa o espaco liberado (wrap) */
    CHECK(rb_size(rb) == 4);

    unsigned char out[4];
    CHECK(rb_read(rb, out, 4) == 4);
    const unsigned char expected[] = {30, 40, 50, 60};
    CHECK(memcmp(out, expected, 4) == 0);
    rb_destroy(rb);
}

static void test_read_more_than_available(void) {
    ringbuffer_t *rb = rb_create(8);
    const unsigned char in[] = {7, 8};
    rb_write(rb, in, 2);
    unsigned char out[8];
    CHECK(rb_read(rb, out, 8) == 2);
    rb_destroy(rb);
}

static void test_clear(void) {
    ringbuffer_t *rb = rb_create(8);
    const unsigned char in[] = {1, 2, 3};
    rb_write(rb, in, 3);
    rb_clear(rb);
    CHECK(rb_is_empty(rb));
    CHECK(rb_size(rb) == 0);
    rb_destroy(rb);
}

int main(void) {
    test_create_and_empty();
    test_create_zero_returns_null();
    test_write_then_read();
    test_fill_and_overflow();
    test_wraparound();
    test_read_more_than_available();
    test_clear();

    printf("%d checks, %d failures\n", checks, failures);
    return failures == 0 ? 0 : 1;
}
