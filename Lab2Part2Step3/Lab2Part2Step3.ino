#include "driver/gpio.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"
#include "soc/gpio_periph.h"
#include "soc/timer_group_reg.h"

#define GPIO_PIN 5
#define LED_TOGGLE_INTERVAL 500000 // 500 ms in 1 us timer ticks

void setup() {
  // Set GPIO_PIN pin function to GPIO
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[GPIO_PIN], PIN_FUNC_GPIO);

  // Set GPIO_PIN to output
  *((volatile uint32_t*)GPIO_ENABLE_W1TS_REG) = (1 << GPIO_PIN);

  // Disable timer while configuring
  *(volatile uint32_t*)TIMG_T0CONFIG_REG(0) &= ~(1u << 31);
  // Configure timer to increase
  *(volatile uint32_t*)TIMG_T0CONFIG_REG(0) |= (1u << 30);
  // Configure timer to not auto reload
  *(volatile uint32_t*)TIMG_T0CONFIG_REG(0) &= ~(1u << 29);
  // Zero the divider, & with 0 over divider bits
  *(volatile uint32_t*)TIMG_T0CONFIG_REG(0) &= ~(0xFFFFu << 13);
  // Set divider, bits [28:13] to 80 (80 MHz / 80 = 1 us per tick)
  *(volatile uint32_t*)TIMG_T0CONFIG_REG(0) |= (80u << 13);
  // Enable timer after configuration is done
  *(volatile uint32_t*)TIMG_T0CONFIG_REG(0) |= (1u << 31);
}

void loop() {
  // Track last toggle time
  static uint32_t last_toggle_time = 0;

  // Latch the current counter value, then read it
  *((volatile uint32_t *)TIMG_T0UPDATE_REG(0)) = 1;
  uint32_t current_time = *((volatile uint32_t *)TIMG_T0LO_REG(0));

  // Check if toggle interval has passed
  if ((current_time - last_toggle_time) >= LED_TOGGLE_INTERVAL) {
    // Read current GPIO output state
    uint32_t gpio_out = *((volatile uint32_t *)GPIO_OUT_REG);

    // Toggle GPIO_PIN using XOR
    *((volatile uint32_t *)GPIO_OUT_REG) = gpio_out ^ (1 << GPIO_PIN);

    // Update last_toggle_time
    last_toggle_time = current_time;
  }
}