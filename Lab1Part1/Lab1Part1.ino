#define PIN_0 48
#define PIN_1 45

uint8_t pin_0_state = LOW;
uint8_t pin_1_state = LOW;

void toggle_pin_0();
void toggle_pin_1();

void setup() {
  // pinmode to out
  pinMode(PIN_0, OUTPUT);
  pinMode(PIN_1, OUTPUT);
}

void loop() {
  // turn both leds on
  toggle_pin_0();
  toggle_pin_1();
  delay(500);

  // turn 500ms led off
  toggle_pin_0();
  delay(500);

  // turn 500ms led on
  toggle_pin_0();
  // turn 1000ms led off
  toggle_pin_1();
  delay(500);
  // turn 500ms led off
  toggle_pin_0();
  delay(500);

}

// toggles pin 0
void toggle_pin_0() {
  pin_0_state = !pin_0_state;
  digitalWrite(PIN_0, pin_0_state);
}

// toggles pin 1
void toggle_pin_1() {
  pin_1_state = !pin_1_state;
  digitalWrite(PIN_1, pin_1_state);
}