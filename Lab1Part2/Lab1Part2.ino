#define PIN_0 45
#define PIN_1 21

void setup() {
  // set led pin mode to output
  pinMode(PIN_0, OUTPUT);
  // set button pin mode to input_pullup
  pinMode(PIN_1, INPUT_PULLUP);

  // turn signal on so that button can complete circuit
  digitalWrite(PIN_1, HIGH);
}

void loop() {
  if(digitalRead(PIN_1)) {
    digitalWrite(PIN_0, LOW);
  } else {
    digitalWrite(PIN_0, HIGH);
  }

}
