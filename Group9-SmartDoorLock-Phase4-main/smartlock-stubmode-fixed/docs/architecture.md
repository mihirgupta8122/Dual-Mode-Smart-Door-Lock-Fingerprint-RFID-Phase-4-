
# Architecture (Non‑IoT, Fingerprint + RFID)

- `AccessMethod` → `RFIDAccess` / `FingerprintAccess`
- `LockActuator` → `SolenoidLockActuator`
- `LockController` orchestrates tries + auto‑lock
- `EventBus` decouples feedback (`BuzzerLEDFeedback`) from core logic
- `AuthStorage` (LittleFS + ArduinoJson v7) persists `/authorized.json`

**Admin Mode**: admin card (or stub key 'a') opens 10s window → actions to enroll/delete FP IDs or authorize/remove RFID UIDs.
