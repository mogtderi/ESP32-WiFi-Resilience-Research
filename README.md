# ESP32-WiFi-Resilience-Research
Experimental research framework evaluating wireless network resilience against 802.11 frame injection using ESP32 and OLED HUD.
# ESP32 Wi-Fi Network Resilience & Frame Injection Analysis

This repository contains the experimental setup, source code, and analysis framework for evaluating wireless network resilience against 802.11 management frame injection (Broadcast/Targeted Deauthentication and Disassociation) using an ESP32 DevKit V1 and an OLED display.

## 🔬 Research Overview
Modern Wi-Fi standards (Wi-Fi 4 / Wi-Fi 6) and operating system network stacks implement robust link-layer defenses and Management Frame Protection (MFP / 802.11w). This research investigates the effectiveness of unmanaged frame injection techniques against contemporary mobile hotspots (e.g., Redmi Note 14S) and client network interface cards (e.g., Intel Wi-Fi 6 AX201).

## 🛠 Hardware Requirements
* **Microcontroller:** ESP32 DevKit V1 (ESP-WROOM-32)
* **Display:** 0.96" I2C OLED Display (SSD1306, Address: `0x3C`)
* **Target AP:** Smartphone Hotspot (2.4 GHz, Channel 2)
* **Target Client:** Intel Wi-Fi 6 AX201 Wireless Adapter

## 🔌 Wiring Pinout (Direct I2C)
| ESP32 Pin | OLED Pin | Description |
|-----------|----------|-------------|
| 3V3       | VCC      | Power Supply|
| GND       | GND      | Ground      |
| GPIO 21   | SDA      | I2C Data    |
| GPIO 22   | SCL      | I2C Clock   |

## 📊 Experimental Findings
* **Resilience Analysis:** High-rate broadcast and targeted frame injection resulted in **0% packet loss** on the target client during active ping sessions.
* **Conclusion:** Modern client operating systems and hardware adapters successfully filter unencrypted management frames, demonstrating high link-layer resilience and validating the necessity of robust security extensions like 802.11w.

## 🚀 Getting Started
1. Open `src/wifi_resilience_hud.ino` in Arduino IDE.
2. Select **ESP32 Dev Module** and configure your target channel and MAC addresses.
3. Upload the firmware to your ESP32.
