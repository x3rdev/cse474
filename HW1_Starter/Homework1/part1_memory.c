#include <stdio.h>
#include <stdint.h>
#include "hwlib.h"
#include "hw.h"
#include "helpers.h"

// Part 1: Pointers and Raw Memory
// Do not use memcpy(), memmove(), strcpy(), or any other library copy routine.

void swap_ints(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void my_memcpy(void* dest, const void* src, size_t size) {
    // =================> TODO: Cast dest and src to uint8_t* and copy size bytes in a loop
    for(int i = 0; i < size; i++) {
        ((uint8_t*) dest)[i] = ((uint8_t*) src)[i];
    }
}

bool is_little_endian(void) {
    // =================> TODO: Store a known multi-byte value in a variable, then
    //                          look at its first byte through a uint8_t* pointer
    uint32_t test = 0xDEADBEEF;
    return ((uint8_t*)&test)[0] == 0xEF;
}

uint32_t bytes_to_u32_le(const uint8_t* bytes) {
    // =================> TODO: Combine the 4 bytes using shifts (<<) and OR (|).
    //                          bytes[0] is the least significant byte.
    uint32_t res = 0;
    res |= ((uint32_t)bytes[3]) << 3*8;
    res |= ((uint32_t)bytes[2]) << 2*8;
    res |= ((uint32_t)bytes[1]) << 1*8;
    res |= ((uint32_t)bytes[0]);
    return res;
}
