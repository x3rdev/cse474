#include "driver/gpio.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"
#include "soc/gpio_periph.h"
#include "soc/timer_group_reg.h"

#define GPIO_PIN 5
#define LED_TOGGLE_INTERVAL 50000

void setup() {
  // Set GPIO_PIN pin function to GPIO
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[GPIO_PIN], PIN_FUNC_GPIO);

  // Set GPIO pin 5 to output
  *((volatile uint32_t*)GPIO_ENABLE_REG) |= (1 << GPIO_PIN);

  // Enable timer
  *(volatile uint32_t*)TIMG_T0CONFIG_REG(0) |= (1 << 31);
  // Configure timer to increase
  *(volatile uint32_t*)TIMG_T0CONFIG_REG(0) |= (1 << 30);
  // Configure timer to not auto reload (in case it is set to true before)
  *(volatile uint32_t*)TIMG_T0CONFIG_REG(0) &= ~(1 << 29);
  // Zero the divider, & with 0 over divider bits
  *(volatile uint32_t*)TIMG_T0CONFIG_REG(0) &= ~(0xFFFF << 13);
  // Set divider, bits [28:13] to 80
  *(volatile uint32_t*)TIMG_T0CONFIG_REG(0) |= (80 << 13);
  
  //Trigger timer update to reload settings
  *((volatile uint32_t *)TIMG_T0UPDATE_REG(0)) = 1;

}

void loop() {
  // Track last toggle time
  static uint32_t last_toggle_time = 0;

  // Read current timer value
  uint32_t current_time = 0;
  current_time = *((volatile uint32_t *)TIMG_T0LO_REG(0));

  // Check if toggle interval has passed
  if ((current_time - last_toggle_time) >= LED_TOGGLE_INTERVAL) {
    // Read current GPIO output state
    uint32_t gpio_out = 0;

    gpio_out = *((volatile uint32_t *)GPIO_OUT_REG);

    // Toggle GPIO_PIN using XOR
    *((volatile uint32_t *)GPIO_OUT_REG) = gpio_out ^ (1 << GPIO_PIN);

    // Update last_toggle_time
    last_toggle_time = current_time;
  }

  // Refresh timer counter value
  *((volatile uint32_t *)TIMG_T0UPDATE_REG(0)) = 1;

}
