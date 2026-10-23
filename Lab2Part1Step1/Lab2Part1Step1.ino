#include "driver/gpio.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"
#include "soc/gpio_periph.h"

#define GPIO_PIN 5

void setup() {
  // Set GPIO_PIN pin function to GPIO
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[GPIO_PIN], PIN_FUNC_GPIO);

  // Set GPIO pin 5 to output
  *((volatile uint32_t*)GPIO_ENABLE_REG) |= (1 << GPIO_PIN);
}

void loop() {
  // Set the output register bit corresponding to GPIO_PIN to 1
  *((volatile uint32_t*)GPIO_OUT_W1TS_REG) = (1 << GPIO_PIN);
  // Wait 1 second
  delay(1000);

  // Clear the output register bit corresponding to GPIO_PIN
  *((volatile uint32_t*)GPIO_OUT_W1TC_REG) = (1 << GPIO_PIN);
  // Wait 1 second
  delay(1000);
}
