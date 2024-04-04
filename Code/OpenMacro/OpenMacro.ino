//The OpenMacro by The Young Maker theyoungmaker.ddns.net

#include <BleKeyboard.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_PASWORD";

#define DEBUG_MODE
#define NEOPIXEL_PIN 13
#define NUMPIXELS 6
bool blink = false;
bool ota = false;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

uint8_t BatteryLevel;
unsigned long previousMillisBattery = 0;
const int batteryPin = 34;  // ADC pin for battery voltage (GPIO34)
const float voltageDividerFactor = 2.20779220779;

bool wifiInitialized = false;
const unsigned long DEEP_SLEEP_TIMEOUT = 5 * 60 * 1000;                // 5 minutes in milliseconds
const unsigned long DEEP_SLEEP_TIMEOUT_NOT_CONNECTED = 1 * 60 * 1000;  // 1 minutes in milliseconds
unsigned long lastButtonPressMillis = 0;

unsigned long button12PressedTime = 0;
bool button12Pressed = false;

void buttonPressed12() {
  if (!button12Pressed) {
    Keyboard.print("a");
    button12PressedTime = millis();
    button12Pressed = true;
  } else {
    if (millis() - button12PressedTime >= 5000) {
      if (digitalRead(12) == HIGH) {
        ota = !ota;
      }
      button12Pressed = false;
    }
  }
}

void buttonPressed14() {
  Keyboard.print("b");
  lastButtonPressMillis = millis();
}

void buttonPressed27() {
  Keyboard.print("c");
  lastButtonPressMillis = millis();
}

void buttonPressed26() {
  Keyboard.print("d");
  lastButtonPressMillis = millis();
}

void buttonPressed25() {
  Keyboard.print("e");
  lastButtonPressMillis = millis();
}

void buttonPressed33() {
  Keyboard.print("f");
  lastButtonPressMillis = millis();
}

void buttonPressed32() {
  Keyboard.print("g");
  lastButtonPressMillis = millis();
}

void buttonPressed35() {
  Keyboard.print("h");
  lastButtonPressMillis = millis();
}

void buttonPressed15() {
  Keyboard.print("i");
  lastButtonPressMillis = millis();
}

void buttonPressed23() {
  Keyboard.print("j");
  lastButtonPressMillis = millis();
}

void buttonPressed4() {
  Keyboard.print("k");
  lastButtonPressMillis = millis();
}

void buttonPressed18() {
  Keyboard.print("l");
  lastButtonPressMillis = millis();
}

void setup() {
  Serial.begin(115200);
  lastButtonPressMillis = millis();
  bleDevice.setName("Diy Macropad");
  bleDevice.setManufacturer("Espressif");
  bleDevice.setDelay(40);
  pinMode(12, INPUT_PULLDOWN);
  pinMode(14, INPUT_PULLDOWN);
  pinMode(27, INPUT_PULLDOWN);
  pinMode(26, INPUT_PULLDOWN);
  pinMode(25, INPUT_PULLDOWN);
  pinMode(33, INPUT_PULLDOWN);
  pinMode(32, INPUT_PULLDOWN);
  pinMode(35, INPUT_PULLDOWN);
  pinMode(15, INPUT_PULLDOWN);
  pinMode(23, INPUT_PULLDOWN);
  pinMode(4, INPUT_PULLDOWN);
  pinMode(18, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(12), buttonPressed12, RISING);
  attachInterrupt(digitalPinToInterrupt(14), buttonPressed14, RISING);
  attachInterrupt(digitalPinToInterrupt(27), buttonPressed27, RISING);
  attachInterrupt(digitalPinToInterrupt(26), buttonPressed26, RISING);
  attachInterrupt(digitalPinToInterrupt(25), buttonPressed25, RISING);
  attachInterrupt(digitalPinToInterrupt(33), buttonPressed33, RISING);
  attachInterrupt(digitalPinToInterrupt(32), buttonPressed32, RISING);
  attachInterrupt(digitalPinToInterrupt(35), buttonPressed35, RISING);
  attachInterrupt(digitalPinToInterrupt(15), buttonPressed15, RISING);
  attachInterrupt(digitalPinToInterrupt(23), buttonPressed23, RISING);
  attachInterrupt(digitalPinToInterrupt(4), buttonPressed4, RISING);
  attachInterrupt(digitalPinToInterrupt(18), buttonPressed18, RISING);
  Keyboard.begin();
  delay(3000);
  while (!bleDevice.isConnected()) {
    setAllPixelsColor(255, 0, 0);  // Red color
    pixels.show();
    Serial.print(".");
    delay(250);
  }
  delay(1000);


  pixels.begin();
  if (!blink) {
    blinkNeoPixel(0, 255, 0, 3);  // Green color
    blink = true;
  }

  Serial.println("BLE Connected!");
}

