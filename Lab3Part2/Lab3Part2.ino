/*
 * File:        Lab3Part2.ino
 * Author:      Ryan Kim, Nicolae Ungur
 * Date:        2026-11-17
 * Description: Non-preemptive priority scheduler running four tasks via TCBs.
 *              Priorities rotate after every full round.
 */

// ============================== Includes ==============================
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// =============================== Macros ===============================
#define NUM_TASKS    4
#define MAX_PRIORITY 4

#define LED_PIN1   4   // blinker LED
#define LED_PIN2   5   // note LED (PWM)
#define BUZZER_PIN 6

#define NUM_NOTES  10

// ========================== Global Variables ==========================
LiquidCrystal_I2C lcd(0x27, 16, 2);

int melody[NUM_NOTES] = {260, 290, 330, 350, 390, 440, 490, 520, 440, 390};

struct TCB {
  const char* name; 
  int priority; // 1 (low) to 4 (high)
  bool done;
  void (*func)();
};

// ======================== Function Prototypes =========================
void blink_led();
void counter();
void music_player();
void alphabet_printer(); 
 
TCB tasks[NUM_TASKS] = {
  {"LED Blinker",      4, false, blink_led},
  {"Counter",          3, false, counter},
  {"Music Player",     2, false, music_player},
  {"Alphabet Printer", 1, false, alphabet_printer}
};

// ============================ Setup / Loop ============================
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN1, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  ledcAttach(LED_PIN2, 5000, 8); 
  lcd.init(); 
  lcd.backlight();
}

void loop() {
  // Find the highest-priority unfinished task, -1 if all are done
  int best = -1;
  for (int i = 0; i < NUM_TASKS; i++) {
    if (!tasks[i].done && (best < 0 || tasks[i].priority > tasks[best].priority))
      best = i;
  }

  if (best >= 0) {
    tasks[best].func(); // runs to completion
    tasks[best].done = true;
    Serial.printf("%s: %d\n", tasks[best].name, tasks[best].priority);
  } else {
    // Round finished: reset and rotate priorities
    for (int i = 0; i < NUM_TASKS; i++) {
      tasks[i].done = false;
      tasks[i].priority = tasks[i].priority % MAX_PRIORITY + 1;
    }
  }
}

// ======================= Function Implementations =====================

/*
 * Blinks LED_PIN1 eight times, 1 s on and 1 s off.
 * Params: none   Returns: none
 */
void blink_led() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(LED_PIN1, HIGH);
    delay(1000);
    digitalWrite(LED_PIN1, LOW);
    delay(1000);
  }
}

/*
 * Counts 1 to 10 on the LCD.
 * Params: none   Returns: none
 */
void counter() {
  for (int i = 1; i <= 10; i++) {
    lcd.clear();
    lcd.print(i);
    delay(500);
  }
}

/*
 * Plays melody[] on the buzzer for 0.5 s per note. The note LED gets
 * brighter with each note.
 * Params: none   Returns: none
 */
void music_player() {
  for (int n = 0; n < NUM_NOTES; n++) {
    int freq = melody[n];
    int half = 500000 / freq;   // half period in us

    ledcWrite(LED_PIN2, (n + 1) * 25);   // duty 25 to 250

    // Manual square wave: freq/2 cycles = 0.5 s
    for (int i = 0; i < freq / 2; i++) {
      digitalWrite(BUZZER_PIN, HIGH);
      delayMicroseconds(half);
      digitalWrite(BUZZER_PIN, LOW);
      delayMicroseconds(half);
    }
  }
  digitalWrite(BUZZER_PIN, LOW);
  ledcWrite(LED_PIN2, 0);
}

/*
 * Prints A to Z to the Serial Monitor.
 * Params: none   Returns: none
 */
void alphabet_printer() {
  for (char c = 'A'; c <= 'Z'; c++) {
    Serial.print(c);
    delay(100);
  }
  Serial.println();
}