#ifndef HELPERS_H
#define HELPERS_H

// Prototypes for your helper functions. Every part file (and my_tests.c)
// includes this header, so any helper declared here can be used in all of them.
//
// How to add a helper:
//   1. Write its prototype here (just the signature, ending in a semicolon).
//   2. Write its definition (the body) in helpers.c.
// Submit both helpers.h and helpers.c along with your four part files.
//
// Example:
//   helpers.h:   int max_int(int a, int b);
//   helpers.c:   int max_int(int a, int b) { return a > b ? a : b; }
//
// Only prototypes go in this file. A function body here would be copied into
// every .c file that includes it, and the build would fail with a
// "multiple definition" / "duplicate symbol" error.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "hw.h"

// =================> Add your helper prototypes below.
int min(int a, int b);

#endif
