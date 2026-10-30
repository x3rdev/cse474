#define INPUT_PIN 1
#define LED_PIN 6

#define RESOLUTION 10
#define FREQ 5000

#define READING_MAX ((1 << 12) - 1) // 4095, max reading from analog_read
#define DUTY_MAX ((1 << RESOLUTION) - 1) // 1023, max 10-bit duty

void setup() {
  Serial.begin(9600);
  ledcAttach(LED_PIN, FREQ, RESOLUTION);
}

void loop() {
  int ambient = analogRead(INPUT_PIN);
  Serial.println(ambient);
  // Invert and scale the 12-bit reading to the 10-bit duty range:
  // ambient 0 -> DUTY_MAX (brightest), ambient READING_MAX -> 0 (dimmest)
  int duty = ((READING_MAX - ambient) * DUTY_MAX) / READING_MAX;
  Serial.println(duty);
  ledcWrite(LED_PIN, duty);
  // poll ambient light 10 times/s
  delay(100);
}