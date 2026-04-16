
#pragma once
#include <Arduino.h>

// ===== Admin Card UID (uppercase hex with spaces) =====
static String ADMIN_UID = "DE AD BE EF"; // For real hardware; in STUB_MODE we simulate

// ===== Pins for real hardware =====
// RC522 (SPI)
static const uint8_t MFRC522_SS_PIN  = 5;   // SDA/SS
static const uint8_t MFRC522_RST_PIN = 27;  // RST
// SPI defaults: SCK=18, MISO=19, MOSI=23

// Fingerprint sensor (Serial2)
static const int FP_RX_PIN = 16;  // ESP32 RX2 (to sensor TX)
static const int FP_TX_PIN = 17;  // ESP32 TX2 (to sensor RX)
static const uint32_t FP_BAUD   = 57600;

// Actuator (relay/solenoid)
static const uint8_t RELAY_PIN = 26; // active HIGH

// Feedback pins (optional)
static const uint8_t GREEN_LED = 25;
static const uint8_t RED_LED   = 33;
static const uint8_t BUZZER_PIN= 32;

// Buttons (active LOW with INPUT_PULLUP)
static const uint8_t BTN_ENROLL = 14;
static const uint8_t BTN_DELETE = 12;

// Behavior
static const uint32_t AUTO_LOCK_MS     = 5000;  // re-lock after 5 s
static const uint32_t ADMIN_WINDOW_MS  = 10000; // 10 s
static const uint32_t ENROLL_TIMEOUT_MS= 20000; // 20 s per capture (real hw)
