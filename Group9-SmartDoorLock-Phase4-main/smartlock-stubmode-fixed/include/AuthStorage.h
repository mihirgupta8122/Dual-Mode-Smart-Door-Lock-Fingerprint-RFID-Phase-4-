#pragma once
#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <vector>
#include <algorithm>

// Holds the allow-lists we persist to /authorized.json
struct AuthStore {
  std::vector<String> rfid_uids; // e.g., "DE AD BE EF"
  std::vector<int>    fp_ids;    // fingerprint template IDs (1..200)
};

class AuthStorage {
  String path;

public:
  explicit AuthStorage(const String& p) : path(p) {}

  // Mount FS and ensure the file exists with a valid empty JSON object
  bool begin() {
    if (!LittleFS.begin(true)) return false;

    if (!LittleFS.exists(path)) {
      File f = LittleFS.open(path, "w");
      if (!f) return false;

      // IMPORTANT: quotes in JSON must be escaped inside a C++ string literal
      f.print("{\"rfid_uids\":[],\"fp_ids\":[]}");
      f.close();
    }
    return true;
  }

  // Load the allow-lists from JSON
  bool load(AuthStore& out) {
    File f = LittleFS.open(path, "r");
    if (!f) return false;

    JsonDocument doc;                 // ArduinoJson v7
    DeserializationError err = deserializeJson(doc, f);
    f.close();
    if (err) return false;

    out.rfid_uids.clear();
    out.fp_ids.clear();

    // Safe, const iteration with v7
    for (JsonVariantConst v : doc["rfid_uids"].as<JsonArrayConst>()) {
      const char* s = v.as<const char*>();
      if (s && *s) out.rfid_uids.push_back(String(s));
    }
    for (JsonVariantConst v : doc["fp_ids"].as<JsonArrayConst>()) {
      out.fp_ids.push_back(v.as<int>());
    }
    return true;
  }

  // Save the allow-lists to JSON
  bool save(const AuthStore& in) {
    JsonDocument doc;  // v7 dynamic document

    // Build arrays with the modern API
    JsonArray a = doc["rfid_uids"].to<JsonArray>();
    for (const auto& s : in.rfid_uids) a.add(s);

    JsonArray b = doc["fp_ids"].to<JsonArray>();
    for (int id : in.fp_ids) b.add(id);

    File f = LittleFS.open(path, "w");
    if (!f) return false;

    serializeJson(doc, f);
    f.close();
    return true;
  }
};

// -------- Small helpers for the allow-lists --------
inline bool containsUID(const AuthStore& s, const String& uid) {
  for (const auto& u : s.rfid_uids) if (u == uid) return true;
  return false;
}
inline bool containsFP(const AuthStore& s, int id) {
  for (int i : s.fp_ids) if (i == id) return true;
  return false;
}
inline void addUID(AuthStore& s, const String& uid) {
  if (!containsUID(s, uid)) s.rfid_uids.push_back(uid);
}
inline void delUID(AuthStore& s, const String& uid) {
  s.rfid_uids.erase(std::remove(s.rfid_uids.begin(), s.rfid_uids.end(), uid), s.rfid_uids.end());
}
inline void addFP(AuthStore& s, int id) {
  if (id > 0 && !containsFP(s, id)) s.fp_ids.push_back(id);
}
inline void delFP(AuthStore& s, int id) {
  s.fp_ids.erase(std::remove(s.fp_ids.begin(), s.fp_ids.end(), id), s.fp_ids.end());
}