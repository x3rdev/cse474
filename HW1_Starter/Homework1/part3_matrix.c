#include <stdio.h>
#include "hwlib.h"
#include "hw.h"
#include "helpers.h"

// Part 3: 2D Data in Flat Memory
// data points to a rows x cols matrix stored in row-major order.

void zero_2d(int* data, int rows, int cols) {
    // =================> TODO: Set all rows * cols elements to 0
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            data[i*cols+j] = 0;
        }
    }
}

int get_2d(const int* data, int rows, int cols, int r, int c) {
    // =================> TODO: Compute the flat index of (r, c) and return that element
    
    return data[r*cols + c];
}

void set_2d(int* data, int rows, int cols, int r, int c, int value) {
    // =================> TODO: Store value at (r, c)
    data[r*cols + c] = value;
}

void row_sums(const int* data, int rows, int cols, int* out) {
    // =================> TODO: Write the sum of each row into out[r].
    //                          Hint: each row is itself a 1D array. Can you reuse sum_array()?
    for(int i = 0; i < rows; i++) {
        out[i] = sum_array(data + i*cols, cols);
    }
}
