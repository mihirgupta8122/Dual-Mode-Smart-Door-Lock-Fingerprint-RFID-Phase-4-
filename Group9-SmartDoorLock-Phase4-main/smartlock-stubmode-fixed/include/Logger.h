
#pragma once
#include <Arduino.h>
struct Logger{ static void info(const String&s){ Serial.println("[INFO] "+s);} static void warn(const String&s){ Serial.println("[WARN] "+s);} static void error(const String&s){ Serial.println("[ERROR] "+s);} };
