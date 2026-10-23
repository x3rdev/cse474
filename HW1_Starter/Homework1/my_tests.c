// Your own tests. This file is NOT graded: the autograder never compiles it,
// so you can add, change, or delete anything here. It runs on your ESP32 after
// the four demo parts, and its output appears under "MY TESTS".
//
// The autograder has hidden tests for edge cases. Use this file to test those
// cases yourself before you submit. Some ideas:
//   - sum_array / find_max with size 0 (see hw.h for the required results)
//   - filter_positive when the array contains 0 or has no positives
//   - find_max_offset with offset 0, offset larger than size, or a size that
//     is not a multiple of offset
//   - get_2d / set_2d / row_sums on a non-square matrix such as 3 x 4
//   - bytes_to_u32_le with bytes 0x80 or larger, e.g. {0xEF, 0xBE, 0xAD, 0xDE}
//   - fill_sensors with n = 0, n larger than MAX_BATCH, and labels "T7" and up
//   - the ring buffer after it wraps around: push 4, pop 3, push 3, then check
//     buffer_count, the order pop returns, and buffer_average

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "hwlib.h"
#include "hw.h"
#include "helpers.h"

static int passed = 0;
static int failed = 0;

// CHECK(condition, "description"): prints FAIL with the line number if the condition is false.
#define CHECK(cond, what) do { \
    if (cond) { passed++; } \
    else { failed++; printf("  FAIL (my_tests.c line %d): %s\n", __LINE__, what); } \
} while (0)

// CHECK_INT(actual, expected, "description"): also prints both values on failure.
#define CHECK_INT(actual, expected, what) do { \
    long a_ = (long)(actual), e_ = (long)(expected); \
    if (a_ == e_) { passed++; } \
    else { failed++; printf("  FAIL (my_tests.c line %d): %s: expected %ld, got %ld\n", \
                            __LINE__, what, e_, a_); } \
} while (0)

// ---------------------------------------------------------------------------
// Example tests: copy these patterns to write your own.
// ---------------------------------------------------------------------------

static void test_find_max_all_negative(void) {
    int arr[4] = {-8, -3, -5, -9};
    CHECK_INT(find_max(arr, 4), -3, "find_max of all negative values");
}

static void test_push_rejects_when_full(void) {
    SensorBuffer buf;
    init_buffer(&buf);
    Sensor s = {1, "A", 1.0f};
    for (int i = 0; i < BUF_CAPACITY - 1; i++) {
        CHECK(push(&buf, &s), "push into a buffer that is not full");
    }
    CHECK(is_full(&buf), "buffer is full after BUF_CAPACITY - 1 pushes");
    CHECK(!push(&buf, &s), "push into a full buffer returns false");
    CHECK_INT(buffer_count(&buf), BUF_CAPACITY - 1, "count after a rejected push");
}

// ---------------------------------------------------------------------------
// Add your own test functions above, then call them here.
// ---------------------------------------------------------------------------

void run_my_tests(void) {
    test_find_max_all_negative();
    test_push_rejects_when_full();

    printf("my_tests: %d passed, %d failed\n", passed, failed);
}
