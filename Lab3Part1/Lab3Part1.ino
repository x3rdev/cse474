#include <Wire.h>
// Ensure that the LiquidCrystal I2C library by Frank de Brabander is installed in your Arduino IDE Library Manager
#include <LiquidCrystal_I2C.h>

#define BUF_SIZE 64

LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize the LCD
char line[64];
int write_pos = 0;

void setup() {
 Serial.begin(115200);
 Wire.begin();
 lcd.init();
 delay(2);
}


void loop() {
 // ====================> TODO:
 // Write code that takes Serial input and displays it to the LCD. Do NOT use any 
 // functions from the LiquidCrystal I2C library here.
  while(Serial.available()) {
     char c = Serial.read();
     if(c == '\r') continue;

     if(c == '\n') {
      print_to_lcd();
      write_pos = 0;
     } else if (write_pos < BUF_SIZE - 1) {
      line[write_pos] = c;  
      write_pos++;
     }
       
  }
}

void print_to_lcd() {
  // Clear display
  Wire.beginTransmission(0x27);
  send_command(0x01);
  Wire.endTransmission();
  delay(10);

  // Wire.beginTransmission(0x27);
  // send_command(0x80); 
  // for (int i = 0; i < write_pos; i++) {
  //   send_data(line[i]);
  // }
  // Wire.endTransmission();
  for (int i = 0; i < write_pos && i < 32; i++) {
    Wire.beginTransmission(0x27);
    if (i == 0)  send_command(0x80);
    if (i == 16) send_command(0xC0);
    send_data(line[i]);
    Wire.endTransmission();
  }

}

void send_command(uint8_t c) {
  uint8_t left_nibble = c & 0xF0;
  uint8_t right_nibble = (c << 4) & 0xF0;

  Wire.write(left_nibble + 0b1100);
  Wire.write(left_nibble + 0b1000);

  Wire.write(right_nibble + 0b1100);
  Wire.write(right_nibble + 0b1000);
}

void send_data(uint8_t c) {
  uint8_t left_nibble = c & 0xF0;
  uint8_t right_nibble = (c << 4) & 0xF0;

  Wire.write(left_nibble + 0b1101);
  Wire.write(left_nibble + 0b1001);

  Wire.write(right_nibble + 0b1101);
  Wire.write(right_nibble + 0b1001);
}