#include "driver/gpio.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"
#include "soc/gpio_periph.h"
#include "soc/timer_group_reg.h"

#define INPUT_PIN 1
#define BUZZER_PIN 19

#define RESOLUTION 12
#define FREQ 5000

// buzzer frequencies
#define HI (1 << RESOLUTION) * 0.3
#define MED (1 << RESOLUTION) * 0.4
#define LOW (1 << RESOLUTION) * 0.5   

#define BUZZER_TOGGLE_INTERVAL 500000 // 500 ms

void setup() {
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

  ledcAttach(BUZZER_PIN, FREQ, RESOLUTION);
}

void loop() {
  int ambient = analogRead(INPUT_PIN);
  if (((1 << 12) - (float) ambient)/(1 << 12) < 0.3F) {
    ledcWrite(BUZZER_PIN, 0);
    return;
  }

  // Track last toggle time
  static uint32_t last_toggle_time = 0;

  // Read current timer value
  uint32_t current_time = 0;
  current_time = *((volatile uint32_t *)TIMG_T0LO_REG(0));

  // Check if toggle interval has passed
  static int count = 0;
  if ((current_time - last_toggle_time) >= BUZZER_TOGGLE_INTERVAL) {
    if (count == 0) {
      //low
      ledcWrite(BUZZER_PIN, LOW);
    } else if (count == 1) {
      //med
      ledcWrite(BUZZER_PIN, MED);
    } else if (count == 2) {
      // hi
      ledcWrite(BUZZER_PIN, HI);
    } else {
      ledcWrite(BUZZER_PIN, 0);
    }

    count = (count+1)%5;

    // Update last_toggle_time
    last_toggle_time = current_time;
  }
  
  

  // Refresh timer counter value
  *((volatile uint32_t *)TIMG_T0UPDATE_REG(0)) = 1;
}
