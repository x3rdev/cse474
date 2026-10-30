#include "driver/gpio.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"
#include "soc/gpio_periph.h"
#include "soc/timer_group_reg.h"

#define INPUT_PIN 1
#define BUZZER_PIN 19

#define RESOLUTION 12

// buzzer frequencies
#define FREQ_LOW 500
#define FREQ_MED 1000
#define FREQ_HI 2000
#define DUTY (1 << (RESOLUTION - 1))   // 50% duty cycle
#define LIGHT_THRESHOLD 3000 // This number worked well for the room we were in.           

#define BUZZER_TOGGLE_INTERVAL 500000 // 500 ms in 1 us timer ticks

void setup() {
  // Disable timer while configuring
  *(volatile uint32_t*)TIMG_T0CONFIG_REG(0) &= ~(1u << 31);
  // Configure timer to increase
  *(volatile uint32_t*)TIMG_T0CONFIG_REG(0) |= (1u << 30);
  // Configure timer to not auto reload (in case it is set to true before)
  *(volatile uint32_t*)TIMG_T0CONFIG_REG(0) &= ~(1u << 29);
  // Zero the divider, & with 0 over divider bits
  *(volatile uint32_t*)TIMG_T0CONFIG_REG(0) &= ~(0xFFFFu << 13);
  // Set divider, bits [28:13] to 80 (80 MHz / 80 = 1 us per tick)
  *(volatile uint32_t*)TIMG_T0CONFIG_REG(0) |= (80u << 13);
  // Enable timer after configuration is done
  *(volatile uint32_t*)TIMG_T0CONFIG_REG(0) |= (1u << 31);

  ledcAttach(BUZZER_PIN, FREQ_LOW, RESOLUTION);
}

void loop() {
  // Track last toggle time
  static uint32_t last_toggle_time = 0;
  static int count = 0;

  // Latch the current counter value, then read it
  *((volatile uint32_t *)TIMG_T0UPDATE_REG(0)) = 1;
  uint32_t current_time = *((volatile uint32_t *)TIMG_T0LO_REG(0));

  int ambient = analogRead(INPUT_PIN);
  if (ambient > LIGHT_THRESHOLD) {
    ledcWrite(BUZZER_PIN, 0);
    count = 0;  // restart at low on next trigger
    
    last_toggle_time = current_time - BUZZER_TOGGLE_INTERVAL;
    return;
  }

  // Check if toggle interval has passed
  if ((current_time - last_toggle_time) >= BUZZER_TOGGLE_INTERVAL) {
    if (count == 0) {
      //low
      ledcChangeFrequency(BUZZER_PIN, FREQ_LOW, RESOLUTION);
      ledcWrite(BUZZER_PIN, DUTY);
    } else if (count == 1) {
      //med
      ledcChangeFrequency(BUZZER_PIN, FREQ_MED, RESOLUTION);
      ledcWrite(BUZZER_PIN, DUTY);
    } else if (count == 2) {
      // hi
      ledcChangeFrequency(BUZZER_PIN, FREQ_HI, RESOLUTION);
      ledcWrite(BUZZER_PIN, DUTY);
    } else {
      ledcWrite(BUZZER_PIN, 0);
    }

    if (count < 3) count++;   // stop after one sequence

    // Update last_toggle_time
    last_toggle_time = current_time;
  }
}