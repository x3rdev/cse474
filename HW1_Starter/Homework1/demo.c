// Demo driver: calls your functions and prints the results to the Serial
// Monitor. Do NOT modify this file. Compare your output with the expected
// output in the handout (memory addresses will differ).

#include <stdio.h>
#include <limits.h>
#include "hwlib.h"
#include "hw.h"

static void print_array(const char* name, const int* arr, int size) {
    printf("%s:", name);
    for (int i = 0; i < size; i++) {
        printf(" %d", arr[i]);
    }
    printf("\n");
}

void run_part1(void) {
    int a = 3, b = 7;
    swap_ints(&a, &b);
    printf("After swap: a = %d, b = %d\n", a, b);

    const char* src = "hello world";
    char dest[20] = {0};
    my_memcpy(dest, src, 12);   // 11 characters + '\0'
    printf("Copied string: %s\n", dest);

    uint32_t value = 0x11223344;
    uint8_t* bytes = (uint8_t*)&value;
    printf("Bytes of 0x%08lX in memory:\n", (unsigned long)value);
    for (int i = 0; i < 4; i++) {
        printf("  byte %d = 0x%02X at %p\n", i, bytes[i], (void*)&bytes[i]);
    }
    printf("Little-endian: %s\n", is_little_endian() ? "yes" : "no");

    uint8_t packet[4] = {0x78, 0x56, 0x34, 0x12};
    printf("Packet as uint32: 0x%08lX\n", (unsigned long)bytes_to_u32_le(packet));
}

void run_part2(void) {
    int arr[6] = {1, -2, 3, -4, 5, -6};
    int filtered[6];

    print_array("Original array", arr, 6);
    printf("Sum = %d\n", sum_array(arr, 6));
    printf("Max = %d\n", find_max(arr, 6));

    int count = filter_positive(arr, filtered, 6);
    printf("Filtered positives (%d):", count);
    for (int i = 0; i < count; i++) {
        printf(" %d", filtered[i]);
    }
    printf("\n");
    printf("Max with offset 2: %d\n", find_max_offset(arr, 6, 2));
    printf("Max with offset 3: %d\n", find_max_offset(arr, 6, 3));
}

void run_part3(void) {
    int matrix[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int* flat = &matrix[0][0];

    for (int r = 0; r < 2; r++) {
        for (int c = 0; c < 3; c++) {
            printf("matrix[%d][%d] = %d, get_2d = %d, at %p\n",
                   r, c, matrix[r][c], get_2d(flat, 2, 3, r, c), (void*)&matrix[r][c]);
        }
    }

    set_2d(flat, 2, 3, 1, 2, 60);
    printf("After set_2d(1, 2, 60): matrix[1][2] = %d\n", matrix[1][2]);

    int sums[2];
    row_sums(flat, 2, 3, sums);
    printf("Row sums: %d %d\n", sums[0], sums[1]);

    zero_2d(flat, 2, 3);
    printf("Zeroed matrix:\n");
    for (int r = 0; r < 2; r++) {
        printf("  %d %d %d\n", matrix[r][0], matrix[r][1], matrix[r][2]);
    }
}

void run_part4(void) {
    SensorBuffer buf;
    init_buffer(&buf);

    int pushed = load_batch(&buf, 5, 100, 20.0f, 1.5f);
    printf("Loaded %d of 5 sensors (capacity %d)\n", pushed, BUF_CAPACITY - 1);
    print_buffer(&buf);
    printf("Average reading: %.2f\n", buffer_average(&buf));

    Sensor s;
    for (int i = 0; i < 2; i++) {
        if (pop(&buf, &s)) {
            printf("Popped %s (id %d, %.1f)\n", s.label, s.id, s.reading);
        }
    }

    Sensor extra[2] = {{200, "HUM", 55.0f}, {201, "PRES", 101.3f}};
    for (int i = 0; i < 2; i++) {
        printf("Push %s: %s\n", extra[i].label, push(&buf, &extra[i]) ? "ok" : "full");
    }
    print_buffer(&buf);
    printf("Count = %d, full = %s\n", buffer_count(&buf), is_full(&buf) ? "yes" : "no");
    printf("Average reading: %.2f\n", buffer_average(&buf));
}
