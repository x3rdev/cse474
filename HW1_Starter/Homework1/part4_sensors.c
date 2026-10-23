#include <stdio.h>
#include "hwlib.h"
#include "hw.h"
#include "helpers.h"

// Part 4: Sensor Records and a Ring Buffer
// The Sensor and SensorBuffer types are defined in hw.h.
// Do not use malloc, calloc, realloc, or free in this part: use fixed-size arrays.
// The buffer is empty when head == tail and full when advancing head would make it equal tail.

static Sensor batch[MAX_BATCH];

int fill_sensors(Sensor out[MAX_BATCH], int n, int base_id, float start_reading, float step) {
    // =================> TODO: Fill in min(n, MAX_BATCH) sensors: id, label ("T0", "T1", ...
    //                          using snprintf), and reading. Return how many you filled
    //                          (0 if n <= 0). Never write past out[MAX_BATCH - 1].
    if(n <= 0) {
        return 0;
    }
    int count = n < MAX_BATCH ? n : MAX_BATCH;
    for(int i = 0; i < count; i++) {
        out[i].id = base_id+i;
        snprintf(out[i].label, sizeof(out[i].label), "T%d", i);
        out[i].reading = start_reading + i*step;
    }

    return min(n, MAX_BATCH);
}

void init_buffer(SensorBuffer* buf) {
    // =================> TODO: Set head and tail to 0 and zero out every slot
    buf->head = 0;
    buf->tail = 0;
    for(int i = 0; i < BUF_CAPACITY; i++) {
        buf->items[i] = (Sensor) {0};
    }
}

bool is_empty(const SensorBuffer* buf) {
    // =================> TODO
    return buf->head == buf->tail;
}

bool is_full(const SensorBuffer* buf) {
    // =================> TODO: Use modulo arithmetic
    return (buf->head+1)%BUF_CAPACITY == buf->tail;
}

int buffer_count(const SensorBuffer* buf) {
    // =================> TODO: Number of stored records (watch out for wraparound)
    return (buf->head - buf->tail + BUF_CAPACITY) % BUF_CAPACITY;
}

bool push(SensorBuffer* buf, const Sensor* s) {
    // =================> TODO: If full return false. Otherwise copy *s into items[head]
    //                          and advance head with wraparound.
    if(is_full(buf)) {
        return false;
    }
    buf->items[buf->head] = *s;
    buf->head = (buf->head+1)%BUF_CAPACITY;
    return true;
}

bool pop(SensorBuffer* buf, Sensor* out) {
    // =================> TODO: If empty return false. Otherwise copy items[tail] into *out
    //                          and advance tail with wraparound.
    if(is_empty(buf)) {
        return false;
    }
    *out = buf->items[buf->tail];
    buf->tail = (buf->tail+1)%BUF_CAPACITY;
    return true;
}

float buffer_average(const SensorBuffer* buf) {
    // =================> TODO: Average reading of the stored records (0.0f if empty)
    int count = buffer_count(buf);
    if (count == 0) return 0.0f;
    float sum = 0.0f;
    for (int i = 0; i < count; i++) {
        sum += buf->items[(buf->tail + i) % BUF_CAPACITY].reading;
    }
    return sum / count;
}

void print_buffer(const SensorBuffer* buf) {
    // =================> TODO: Print each stored record from tail to head, e.g.
    //                          "  [T0] id=100 reading=20.0"
    int count = buffer_count(buf);
    printf("Buffer (%d):\n", count);
    for (int k = 0; k < count; k++) {
        const Sensor* s = &buf->items[(buf->tail + k) % BUF_CAPACITY];
        printf("  [%s] id=%d reading=%.1f\n", s->label, s->id, s->reading);
    }
}

int load_batch(SensorBuffer* buf, int n, int base_id, float start_reading, float step) {
    // =================> TODO: Declare a fixed-size array of MAX_BATCH Sensors, fill it with
    //                          fill_sensors(), push records until all are pushed or the
    //                          buffer is full, and return how many were pushed
    
    int pushed = 0;
    int filled = fill_sensors(batch, n, base_id, start_reading, step);

    while (pushed < filled && push(buf, &batch[pushed])) {
        pushed++;
    }
    return pushed;;
}
