# Sacred Lamp (ESP32 Web Server)

Hey dragons, this Sacred Lamp is an ESP32-based project that creates a standalone WiFi Access Point and hosts an interactive, beautifully animated web interface. Through the mobile-friendly web app, users can tap to "ignite" or "extinguish" five virtual traditional lamps, which seamlessly toggle five physical LEDs connected to the ESP32 hardware.

## ✨ Features
- **Standalone Network:** Operates as a WiFi Access Point (no external router required).
- **Interactive Web Interface:** Beautiful SVG-based traditional oil lamps.
- **Fluid Animations:** Features flame flickering, glowing pulses, ripple click effects, and smooth hardware-accelerated CSS animations.
- **Responsive Design:** Optimized for mobile screens with touch-friendly interactions.
- **Asynchronous Updates:** Uses the JavaScript Fetch API to toggle LEDs without reloading the page.

## 🛠 Hardware Requirements
- 1x ESP32 Development Board
- 5x LEDs (Yellow or warm-white recommended to match the theme)
- 5x Current-limiting resistors (e.g., 220Ω or 330Ω)

you can also go with more powerful lights and control them through a relay module.

## 🔌 Pin Configuration
Connect the anodes (long leg) of your LEDs to the following ESP32 GPIO pins (in series with a resistor), and the cathodes (short leg) to the ESP32's `GND` pin.

| Lamp (UI) | ESP32 GPIO Pin |
| :--- | :--- |
| **Lamp 1** (Top Center) | `GPIO 14` |
| **Lamp 2** (Top Right) | `GPIO 27` |
| **Lamp 3** (Bottom Right) | `GPIO 26` |
| **Lamp 4** (Bottom Left) | `GPIO 25` |
| **Lamp 5** (Top Left) | `GPIO 33` |

## 🚀 Installation & Setup
1. Install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Ensure you have the **ESP32 board package** installed (`Tools > Board > Boards Manager...` -> search for `esp32` by Espressif Systems).
3. Create a new sketch and paste the provided code.
4. Connect your ESP32 to your computer via USB.
5. Select your specific ESP32 board model and COM port from the `Tools` menu.
6. Click **Upload**.

## 📱 How to Use
1. Power on your ESP32.
2. Open the WiFi settings on your smartphone or computer and look for the new network.
3. Connect using the following credentials:
   - **Network Name (SSID):** `SacredLamp`
   - **Password:** `12345678`
4. Open a web browser and navigate to the ESP32's default AP IP address:
   - **`http://192.168.4.1`**
   - *(Note: You can verify the exact IP address by opening the Arduino IDE Serial Monitor at `115200` baud during boot).*
5. Tap any of the 5 lamps on the screen to ignite them and watch your physical LEDs turn on!

## 📁 Code Overview
- **WIFI Setup:** Configures the ESP32 strictly in `WIFI_AP` (Access Point) mode so it can act as its own router.
- **GPIO Handler:** Initializes pins as outputs and tracks the boolean state of the 5 LEDs in an array.
- **Web App (`PROGMEM`):** Stores the entire HTML, CSS, and JS front-end application in the ESP32's flash memory to conserve RAM. It heavily utilizes inline SVGs for the lamps and CSS `@keyframes` for performance-friendly animations.
- **Web Server:** Uses the standard `WebServer.h` library to handle root (`/`) requests to serve the interface, and dedicated `/ledX` endpoints to handle state changes triggered by the frontend.
