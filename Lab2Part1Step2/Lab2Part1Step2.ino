#include "driver/gpio.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"
#include "soc/gpio_periph.h"

#define GPIO_PIN 5

// REGISTER CODE 126 microseconds

void setup() {
  // Set GPIO_PIN pin function to GPIO
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[GPIO_PIN], PIN_FUNC_GPIO);

  // Set GPIO pin 5 to output
  *((volatile uint32_t*)GPIO_ENABLE_REG) |= (1 << GPIO_PIN);

  
  // Initalize serial
  Serial.begin(9600);
}

void loop() {
  // Measure start time
  uint64_t starting_micros = micros();

  // Flip register 1000 times
  for(int i = 0; i < 1000; i++) {
    *((volatile uint32_t*)GPIO_OUT_W1TS_REG) = (1 << GPIO_PIN);
    *((volatile uint32_t*)GPIO_OUT_W1TC_REG) = (1 << GPIO_PIN);
  }
  //print time
  uint64_t duration = micros() - starting_micros;
  Serial.printf("Time elapsed: %lu\n", duration);
  delay(1000);
}

// HELPER FUNCTION CODE 1068 microseconds

// void setup() {
//   // Setup GPIO_PIN to output
//   pinMode(GPIO_PIN, OUTPUT);

//   // Initalize serial
//   Serial.begin(9600);
// }

// void loop() {
//   // Measure start time
//   uint64_t starting_micros = micros();

//   // Flip pin 1000 times
//   for(int i = 0; i < 1000; i++) {
//     digitalWrite(GPIO_PIN, HIGH);
//     digitalWrite(GPIO_PIN, LOW);
//   }
//   //print time
//   uint64_t duration = micros() - starting_micros;
//   Serial.printf("Time elapsed: %lu\n", duration);
//   delay(1000);
// }
