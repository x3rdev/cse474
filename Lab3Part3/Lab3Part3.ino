#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <LiquidCrystal_I2C.h>

#define SERVICE_UUID "1731c169-65a1-418d-9628-68a937a81cdc"
#define CHARACTERISTIC_UUID "49c15281-84d1-488a-94e7-90c72557e4df"

#define BUTTON_PIN 7
#define MSG_TIME_MS 2000

LiquidCrystal_I2C lcd(0x27, 16, 2);
hw_timer_t *timer = NULL;

// Flags set by interrupts, read in loop(). volatile because ISRs change them.
volatile bool tickFlag = false;
volatile bool buttonFlag = false;
volatile bool bleFlag = false;
volatile unsigned long lastPress = 0;

int counter = 0;
bool showingMsg = false;
unsigned long msgStart = 0;

// BLE interrupt: runs when LightBlue writes to the characteristic
class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    bleFlag = true;
  }
};

// Timer ISR: fires every second
void IRAM_ATTR onTimer() {
  tickFlag = true;
}

// Button ISR: fires on press, ignores bounces within 200 ms
void IRAM_ATTR onButton() {
  unsigned long now = millis();
  if (now - lastPress > 200) {
    buttonFlag = true;
    lastPress = now;
  }
}

void setup() {
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
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();

  // LCD
  lcd.init();
  lcd.backlight();
  lcd.print(counter);

  // Timer: 1 MHz tick rate, alarm every 1,000,000 ticks = 1 s, auto-reload
  timer = timerBegin(1000000);
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000000, true, 0);

  // Button: pull-up, interrupt on press (HIGH -> LOW)
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), onButton, FALLING);
}

// Show a message and pause the counter for 2 s
void showMessage(const char *msg) {
  lcd.clear();
  lcd.print(msg);
  showingMsg = true;
  msgStart = millis();
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