#include <stdarg.h>

extern "C" void hw_printf(const char* format, ...) {
  char buffer[128];
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);
  Serial.print(buffer);
}

extern "C" {
  void run_part1();
  void run_part2();
  void run_part3();
  void run_part4();
  void run_my_tests();
}

void setup() {
    Serial.begin(9600);
    delay(5000);
    while(!Serial);
    Serial.println("============ PART 1: Pointers and Raw Memory ============");
    run_part1();

    Serial.println("============ PART 2: Arrays and Strides =================");
    run_part2();

    Serial.println("============ PART 3: 2D Data in Flat Memory =============");
    run_part3();

    Serial.println("============ PART 4: Sensor Records and Ring Buffer =====");
    run_part4();

    Serial.println("============ MY TESTS (not graded) ======================");
    run_my_tests();

    Serial.println("============ DONE =======================================");
}

void loop() {}
