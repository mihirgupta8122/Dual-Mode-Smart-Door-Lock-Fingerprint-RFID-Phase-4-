
#pragma once
#include "AccessMethod.h"
#include "Config.h"
#include "Logger.h"
#include "AuthStorage.h"

#ifdef STUB_MODE
class FingerprintAccess: public AccessMethod{ AuthStore* store; int enrollId=-1; public: FingerprintAccess(AuthStore* s): store(s) {} void begin(){ Logger::info("Fingerprint (STUB) ready — press 'p' to simulate auth"); } bool tryAuthenticate() override{ if(Serial.available()){ int c=Serial.read(); if(c=='p'){ Logger::info("(STUB) FP auth success"); return true; } } return false; } void setEnrollId(int id){ enrollId=id; } int getEnrollId() const { return enrollId; } uint8_t captureOnce(uint8_t){ return 0; } uint8_t createModel(){ return 0; } uint8_t storeModel(){ return 0; } uint8_t deleteModel(int){ return 0; } String listTemplates(){ return String("[]"); } };
#else
#include <HardwareSerial.h>
#include <Adafruit_Fingerprint.h>
class FingerprintAccess: public AccessMethod{ HardwareSerial* ser; Adafruit_Fingerprint finger; AuthStore* store; int enrollId=-1; public: FingerprintAccess(AuthStore* s): ser(&Serial2), finger(ser), store(s) {} void begin(){ ser->begin(FP_BAUD, SERIAL_8N1, FP_RX_PIN, FP_TX_PIN); delay(50); finger.begin(FP_BAUD); if(finger.verifyPassword()) Logger::info("Fingerprint sensor OK"); else Logger::error("Fingerprint sensor NOT found"); } bool tryAuthenticate() override{ if(finger.getImage()!=FINGERPRINT_OK) return false; if(finger.image2Tz()!=FINGERPRINT_OK) return false; if(finger.fingerSearch()!=FINGERPRINT_OK) return false; int id=finger.fingerID; bool ok=containsFP(*store,id); if(ok) Logger::info(String("FP authorized: ID=")+id); return ok; } void setEnrollId(int id){ enrollId=id; } int getEnrollId() const { return enrollId; } uint8_t captureOnce(uint8_t slot){ int p=finger.getImage(); if(p!=FINGERPRINT_OK) return p; return finger.image2Tz(slot); } uint8_t createModel(){ return finger.createModel(); } uint8_t storeModel(){ if(enrollId<=0) return FINGERPRINT_PACKETRECIEVEERR; return finger.storeModel(enrollId); } uint8_t deleteModel(int id){ return finger.deleteModel(id); } String listTemplates(){ String s="["; bool first=true; for(int id=1; id<=200; ++id){ if(finger.loadModel(id)==FINGERPRINT_OK){ if(!first) s+=","; s+=String(id); first=false; }} s+="]"; return s; } };
#endif
