#define PIN_BUTTON_0 21
#define PIN_BUTTON_1 47
#define PIN_LED_0 48
#define PIN_LED_1 45

uint8_t led_0_state = LOW;
uint8_t led_1_state = LOW;
uint8_t button_0_toggled = HIGH;
uint8_t button_1_toggled = HIGH;
uint64_t button_0_last_press_time = 0;
uint64_t button_1_last_press_time = 0;

void toggle_led_0();
void toggle_led_1();
void check_buttons();

void setup() {
  // set button pinmodes to input pullup
  pinMode(PIN_BUTTON_0, INPUT_PULLUP);
  pinMode(PIN_BUTTON_1, INPUT_PULLUP);
  // set button pinmodes to input pullup
  pinMode(PIN_LED_0, OUTPUT);
  pinMode(PIN_LED_1, OUTPUT);
}

void loop() {
  // update button state
  check_buttons();
  // toggle leds
  toggle_led_0();
  toggle_led_1();
  // prevent double toggling
  delay(20);
  

}

// toggles pin 0
void toggle_led_0() {
  if(millis()%500 < 10) {
    led_0_state = !led_0_state;
    if(!button_0_toggled) {
      digitalWrite(PIN_LED_0, LOW);
    } else {
      digitalWrite(PIN_LED_0, led_0_state);
    }    
  }
}

// toggles pin 1
void toggle_led_1() {
  if(millis()%1000 < 10) {
    led_1_state = !led_1_state;
    if(!button_1_toggled) {
      digitalWrite(PIN_LED_1, LOW);
    } else {
      digitalWrite(PIN_LED_1, led_1_state);
    } 
  }

  
}

void check_buttons() {
  // toggle buttons based on pins
  if(millis() - button_0_last_press_time > 500 && !digitalRead(PIN_BUTTON_0)) {
    button_0_toggled = !button_0_toggled;
    button_0_last_press_time = millis();
  }
  if(millis() - button_1_last_press_time > 500 && !digitalRead(PIN_BUTTON_1)) {
    button_1_toggled = !button_1_toggled;
    button_1_last_press_time = millis();
  }
  
  
}