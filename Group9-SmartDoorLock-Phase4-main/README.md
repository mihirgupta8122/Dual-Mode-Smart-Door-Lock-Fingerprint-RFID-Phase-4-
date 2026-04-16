Dual‑Mode Smart Door Lock (Fingerprint + RFID) – ESP32
Phase‑4: Implementation, Performance & Deployment

📌 Project Overview
This project is a Dual‑Mode Smart Door Lock system built using an ESP32, RFID authentication, and fingerprint authentication, with a servo‑based locking mechanism and LED/buzzer feedback. The system unlocks when either a valid RFID card or a registered fingerprint is detected.
Phase‑4 represents the final working implementation, focusing on:

Hardware‑integrated firmware
Task‑oriented, event‑driven control flow
Performance behavior and responsiveness
Local embedded deployment
Live demonstration readiness

The system runs entirely locally on the ESP32, with no cloud or server dependency.

✅ Key Features

Dual authentication: RFID + Fingerprint
Fingerprint enrollment via hardware trigger
Servo‑controlled lock/unlock mechanism
Automatic re‑lock after timeout
Real‑time LED and buzzer feedback
Event‑driven, non‑blocking logic
Designed for future FreeRTOS task expansion


🧰 Hardware Used

ESP32 DevKit (ESP‑WROOM‑32)
RC522 RFID Module
Optical Fingerprint Sensor (UART)
Servo Motor (lock actuator)
LEDs and Buzzer
Breadboard and jumper wires
Stable 5V power supply


🧠 Software Architecture (Phase‑4)
The firmware is written in C++ (Arduino on ESP32) and structured with clean separation of responsibilities:

RFID authentication logic
Fingerprint enrollment and matching
Access‑control decision logic
Servo actuation and auto‑lock timing
LED and buzzer feedback system

Although implemented using Arduino’s loop‑based execution model, the code is organized in a task‑oriented and event‑driven structure, making it well‑suited for future FreeRTOS task migration.

📁 Repository Structure (Matches Actual Files)
Group9-SmartDoorLock-Phase4/
│
├── smartlock-submode-fixed/
│
├── UML diagram/
│   ├── Screenshot 2026-03-24 ... (Class, Use Case, Sequence)
│
├── docs/
│   ├── architecture.md
│   ├── sample-serial-output.txt
│
├── include/
│   ├── AccessMethod.h
│   ├── AuthStorage.h
│   ├── BuzzerLEDFeedback.h
│   ├── Config.h
│   ├── EventBus.h
│   ├── FingerprintAccess.h
│   ├── LockActuator.h
│   ├── LockController.h
│   ├── Logger.h
│   ├── RFIDAccess.h
│   ├── SolenoidLockActuator.h
│
├── src/
│   └── main.cpp
│
├── tinkercad work/
│   ├── Wiring screenshots
│   ├── _FingerPrint DoorLock.pdf
│
├── platformio.ini
├── maincode.cpp
├── README.md
├── README_PHASE3.md
└── .DS_Store

✅ This structure reflects your actual implementation, UML work, and documentation.

📐 UML & Design Artifacts
Located in the UML diagram/ and docs/ folders:

Class Diagram
Use Case Diagram
Sequence Diagram
Tinkercad hardware diagrams

These diagrams guided the Phase‑4 implementation and closely match the final firmware structure.

⚙️ Shell Scripting & Tooling
Automation and tooling support project development:

Serial output logging (sample outputs in docs/)
PlatformIO configuration (platformio.ini)
Clean modular header/source organization


🚀 Deployment (Phase‑4)
Deployment Type
✅ Local Embedded Deployment

Firmware compiled and uploaded from Windows
Uses Arduino IDE / PlatformIO
ESP32 connected via USB
System operates independently after flashing

No Cloud / No Server

No Wi‑Fi, cloud services, or external servers used
All authentication and control runs on‑device


📊 Performance & Reliability

Responsive authentication during servo movement
No system freezing during fingerprint processing
Consistent auto‑lock behavior
Stable performance during repeated scans

Performance improvements are achieved through event‑driven logic and careful control‑flow design.

🔮 Future Improvements

Mobile‑based authentication
Centralized logging (local server – future work)
Multi‑door deployment
Explicit FreeRTOS task implementation
Improved persistent credential storage


👥 Team Members

Mihir Gupta
Saihaj
Altesse
Majd


📄 Project Status
✅ Phase‑4 completed
✅ Fully working physical hardware
✅ Ready for live demonstration and evaluation
