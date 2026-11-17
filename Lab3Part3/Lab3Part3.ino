/*
 * File:        Lab3Part3.ino
 * Authors:     Ryan Kim, Nicolae Ungur
 * Date:        2026-11-17
 * Description: Shows a counter on the LCD that a timer interrupt advances
 *              every second. A button interrupt or a BLE write shows a
 *              message for 2 s, then the counter resumes.
 */

// ============================== Includes ==============================
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <LiquidCrystal_I2C.h>

// =============================== Macros ===============================
#define SERVICE_UUID        "1731c169-65a1-418d-9628-68a937a81cdc"
#define CHARACTERISTIC_UUID "49c15281-84d1-488a-94e7-90c72557e4df"

#define BUTTON_PIN   7
#define MSG_TIME_MS  2000
#define DEBOUNCE_MS  200
#define TIMER_HZ    1000000  // 1 MHz timer tick

// ========================== Global Variables ==========================
LiquidCrystal_I2C lcd(0x27, 16, 2);
hw_timer_t *timer = NULL;

volatile bool tickFlag = false;
volatile bool buttonFlag = false;
volatile bool bleFlag = false;
volatile unsigned long lastPress = 0;

int counter = 0;
bool showingMsg = false;
unsigned long msgStart = 0;

// ======================== Function Prototypes =========================
void IRAM_ATTR onTimer();
void IRAM_ATTR onButton();
void showMessage(const char *msg);

/*
 * BLE callback
 * Params: pCharacteristic - the written characteristic   Returns: none
 */
class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    bleFlag = true;
  }
};

// ============================ Setup / Loop ============================
void setup() {
  // BLE server with one read/write characteristic
  BLEDevice::init("Nicolae + Ryan esp");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE
  );
  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  pServer->getAdvertising()->start();

  lcd.init();
  lcd.backlight();
  lcd.print(counter);

  // 1 s auto-reloading alarm
  timer = timerBegin(TIMER_HZ);
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, TIMER_HZ, true, 0);

  // Pull-up: pin idles HIGH, press pulls LOW
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), onButton, FALLING);
}

void loop() {
  if (buttonFlag) {
    buttonFlag = false;
    showMessage("Button Pressed");
  }
  if (bleFlag) {
    bleFlag = false;
    showMessage("New Message!");
  }

  // Counter paused while a message is up, ticks are discarded
  if (showingMsg) {
    tickFlag = false;
    if (millis() - msgStart >= MSG_TIME_MS) {
      showingMsg = false;
      lcd.clear();
      lcd.print(counter);
    }
    return;
  }

  if (tickFlag) {
    tickFlag = false;
    counter++;
    lcd.setCursor(0, 0);
    lcd.print(counter);
  }
}

// ======================= Function Implementations =====================

/*
 * Timer ISR, fires every second.
 * Params: none   Returns: none
 */
void IRAM_ATTR onTimer() {
  tickFlag = true;
}

/*
 * Button ISR, fires on press. Ignores bounces within DEBOUNCE_MS.
 * Params: none   Returns: none
 */
void IRAM_ATTR onButton() {
  unsigned long now = millis();
  if (now - lastPress > DEBOUNCE_MS) {
    buttonFlag = true;
    lastPress = now;
  }
}

/*
 * Displays msg and pauses the counter for MSG_TIME_MS 
 * Params: msg - text to display   Returns: none
 */
void showMessage(const char *msg) {
  lcd.clear();
  lcd.print(msg);
  showingMsg = true;
  msgStart = millis();
}