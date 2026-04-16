
#include <Arduino.h>
#include "Config.h"
#include "Logger.h"
#include "EventBus.h"
#include "AuthStorage.h"
#include "LockActuator.h"
#include "SolenoidLockActuator.h"
#include "RFIDAccess.h"
#include "FingerprintAccess.h"
#include "LockController.h"
#include "BuzzerLEDFeedback.h"

EventBus BUS; 
SolenoidLockActuator ACTUATOR; 
BuzzerLEDFeedback FEEDBACK; 

AuthStore STORE; 
AuthStorage STORAGE("/authorized.json");

RFIDAccess *RFIDM = nullptr; 
FingerprintAccess *FPA = nullptr; 
LockController *CTRL = nullptr;

static bool askIntFromSerial(int &out, unsigned long ms){ unsigned long t0=millis(); out=-1; while(millis()-t0<ms){ if(Serial.available()){ out = Serial.parseInt(); if(out>0) return true; } delay(10);} return false; }

static void adminWindow(){
  Logger::info("[ADMIN] Window opened");
  BuzzerLEDFeedback::buzz(); FEEDBACK.green(2);
  unsigned long t0 = millis();
  while(millis()-t0 < ADMIN_WINDOW_MS){
#ifdef STUB_MODE
    if(Serial.available()){
      int c = Serial.read();
      if(c=='e'){ Logger::info("Enter FP ID to enroll (1..200) within 8s"); int id; if(askIntFromSerial(id,8000)){ addFP(STORE,id); STORAGE.save(STORE); Logger::info("(STUB) Enroll OK"); FEEDBACK.green(3); BuzzerLEDFeedback::buzz(180);} }
      if(c=='d'){ Logger::info("Enter FP ID to delete within 8s"); int id; if(askIntFromSerial(id,8000)){ delFP(STORE,id); STORAGE.save(STORE); Logger::info("(STUB) Delete OK"); FEEDBACK.green(2);} }
      if(c=='c'){ String u="DE AD BE EF"; addUID(STORE,u); STORAGE.save(STORE); Logger::info("(STUB) RFID authorized: "+u); FEEDBACK.green(2);} 
      if(c=='x'){ String u="DE AD BE EF"; delUID(STORE,u); STORAGE.save(STORE); Logger::info("(STUB) RFID removed: "+u); FEEDBACK.red(1);} 
    }
#else
    if(digitalRead(BTN_ENROLL)==LOW){ Logger::info("Enter FP ID to enroll (1..200) within 8s"); int id; if(askIntFromSerial(id,8000)){ FPA->setEnrollId(id); Logger::info("Place finger #1"); unsigned long e0=millis(); while(millis()-e0<ENROLL_TIMEOUT_MS){ if(FPA->captureOnce(1)==FINGERPRINT_OK) break; delay(100);} Logger::info("Remove finger..."); delay(1500); Logger::info("Place SAME finger #2"); e0=millis(); while(millis()-e0<ENROLL_TIMEOUT_MS){ if(FPA->captureOnce(2)==FINGERPRINT_OK) break; delay(100);} if(FPA->createModel()==FINGERPRINT_OK && FPA->storeModel()==FINGERPRINT_OK){ addFP(STORE,id); STORAGE.save(STORE); Logger::info("Enroll OK"); FEEDBACK.green(3); BuzzerLEDFeedback::buzz(180);} else { Logger::warn("Enroll failed"); FEEDBACK.red(3);} break; } }
    if(digitalRead(BTN_DELETE)==LOW){ Logger::info("Enter FP ID to delete within 8s"); int id; if(askIntFromSerial(id,8000)){ uint8_t r = FPA->deleteModel(id); if(r==FINGERPRINT_OK){ delFP(STORE,id); STORAGE.save(STORE); Logger::info("Delete OK"); FEEDBACK.green(2);} else { Logger::warn("Delete failed"); FEEDBACK.red(3);} break; } }
    String uid; if(RFIDM->readOneUID(uid)){ Logger::info(String("Saw UID: ")+uid+" — press ENROLL to authorize, DELETE to remove (5s)"); unsigned long w=millis(); bool decided=false; while(millis()-w<5000 && !decided){ if(digitalRead(BTN_ENROLL)==LOW){ addUID(STORE,uid); STORAGE.save(STORE); Logger::info("RFID authorized"); FEEDBACK.green(2); decided=true; } if(digitalRead(BTN_DELETE)==LOW){ delUID(STORE,uid); STORAGE.save(STORE); Logger::info("RFID removed"); FEEDBACK.red(1); decided=true; } } }
#endif
  }
}

void setup(){
  Serial.begin(115200); delay(200); Logger::info("Boot: Fingerprint + RFID Smart Lock (STUB compatible)");
  pinMode(BTN_ENROLL, INPUT_PULLUP); pinMode(BTN_DELETE, INPUT_PULLUP);
  FEEDBACK.begin();
  ACTUATOR.begin(); ACTUATOR.lock();
  if(!STORAGE.begin()) Logger::error("LittleFS mount failed");
  STORAGE.load(STORE);

  static RFIDAccess rfid(&STORE); rfid.begin(); RFIDM=&rfid;
  static FingerprintAccess fp(&STORE); fp.begin();    FPA=&fp;

  std::vector<AccessMethod*> methods; methods.push_back(RFIDM); methods.push_back(FPA);
  static LockController controller(methods, &ACTUATOR, &BUS); CTRL=&controller; CTRL->begin();

  BUS.subscribe([](const Event&e){ switch(e.type){ case EventType::AccessGranted: Logger::info("Event: AccessGranted"); FEEDBACK.green(2); BuzzerLEDFeedback::buzz(); break; case EventType::AccessDenied: Logger::warn("Event: AccessDenied"); FEEDBACK.red(2); BuzzerLEDFeedback::buzz(60); break; case EventType::Locked: Logger::info("Event: Locked"); break; case EventType::Unlocked: Logger::info("Event: Unlocked"); break; case EventType::SystemFault: Logger::error("Event: SystemFault"); break; } });

  Logger::info("Controls (when board attached): 'r' RFID, 'p' FP, 'a' admin → 'e'/'d'/'c'/'x'");
}

void loop(){
  if(RFIDM->checkAdminTap()) adminWindow();
  CTRL->poll();
  CTRL->tick();
  delay(20);
}
