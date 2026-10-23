#ifndef HW_H
#define HW_H

// Homework 1: Sensor Data Pipeline
// Function prototypes and types for all four parts.
// Do NOT modify this file. The autograder uses its own copy, so any change
// you make here will be ignored and may cause your code to fail to compile.

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ---------------------------------------------------------------------------
// Part 1: Pointers and Raw Memory          (part1_memory.c)
// ---------------------------------------------------------------------------

// Exchange the values pointed to by a and b.
void swap_ints(int* a, int* b);

// Copy size bytes from src to dest, one byte at a time.
// Do not use memcpy(), memmove(), strcpy(), or any other library copy routine.
void my_memcpy(void* dest, const void* src, size_t size);

// Return true if this machine stores the least significant byte of a
// multi-byte integer at the lowest address (little-endian).
bool is_little_endian(void);

// Build a 32-bit value from 4 bytes stored in little-endian order
// (bytes[0] is the least significant byte), the way many sensors send data.
// Must give the same answer on any machine, regardless of its endianness.
uint32_t bytes_to_u32_le(const uint8_t* bytes);

// ---------------------------------------------------------------------------
// Part 2: Arrays and Strides               (part2_arrays.c)
// ---------------------------------------------------------------------------

// Return the sum of the first size elements. Return 0 if size <= 0.
int sum_array(const int* arr, int size);

// Return the largest of the first size elements. Return INT_MIN if size <= 0.
int find_max(const int* arr, int size);

// Copy every element of in that is > 0 into out, in order.
// Return how many elements were copied.
int filter_positive(const int* in, int* out, int size);

// Return the largest of arr[0], arr[offset], arr[2*offset], ... (indices < size).
// Return INT_MIN if size <= 0 or offset <= 0.
int find_max_offset(const int* arr, int size, int offset);

// ---------------------------------------------------------------------------
// Part 3: 2D Data in Flat Memory           (part3_matrix.c)
// ---------------------------------------------------------------------------
// In every function below, data points to a rows x cols matrix stored in
// row-major order (the way C lays out int m[rows][cols]).

// Set every element of the matrix to 0.
void zero_2d(int* data, int rows, int cols);

// Return the element at row r, column c.
int get_2d(const int* data, int rows, int cols, int r, int c);

// Set the element at row r, column c to value.
void set_2d(int* data, int rows, int cols, int r, int c, int value);

// Write the sum of each row into out[0 .. rows-1].
void row_sums(const int* data, int rows, int cols, int* out);

// ---------------------------------------------------------------------------
// Part 4: Sensor Records and a Ring Buffer (part4_sensors.c)
// ---------------------------------------------------------------------------
// Part 4 must not use malloc, calloc, realloc, or free. All storage has a fixed
// size known at compile time, as is common in embedded firmware.

#define LABEL_LEN 10

typedef struct {
    int id;
    char label[LABEL_LEN];
    float reading;
} Sensor;

// The buffer holds at most BUF_CAPACITY - 1 records (one slot stays empty).
#define BUF_CAPACITY 5

// The largest batch of sensors that fill_sensors() / load_batch() will create.
#define MAX_BATCH 8

typedef struct {
    Sensor items[BUF_CAPACITY];
    int head;   // index where the next record is written
    int tail;   // index of the oldest record (next to be read)
} SensorBuffer;

// Fill out[0], out[1], ... with count = min(n, MAX_BATCH) sensors, where for each i
//   id      = base_id + i
//   label   = "T<i>"  (e.g. "T0", "T1", ...)
//   reading = start_reading + i * step
// out must have room for MAX_BATCH sensors. Never write past out[MAX_BATCH - 1].
// Return count (0 if n <= 0, and then write nothing).
int fill_sensors(Sensor out[MAX_BATCH], int n, int base_id, float start_reading, float step);

// Set head and tail to 0 and zero out every slot.
void init_buffer(SensorBuffer* buf);

bool is_empty(const SensorBuffer* buf);
bool is_full(const SensorBuffer* buf);

// Return the number of records currently stored.
int buffer_count(const SensorBuffer* buf);

// Copy *s into the buffer. Return false (and change nothing) if full.
bool push(SensorBuffer* buf, const Sensor* s);

// Copy the oldest record into *out and remove it. Return false if empty.
bool pop(SensorBuffer* buf, Sensor* out);

// Return the average reading of the stored records, or 0.0f if empty.
float buffer_average(const SensorBuffer* buf);

// Print the stored records from oldest to newest. (Not autograded.)
void print_buffer(const SensorBuffer* buf);

// Use fill_sensors() to make a batch of n sensors in a fixed-size array of
// MAX_BATCH Sensors (no malloc), then push them into buf in order until all are
// pushed or buf is full. Return how many were pushed.
int load_batch(SensorBuffer* buf, int n, int base_id, float start_reading, float step);

#ifdef __cplusplus
}
#endif

#endif
