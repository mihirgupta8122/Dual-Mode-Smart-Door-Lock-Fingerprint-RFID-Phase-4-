
# Dual‑Mode Smart Door Lock (Fingerprint + RFID) – ESP32 (Phase‑3)

## Project Overview
This project is a Dual‑Mode Smart Door Lock made with an ESP32, R307 fingerprint sensor, RC522 RFID reader, and a servo lock actuator. The system unlocks when either the fingerprint or RFID card matches a stored authorized credential. This is the Phase‑3 version, focusing on system design, OOP structure, UML diagrams, and automation scripts.

## Hardware Used
- ESP32 DevKit
- RC522 RFID module
- R307 Fingerprint sensor
- Servo motor / solenoid
- LEDs + Buzzer
- 5V USB/battery power

## Software Architecture
- AccessMethod (base interface)
- RFIDAccess / FingerprintAccess (authentication modules)
- LockActuator / SolenoidLockActuator (locking hardware)
- LockController (manages authentication + auto‑lock)
- AuthStorage (LittleFS storage for IDs)
- EventBus (Observer pattern for events)
- Logger (INFO/WARN/ERROR logs)
- Config (pins + timings)

## Repository Structure
```
Dual-Mode-Smart-Door-Lock/
│
├── src/
│   ├── *.h / *.cpp files (full ESP32 firmware)
│
├── docs/
│   ├── architecture.md
│   ├── sample-serial-output.txt
│   ├── diagrams/
│       ├── class_diagram.png
│       ├── use_case_diagram.png
│       ├── sequence_diagram.png
│       ├── tinkercad_diagram.png
│
├── scripts/
│   ├── build_upload.sh
│   ├── monitor_logs.sh
│
├── platformio.ini
└── report/
    ├── Phase3_Report_Group9.pdf
```

## UML Diagrams (Phase‑3)
Stored in `docs/diagrams/`:
- Class Diagram
- Use Case Diagram
- Sequence Diagram
- Tinkercad wiring diagram

## Shell Scripts (Automation)
### Script 1 – build_upload.sh
Automates compile + upload using `arduino-cli`.

### Script 2 – monitor_logs.sh
Filters ESP32 serial logs (RFID/Fingerprint events).

## How to Run
To compile & upload:
```
bash scripts/build_upload.sh
```
To monitor serial logs:
```
bash scripts/monitor_logs.sh
```

## Team Members
- Mihir Gupta
- Saihaj
- Altesse
- Majd

## Status
Phase‑3 completed with full ESP32 hardware support.
