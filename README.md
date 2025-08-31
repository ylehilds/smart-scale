# Smart Scale

An **IoT-based smart scale system** that measures weight and includes a **goal/reward mechanism**. When a user hits a target weight or fitness milestone, the system can trigger external rewards/notifications via **Adafruit IO + IFTTT** or activate a **smart lock** for a physical reward. The repo demonstrates embedded IoT development, hardware–software integration, and cloud automation.

## ✨ Features
- **Weight Measurement** — Reads load cell data (via HX711 amplifier) in Arduino firmware.
- **Goal & Reward System** — Compares measured weight against user-defined goals and triggers a reward when the goal is met.
- **Smart Lock Integration** — `smartLock.ino` drives a servo/relay to unlock/trigger a mechanism as a physical reward.
- **Adafruit IO + IFTTT** — `smartScaleUsingAdafruitIFTTT` publish goal events and trigger automations/notifications.
- **AWS** - data is then stored in AWS DynamoDB table
- **User Interface then displays data points in a chart and allows user to enter goals and dates** - 
- **Modular Iterations** — Multiple versions explore different wiring, logic flows, and cloud integrations.

## 🛠 Tech Stack
- **Embedded:** Arduino / C++ (`.ino` sketches)
- **Boards:** Arduino-compatible (ESP8266, ESP32, or Arduino Uno—depending on your setup)
- **Cloud & Automation:** Adafruit IO (feeds/dashboards), IFTTT (applets/notifications), AWS (IoT, DynamoDB, Lambdas)
- **Electronics:** Load cell + **HX711** amplifier; servo/relay for lock control
- **Arduino Libraries (common):** HX711, ESP8266WiFi/WiFi (if Wi-Fi board), Adafruit_MQTT (for Adafruit IO)

## 🚀 Getting Started
**Prerequisites**
- Arduino IDE (or PlatformIO)
- Hardware: Load cell, HX711, servo/relay (for lock), Arduino/ESP8266/ESP32 board
- Accounts/keys (for cloud integration): Adafruit IO, IFTTT

**Clone & Open**
- $ git clone https://github.com/ylehilds/smart-scale.git
- $ cd smart-scale
- Open a sketch in the Arduino IDE, e.g.:
    - `smartLock.ino`
    - `smartScaleUsingAdafruitIFTTT/smartScaleUsingAdafruitIFTTT.ino`

**Board Setup & Upload**
1) Connect your Arduino or ESP8266/ESP32.
2) In Arduino IDE: select the correct **Board** and **Port**.
3) Enter Wi-Fi and service credentials in the sketch (where indicated).
4) Upload the sketch.

**Configure Adafruit IO + IFTTT (for the IFTTT variants)**
1) Create Adafruit IO feeds (e.g., `weight`, `goal_status`).
2) Create an IFTTT applet that triggers on Adafruit IO feed updates (e.g., when `goal_status` == achieved).
3) Add your **Adafruit IO Key**, **Wi-Fi SSID/Pass**, and feed names to the sketch and re-upload.

## 📂 Project Structure
.
├── smartLock.ino                    (smart lock controller for physical rewards)  
├── smartScale/                      (base smart scale code)  
├── smartScaleUsingAdafruitIFTTT/    (scale integrated with Adafruit IO + IFTTT)  
├── smartScaleUsingAdafruitIFTTTV2/  (refined Adafruit IO + IFTTT version)  
├── .gitignore / .idea               (project metadata)  
└── README.md / LICENSE

## 🔭 Potential Use Cases
- Fitness **gamification** and adherence
- Reward-based weight loss/maintenance programs
- Smart-home triggers on health milestones
- Prototype for connected healthcare/health-tech devices

## 🧩 Notes & Tips
- Calibrate the **HX711** with known weights to improve accuracy.
- For ESP8266/ESP32, ensure proper level-shifting/wiring for HX711.
- Keep IFTTT applets simple (single responsibility) for clarity and reliability.

## 📜 License
This project is licensed under the **MIT License** — see `LICENSE` for details.

## 👤 Author
**Lehi Alcantara**  
Website: https://www.lehi.dev  
Email: lehi@lehi.dev