void loop() {
  if (bleDevice.isConnected()) {
    if (!blink) {
      blinkNeoPixel(0, 255, 0, 3);  // Green color
      blink = true;
    }
    rainbowCycle(50);
    checkForOta();
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillisBattery >= 30000) {
      // Read battery voltage from ADC pin 6 (GPIO34)
      int batteryValue1 = analogRead(batteryPin);
      int batteryValue2 = analogRead(batteryPin);
      int batteryValue3 = analogRead(batteryPin);
      int batteryValue4 = analogRead(batteryPin);

      // Apply voltage divider correction
      float batteryVoltage1 = batteryValue1 * (3.3 / 4095) * voltageDividerFactor;
      float batteryVoltage2 = batteryValue2 * (3.3 / 4095) * voltageDividerFactor;
      float batteryVoltage3 = batteryValue3 * (3.3 / 4095) * voltageDividerFactor;
      float batteryVoltage4 = batteryValue4 * (3.3 / 4095) * voltageDividerFactor;
      float batteryVoltage = (batteryVoltage1 + batteryVoltage2 + batteryVoltage3 + batteryVoltage4) / 4;

      // Print battery voltage to Serial monitor
#ifdef DEBUG_MODE
      Serial.print("Battery Voltage: ");
      Serial.print(batteryVoltage, 2);  // Print with 2 decimal places
      Serial.println("V");
#endif
      BatteryLevel = min(static_cast<uint8_t>((batteryVoltage / 4) * 100), static_cast<uint8_t>(100));
      previousMillisBattery = currentMillis;
      bleDevice.setBatteryLevel(BatteryLevel);
      if (BatteryLevel <= 25) {
        blinkNeoPixel(255, 0, 0, 5);  // Red color
      }
    }
    if (millis() - lastButtonPressMillis >= DEEP_SLEEP_TIMEOUT) {  // Check if it's time to enter deep sleep
      Serial.println("Entering deep sleep...");
      esp_sleep_enable_ext0_wakeup(GPIO_NUM_12, 1);
      esp_deep_sleep_start();
    }
  } else if (!bleDevice.isConnected()) {
    checkForOta();
    setAllPixelsColor(255, 0, 0);  // Red color
    pixels.show();
    blink = false;
    if (millis() - lastButtonPressMillis >= DEEP_SLEEP_TIMEOUT_NOT_CONNECTED) {  // Check if it's time to enter deep sleep
      setAllPixelsColor(0, 0, 0);                                                // No color
      pixels.show();

      Serial.println("Entering deep sleep...");
      esp_sleep_enable_ext0_wakeup(GPIO_NUM_12, 1);
      esp_deep_sleep_start();
    }
  }
}

// Function to set the color of all pixels
void setAllPixelsColor(uint8_t red, uint8_t green, uint8_t blue) {
  for (int i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, pixels.Color(red, green, blue));
    pixels.show();
  }
}
void blinkNeoPixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t blink) {
  for (int i = 0; i < blink; i++) {
    setAllPixelsColor(red, green, blue);
    delay(500);
    setAllPixelsColor(0, 0, 0);
    delay(500);
  }
}
void checkForOta() {
  if (ota == true) {
    if (!wifiInitialized) {
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password);
      ArduinoOTA.setHostname("OpenMacro");
      ArduinoOTA.setPassword("openmacro");
      ArduinoOTA
        .onStart([]() {
          String type;
          if (ArduinoOTA.getCommand() == U_FLASH)
            type = "sketch";
          else  // U_SPIFFS
            type = "filesystem";

          // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
          Serial.println("Start updating " + type);
        })
        .onEnd([]() {
          Serial.println("\nEnd");
        })
        .onProgress([](unsigned int progress, unsigned int total) {
          Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        })
        .onError([](ota_error_t error) {
          Serial.printf("Error[%u]: ", error);
          if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
          else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
          else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
          else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
          else if (error == OTA_END_ERROR) Serial.println("End Failed");
        });
      wifiInitialized = true;
    }
    ArduinoOTA.begin();
    blinkNeoPixel(0, 0, 255, 2);  // Blue color
    ArduinoOTA.handle();
  } else {
    ArduinoOTA.end();
    WiFi.disconnect();
    wifiInitialized = false;
  }
}
void rainbowCycle(uint8_t wait) {
  for (int j = 0; j < 256; j++) { // 256 cycles of all colors in the wheel
    for (int i = 0; i < pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
