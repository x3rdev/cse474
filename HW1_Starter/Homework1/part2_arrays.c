#include <stdio.h>
#include <limits.h>
#include "hwlib.h"
#include "hw.h"
#include "helpers.h"

// Part 2: Arrays and Strides

int sum_array(const int* arr, int size) {
    // =================> TODO: Return the sum of the first size elements (0 if size <= 0)
    int sum = 0;
    for(int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum;
}

int find_max(const int* arr, int size) {
    // =================> TODO: Return the largest element (INT_MIN if size <= 0)
    int max = INT_MIN;
    for(int i = 0; i < size; i++) {
        if(arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

int filter_positive(const int* in, int* out, int size) {
    // =================> TODO: Copy each element > 0 into out and return the count
    int out_ptr = 0;
    for(int i = 0; i < size; i++) {
        if(in[i] > 0) {
            out[out_ptr] = in[i];
            out_ptr++;
        }
    }
    return out_ptr;
}

int find_max_offset(const int* arr, int size, int offset) {
    // =================> TODO: Return the max of arr[0], arr[offset], arr[2*offset], ...
    //                          Use pointer arithmetic (not arr[i]).
    //                          Return INT_MIN if size <= 0 or offset <= 0.
    if (size <= 0 || offset <= 0) return INT_MIN;
    int max = INT_MIN;
    for(const int* p = arr; p < arr + size; p += offset) {
        if (*p > max) {
            max = *p;
        }
    }
    return max;
}
