This project is sponsored by [JLCPCB](https://jlcpcb.com/?from=cld). Check their website [here](https://jlcpcb.com/?from=cld).

This is a ~15$ (US) DIY macropad called OpenMacro featuring 12 customizable buttons. While I won't be actively pursuing further development of this project in the immediate future, I'm sharing the files for anyone interested in exploring and enhancing it. While I can't fully endorse its functionality due to limited testing, I encourage you to experiment with it and perhaps even take it to new heights. Who knows, there might be a resurgence of interest in this project down the line! The PCB and Schematics provided are EasyEDA files (.json) and the code is Arduino code to program with Arduino IDE. Enjoy tinkering and creating!



## How to Use Guide for OpenMacro

1. **Setup WiFi Connection:**
   - Replace `"YOUR_WIFI_NAME"` and `"YOUR_PASSWORD"` with your WiFi credentials in the variables `ssid` and `password` respectively.

2. **Button Configuration:**
   - Each button on the macropad is assigned a specific function (e.g., printing a character).
   - Modify the functions `buttonPressed12()`, `buttonPressed14()`, etc., to change the characters printed when respective buttons are pressed.

3. **Initial Setup:**
   - Connect the macropad to your computer via USB.
   - Upload the code to the macropad using the Arduino IDE or another compatible development environment.
   - Ensure that the required libraries (`BleKeyboard`, `Adafruit_NeoPixel`, `WiFi`, `ESPmDNS`, `WiFiUdp`, `ArduinoOTA`) are installed.

4. **Functionality:**
   - The macropad acts as a BLE keyboard and sends key presses to connected devices.
   - Buttons 12 to 18 correspond to characters "a" to "h" respectively, while buttons 23 to 4 correspond to characters "i" to "l" respectively.

5. **Battery Monitoring:**
   - The code includes battery level monitoring functionality. Ensure that the voltage divider factor is correctly calibrated for accurate readings.

6. **Deep Sleep Mode:**
   - The macropad enters deep sleep mode after a period of inactivity to conserve power.
   - Adjust `DEEP_SLEEP_TIMEOUT` and `DEEP_SLEEP_TIMEOUT_NOT_CONNECTED` variables to change the sleep timeout duration.

7. **Over-the-Air (OTA) Updates:**
   - The macropad supports OTA updates for firmware upgrades.
   - When button 12 is held down for 5 seconds, the macropad enters OTA mode, indicated by blinking blue lights.

8. **LED Indicator:**
   - The NeoPixel LEDs provide visual feedback.
   - Green light indicates Bluetooth connection.
   - Red light indicates low battery (below 25%).
   - Rainbow cycle indicates normal operation.

9. **Debugging:**
   - Serial output can be monitored for debugging purposes by connecting the macropad to a computer via USB and opening the serial monitor.

10. **Customization:**
   - Modify the code according to your requirements, such as changing button functions, LED patterns, or adding new features. If you make useful changes/bug fixes, I will be happy to accept pull requests!

