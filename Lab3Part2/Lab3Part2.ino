#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
#define MAX_PRIORITY 4

#define LED_PIN1 4
#define LED_PIN2 5
#define BUZZER_PIN 6

int melody[10] = {260, 290, 330, 350, 390, 440, 490, 520, 440, 390};

struct TCB {
  const char* name;
  int priority;
  bool done;
  void (*func)();
};

void blink_led() {
  for(int i = 0; i < 8; i++) {
    digitalWrite(LED_PIN1, HIGH);
    delay(1000);
    digitalWrite(LED_PIN1, LOW);
    delay(1000);
  }
  
}

void counter() {
  for(int i = 1; i <= 10; i++) {
    lcd.clear();
    lcd.print(i);
    delay(500);
  }
}

void music_player() {
  for (int n = 0; n < 10; n++) {
    int freq = melody[n];
    int half = 500000 / freq;

    ledcWrite(LED_PIN2, (n + 1) * 25);

    for (int i = 0; i < freq / 2; i++) { 
      digitalWrite(BUZZER_PIN, HIGH);
      delayMicroseconds(half);
      digitalWrite(BUZZER_PIN, LOW);
      delayMicroseconds(half);
    }
  }
  digitalWrite(BUZZER_PIN, HIGH);
  ledcWrite(LED_PIN2, 0);
}

void alphabet_printer() {
  for (char c = 'A'; c <= 'Z'; c++) {
    Serial.print(c);
    delay(100);  // readability
  }
  Serial.println();
}


// array of TCB structs
TCB tasks[4] = {
  {"LED Blinker", 4, false, blink_led},
  {"Counter", 3, false, counter},
  {"Music Player", 2, false, music_player},
  {"Alphabet Printer", 1, false, alphabet_printer}
};

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN1, OUTPUT);
  lcd.init();
  lcd.backlight();
  pinMode(BUZZER_PIN, OUTPUT);
  ledcAttach(LED_PIN2, 5000, 8);
}

void loop() {
  // pick task where done == false, priority is highest
  // best initialized to -1, if it remains as this value after this loop, then
  // all tasks are complete
  int best = -1;
  for (int i = 0; i < 4; i++) {
    if (!tasks[i].done && (best < 0 || tasks[i].priority > tasks[best].priority))
      best = i;
  }

  // non-preemptively run best task to completion
  if (best >= 0) {
    tasks[best].func();
    tasks[best].done = true;
    Serial.printf("%s: %d\n", tasks[best].name, tasks[best].priority);
  } else {
    // on next loop iteration, if all tasks done, reset status and cycle
    // priorities by priority % 4 + 1
    for (int i = 0; i < 4; i++) {
      tasks[i].done = false;
      tasks[i].priority = tasks[i].priority % 4 + 1;
    }
  }
}
