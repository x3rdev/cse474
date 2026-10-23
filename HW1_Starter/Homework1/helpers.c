// Definitions of the helper functions declared in helpers.h.
// Submit this file along with helpers.h and your four part files.
//
// You can call functions from any part here (they are all declared in hw.h),
// and you can use printf just like in the part files. Part 4's no-malloc rule
// also applies to any helper that Part 4 calls.

#include <stdio.h>
#include <stdlib.h>
#include "hwlib.h"
#include "hw.h"
#include "helpers.h"

// =================> Add your helper definitions below.

int min(int a, int b) {
  if(a<=b) {
    return a;
  }
  return b;
}