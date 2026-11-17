/*
 * File:        Lab3Part1.ino
 * Author:      Ryan Kim, Nicolae Ungur
 * Date:        2026-11-17
 * Description: Displays lines typed in the Serial Monitor on a 16x2 I2C LCD
 *              using raw Wire calls.
 */

// ============================== Includes ==============================
#include <Wire.h>
#include <LiquidCrystal_I2C.h>   // Frank de Brabander, used only for init

// =============================== Macros ===============================
#define BUF_SIZE  64
#define LCD_ADDR  0x27
#define LCD_COLS  16
#define LCD_ROWS  2

#define CMD_CLEAR      0x01
#define CMD_ROW0_START 0x80
#define CMD_ROW1_START 0xC0

// Low nibble control bits: backlight | EN | RW | RS
#define CTRL_CMD_EN_HIGH   0b1100
#define CTRL_CMD_EN_LOW    0b1000
#define CTRL_DATA_EN_HIGH  0b1101
#define CTRL_DATA_EN_LOW   0b1001

// ========================== Global Variables ==========================
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);
char line[BUF_SIZE];
int write_pos = 0;

// ======================== Function Prototypes =========================
void print_to_lcd();
void send_command(uint8_t c);
void send_data(uint8_t c);

// ============================ Setup / Loop ============================
void setup() {
  Serial.begin(115200);
  Wire.begin();
  lcd.init();
  delay(2);
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\r') continue;   // ignore CR from "\r\n" line endings

    if (c == '\n') {
      print_to_lcd();
      write_pos = 0;
    } else if (write_pos < BUF_SIZE - 1) {
      line[write_pos++] = c;
    }
  }
}

// ======================= Function Implementations =====================

/*
 * Clears the LCD and prints line[] across both rows (max 32 chars).
 * Params: none   Returns: none
 */
void print_to_lcd() {
  Wire.beginTransmission(LCD_ADDR);
  send_command(CMD_CLEAR);
  Wire.endTransmission();
  delay(10);   // Delay for clearing screen

  for (int i = 0; i < write_pos && i < LCD_COLS * LCD_ROWS; i++) {
    Wire.beginTransmission(LCD_ADDR);
    if (i == 0)        send_command(CMD_ROW0_START);
    if (i == LCD_COLS) send_command(CMD_ROW1_START);
    send_data(line[i]);
    Wire.endTransmission();
  }
}

/*
 * Queues a command byte
 * Params: c - command byte   Returns: none
 */
void send_command(uint8_t c) {
  uint8_t hi = c & 0xF0;
  uint8_t lo = (c << 4) & 0xF0;
  Wire.write(hi | CTRL_CMD_EN_HIGH);
  Wire.write(hi | CTRL_CMD_EN_LOW);
  Wire.write(lo | CTRL_CMD_EN_HIGH);
  Wire.write(lo | CTRL_CMD_EN_LOW);
}

/*
 * Queues a character byte
 * Params: c - ASCII character   Returns: none
 */
void send_data(uint8_t c) {
  uint8_t hi = c & 0xF0;
  uint8_t lo = (c << 4) & 0xF0;
  Wire.write(hi | CTRL_DATA_EN_HIGH);
  Wire.write(hi | CTRL_DATA_EN_LOW);
  Wire.write(lo | CTRL_DATA_EN_HIGH);
  Wire.write(lo | CTRL_DATA_EN_LOW);
}