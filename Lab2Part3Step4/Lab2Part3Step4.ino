#define INPUT_PIN 1
#define LED_PIN 19

#define RESOLUTION 10
#define FREQ 5000

void setup() {
  ledcAttach(LED_PIN, FREQ, RESOLUTION);
}

void loop() {
  int ambient = analogRead(INPUT_PIN);
  // We invert the ambient light level based on the resolution:
  // 2^RESOLUTION - norm(ambient)
  int duty = (int) ((1 << RESOLUTION) * ((1 << 12) - (float) ambient)/(1 << 12));
  ledcWrite(LED_PIN, duty);
  // poll ambient light 10 times/s
  delay(100);
}
